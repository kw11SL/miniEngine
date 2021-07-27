/*!
 * @brief	シンプルなモデルシェーダー。
 */

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

//関数宣言
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);

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
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//頂点の法線
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;	//ワールド座標
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
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
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = vsIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);


	//頂点法線をピクセルシェーダに渡す
	psIn.normal = mul(mWorld, vsIn.normal);		//法線を回転させる
	psIn.uv = vsIn.uv;

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
	////ライトの影響度の計算
	////ピクセルの法線とライトの方向の内積を計算する
	//float t = dot(psIn.normal,directionLight.direction);
	////内積の結果に-1を掛ける
	//t *= -1.0f;
	////内積の結果が0より小さい場合、0にする
	//if (t < 0.0f) {
	//	t = 0.0f;
	//}

	////拡散反射光を求める
	//float3 diffuseLig = directionLight.color * t;

	//ディレクションライトの拡散反射光を求める
	float3 diffDirection = CalcLambertDiffuse(
		directionLight.direction,
		directionLight.color,
		psIn.normal
	);
	
	//ポイントライトの拡散反射光を求める
	//サーフェスに入射するポイントライトの向きを計算
	float3 ptLigDir = psIn.worldPos - pointLight.position;
	//正規化
	ptLigDir = normalize(ptLigDir);
	//拡散反射光を計算
	float3 diffPoint = CalcLambertDiffuse(
		ptLigDir,
		pointLight.color,
		psIn.normal
	);


	////サーフェスの反射
	////ライトの方向と頂点の法線情報から反射ベクトルを求める
	//float3 refVec = reflect(directionLight.direction, psIn.normal);
	////光の当たったサーフェスから視点に伸びるベクトルを求める
	//float3 toEye = eyePos - psIn.worldPos;
	////方向を正規化
	//toEye = normalize(toEye);
	////反射ベクトルと実際の反射ベクトルの内積により鏡面反射の強さを求める
	//t = dot(refVec, toEye);
	////内積が負の値なら0にする
	//if (t < 0.0f) {
	//	t = 0.0f;
	//}
	////指数関数的変化にして反射の強さを絞る
	//t = pow(t, 5.0f);
	////鏡面反射光を求める
	//float3 specularLig = directionLight.color * t;
	
	//ディレクションライトの鏡面反射光を求める
	float3 specDirection = CalcPhongSpecular(
		directionLight.direction,
		directionLight.color,
		psIn.worldPos,
		psIn.normal
	);
	
	//ポイントライトの鏡面反射光を求める
	float3 specPoint = CalcPhongSpecular(
		ptLigDir,
		pointLight.color,
		psIn.worldPos,
		psIn.normal
	);

	//ポイントライトの影響率を計算
	float distance = length(psIn.worldPos - pointLight.position);
	//距離に応じた影響度になるよう計算
	float affect = 1.0f - 1.0f / pointLight.range * distance;

	//影響度が負の数にならないように補正
	if (affect < 0.0f) {
		affect = 0.0f;
	}
	
	//指数関数的な補正
	affect = pow(affect, 3.0f);

	//ポイントライトの拡散反射光、鏡面反射光に影響率を乗算して弱める
	diffPoint *= affect;
	specPoint *= affect;

	
	//最終カラーの決定
	
	//それぞれの拡散反射光を足す
	float3 diffuseLig = diffDirection + diffPoint;
	//それぞれの鏡面反射光を足す
	float3 specularLig = specDirection + specPoint;
	//拡散反射光と鏡面反射光、環境光を足す
	float3 lig = diffuseLig + specularLig + ambientLig;

	////アンビエントライト(明るさを底上げ)
	//lig.x += ambientLig.x;
	//lig.y += ambientLig.y;
	//lig.z += ambientLig.z;

	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	
	//最終出力カラーに光を乗算して出力カラーを決める
	albedoColor.xyz *= lig;

	return albedoColor;
}

/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	// ピクセルの法線とライトの方向の内積を計算する
	float t = dot(normal, lightDirection) * -1.0f;

	// 内積の値を0以上の値にする
	t = max(0.0f, t);

	// 拡散反射光を計算する
	return lightColor * t;
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

