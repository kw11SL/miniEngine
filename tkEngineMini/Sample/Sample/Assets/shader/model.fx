/*!
 * @brief	シンプルなモデルシェーダー。
 */

static const float PI = 3.1415926f;		//正規化ランバート拡散反射用の除算値

//ディレクションライト用構造体
struct DirectionLight
{
	float3 direction;
	float3 color;
};

//ポイントライト用構造体
struct PointLight
{
	float3 position;
	float3 color;
	float range;
};

//スポットライト用構造体
struct SpotLight
{
	float3 position;
	float3 color;
	float range;
	float3 direction;
	float angle;
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ディレクションライト用の定数バッファ
cbuffer DirectionLightCb :register(b1) {
	DirectionLight directionLight;	//ライト構造体
	float3 eyePos;					//視点の位置
	float3 ambientLig;				//アンビエントライト
};

//ポイントライト用の定数バッファ
cbuffer PointLightCb : register(b2) {
	PointLight pointLight;
};

//スポットライト用の定数バッファ
cbuffer SpotLightCb : register(b3) {
	SpotLight spotLight;
};

//関数宣言
//ベックマン分布を計算
float Beckmann(float m, float t);
//フレネルを計算
float SpcFresnel(float f0, float u);
//法線マップを取得
float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv);

float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);

//正規化ランバート拡散反射を計算
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V);
//クックトランスの鏡面反射モデルから鏡面反射を計算
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metalic);

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。

struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//頂点の法線
	float3 tangent	: TANGENT;
	float3 biNormal	: BINORMAL;
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//頂点の法線
	float3 tangent		: TANGENT;		
	float3 biNormal		: BINORMAL;
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;	//ワールド座標
	float3 normalInView : TEXCOORD2;	//カメラ空間の法線
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線マップ
Texture2D<float4> g_metallicSmooth : register(t2);		//メタリックスムースマップ

StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。

sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else{
		m = mWorld;
	}

	//座標変換
	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	//頂点法線をピクセルシェーダに渡す
	psIn.normal = mul(m, vsIn.normal);		//法線を回転させる
	psIn.normal = normalize(psIn.normal);
	psIn.uv = vsIn.uv;
	psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
	psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
	//カメラ空間の法線を求める
	psIn.normalInView = mul(mView, psIn.normal);

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	///////////////////////////////////////////////////////////////////////////////////////
	//各種パラメータを取得
	//法線を計算
	float3 normal = GetNormal(psIn.normal,psIn.tangent,psIn.biNormal,psIn.uv);

	//アルベドカラー、スペキュラカラー、金属度、滑さをサンプル
	//アルベドカラー
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	//float alpha = g_albedo.Sample(g_sampler, psIn.uv).a;

	//スペキュラをアルベドカラーと同じにする
	float3 specColor = albedoColor;
	//金属度はメタリックスムースのr成分(r)
	float metallic = g_metallicSmooth.Sample(g_sampler,psIn.uv).r;
	//滑らかさメタリックスムースのa成分(a)
	float smooth = g_metallicSmooth.Sample(g_sampler, psIn.uv).a;

	//サーフェスから視点へのベクトルを計算し、正規化
	float3 toEye = eyePos - psIn.worldPos;
	toEye = normalize(toEye);

	//光の強さ
	float3 lig = float3(0.0f,0.0f,0.0f);
	///////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////
	//ディズニーベースの拡散反射を求める
	
	//ディレクションライト
	//リムの強さを求める///////////////////////////////

	//ディレクションライトによるリムライト
	//サーフェスの法線とディレクションライトの入射方向に依存するリムの強さを求める
	//max関数:受け取った引数のうち、値が大きい方を返す(if文より速いことがある)
	float power1 = 1.0f - max(0.0f, dot(directionLight.direction, psIn.normal));
	//サーフェスの法線と視線の方向に依存するリムの強さを求める
	float power2 = 1.0f - max(0.0f, psIn.normalInView.z * -1.0f);
	//最終的なリムの強さを求める
	float limPower = power1 * power2;
	limPower = pow(limPower, 6.0f); 
	//ディレクションライトにリムライトの反射光を合算
	float3 limColorDir = limPower * directionLight.color;

	
	//フレネル反射を考慮した拡散反射を計算
	//ディレクションライト
	float diffuseFromFresnelDir = CalcDiffuseFromFresnel(
		normal,
		-directionLight.direction,
		toEye
	);

	//正規化ランバート拡散反射を求める
	//サーフェスの法線とライトの向きとの内積を求め、0～1の範囲に設定
	float NdotLDir = saturate(dot(normal,-directionLight.direction));
	//PIで除算し、正規化する
	float3 lambertDiffuseDir = directionLight.color * NdotLDir / PI;
	//最終的な拡散反射光を計算(アルベドカラー×フレネル反射×ランバート拡散反射)
	float3 diffuseDir = albedoColor * diffuseFromFresnelDir * lambertDiffuseDir;

	//ポイントライト
	//ポイントライトの影響率を計算
	float distancePt = length(psIn.worldPos - pointLight.position);
	//距離に応じた影響度になるよう計算
	float affectPt = 1.0f - 1.0f / pointLight.range * distancePt;
	//影響度が負の数にならないように補正
	if (affectPt < 0.0f) {
		affectPt = 0.0f;
	}
	//指数関数的な補正
	affectPt = pow(affectPt, 3.0f);

	//ポイントライトからサーフェスへのベクトルを計算
	float3 ptLigDir = normalize(psIn.worldPos - pointLight.position);
	
	//フレネル反射を考慮した拡散反射を計算
	float diffuseFromFresnelPt = CalcDiffuseFromFresnel(
		normal,
		-ptLigDir,
		toEye
	);

	//正規化ランバート拡散反射を求める
	//サーフェスの法線とライトの向きとの内積を求め、0～1の範囲に設定
	float NdotLPt = saturate(dot(normal, -ptLigDir));
	//PIで除算し、正規化する
	float3 lambertDiffusePt = pointLight.color * NdotLPt / PI;
	//最終的な拡散反射光を計算(アルベドカラー×フレネル反射×ランバート拡散反射)
	float3 diffusePt = albedoColor * diffuseFromFresnelPt * lambertDiffusePt;
	//距離による減衰を行う
	diffusePt *= affectPt;


	//float3 allDiffuse = diffuseDir + diffusePt;
	///////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////
	//クックトランスモデルを利用した鏡面反射率を求める
	//クックトランスモデルの鏡面反射率を計算
	//ディレクションライト
	float3 specDir = CookTorranceSpecular(
		-directionLight.direction,
		toEye,
		normal,
		smooth
	) * directionLight.color;

	//金属度が高い　→　スペキュラカラー(=アルベドカラー = 物体の色)を返す
	//金属度が低い　→　ディレクションライトの色を返す
	//specDir *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metallic);
	specDir *= lerp(directionLight.color,specColor,metallic);

	//ポイントライト
	float3 specPt = CookTorranceSpecular(
		-ptLigDir,
		toEye,
		normal,
		smooth
	) * pointLight.color;

	//金属度が高い　→　スペキュラカラー(=アルベドカラー = 物体の色)を返す
	//金属度が低い　→　ポイントライトの色を返す
	//specPt *= lerp(float3(1.0f,1.0f,1.0f),specColor,metallic);
	specPt *= lerp(pointLight.color,specColor,metallic);

	//距離による減衰を行う
	specPt *= affectPt;

	//滑らかさを使い、拡散反射光と鏡面反射光の合成を行う。滑らかさが高いほど、拡散反射光が弱くなる
	//ディレクションライト
	lig += diffuseDir*(1.0f - smooth) + specDir;
	//ポイントライト
	lig += diffusePt*(1.0f - smooth) + specPt;
	///////////////////////////////////////////////////////////////////////////////////////

	//環境光による底上げ
	lig += ambientLig * albedoColor;
	//リムライトを加算
	lig += limColorDir;

	//最終的なカラーの決定
	float4 finalColor = float4(1.0f, 1.0f, 1.0f, 1.0);
	finalColor.xyz = lig;
	return finalColor;
}

float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

	float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;
	
	return newNormal;
}


/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	// ピクセルの法線とライトの方向の内積を計算する
	float t = dot(normal, lightDirection) * -1.0f;
	//float t = saturate(dot(normal, -lightDirection));

	// 内積の値を0以上の値にする
	t = max(0.0f, t);

	// 拡散反射光を計算する
	return lightColor * t / PI;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
	// 反射ベクトルを求める
	float3 refVec = reflect(lightDirection, normal);

	// 光が当たったサーフェイスから視点に伸びるベクトルを求める
	float3 toEye = eyePos - worldPos;
	toEye = normalize(toEye);

	// 鏡面反射の強さを求める
	float t = dot(refVec, toEye);

	// 鏡面反射の強さを0以上の数値にする
	t = max(0.0f, t);

	// 鏡面反射の強さを絞る
	t = pow(t, 5.0f);

	// 鏡面反射光を求める
	return lightColor * t;
}

//ベックマン分布を計算する
float Beckmann(float m, float t)
{
	float t2 = t * t;
	float t4 = t * t * t * t;
	float m2 = m * m;
	float D = 1.0f / (4.0f * m2 * t4);
	D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
	return D;
}

//フレネルを計算
float SpcFresnel(float f0, float u)
{
	return f0 + (1 - f0) * pow(1 - u, 5);
}

//正規化ランバート拡散反射を求める
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
	//ディズニーベースのフレネル反射
	//光源に向かうベクトルと視線に向かうベクトルのハーフベクトル
	float3 H = normalize(L + V);

	//粗さは0.5で固定
	float roughness = 0.5f;

	//EA DISEによる改良コード
	float energyBias = lerp(0.0f, 0.5f, roughness);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);

	//光源に向かうベクトルとハーフベクトルの相似度を内積で求める
	float dotLH = saturate(dot(L, H));
	
	//光源に向かうベクトルとハーフベクトル
	//光が平行に入射した時の拡散反射量を求める
	float Fd90 = energyBias + 2.0f * dotLH * dotLH * roughness;

	//法線と光源に向かうベクトルwを利用して拡散反射率を求める
	float dotNL = saturate(dot(N, L));
	float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

	//法線と視点に向かうベクトルwを利用して拡散反射率を求める
	float dotNV = saturate(dot(N, V));
	float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

	//法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
	//乗算して最終的な拡散反射率を求めている。
	return (FL * FV * energyFactor);

}

//CookTorranceモデルの鏡面反射を計算
float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic)
{
	float microfacet = 0.76f;

	//金属度を垂直入射時のフレネル反射率として扱う
	//金属度が高いほどフレネル反射は大きくなる
	float f0 = metallic;

	//サーフェスからライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
	float3 H = normalize(L + V);

	//各種ベクトルの相似性を内積から求める
	float NdotH = saturate(dot(N, H));
	float VdotH = saturate(dot(V, H));
	float NdotL = saturate(dot(N, L));
	float NdotV = saturate(dot(N, V));

	//D項をベックマン分布を使用して計算
	float D = Beckmann(microfacet,NdotH);

	//F項をSchlick近似を用いて計算する
	float F = SpcFresnel(f0, VdotH);

	//G項を求める
	float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

	//m項を求める
	float m = PI * NdotV * NdotH;

	
	//計算した各項の数値を用いてCook-Torranceモデルの鏡面反射を求める
	return max(F * D * G / m, 0.0f);

}
