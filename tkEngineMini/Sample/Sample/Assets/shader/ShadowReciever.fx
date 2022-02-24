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

//ライトビュープロジェクション行列にアクセスする定数バッファ
cbuffer ShadowCb : register(b4) {
	float4x4 mLVP;
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
	float3 normalInView : TEXCOORD2;	//カメラ空間の法線
	float4 posInLVP		: TEXCOORD3;	//ライトビュースクリーン空間でのピクセルの座標
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);					//アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);		//ボーン行列。
Texture2D<float4> g_shadowMap : register(t10);				//シャドウマップ
sampler g_sampler : register(s0);							//サンプラステート。

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



	/*psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = vsIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);*/

	//通常の座標変換
	psIn.worldPos = vsIn.pos;
	float4 worldPos = mul(m, vsIn.pos);
	psIn.pos = mul(mView, worldPos);
	psIn.pos = mul(mProj, psIn.pos);

	//ライトビュープロジェクション空間の座標を計算する
	psIn.posInLVP = mul(mLVP, worldPos);

	//頂点法線をピクセルシェーダに渡す
	//psIn.normal = mul(mWorld, vsIn.normal);		//法線を回転させる
	psIn.normal = mul(m, vsIn.normal);		//法線を回転させる
	psIn.normal = normalize(psIn.normal);
	psIn.uv = vsIn.uv;

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
	//拡散反射光を求める///////////////////////////////

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

	//スポットライトの拡散反射光を求める
	//サーフェスに入射するスポットライトの光の向きを計算
	float3 spLigDir = psIn.worldPos - spotLight.position;
	//正規化
	spLigDir = normalize(spLigDir);
	//拡散反射光を計算
	float3 diffSpot = CalcLambertDiffuse(
		spLigDir,
		spotLight.color,
		psIn.normal
	);
	
	//鏡面反射光を求める///////////////////////////////
	
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
	float distancePt = length(psIn.worldPos - pointLight.position);
	//距離に応じた影響度になるよう計算
	float affectPt = 1.0f - 1.0f / pointLight.range * distancePt;
	//影響度が負の数にならないように補正
	if (affectPt < 0.0f) {
		affectPt = 0.0f;
	}
	//指数関数的な補正
	affectPt = pow(affectPt, 3.0f);

	//ポイントライトの拡散反射光、鏡面反射光に影響率を乗算して弱める
	diffPoint *= affectPt;
	specPoint *= affectPt;

	//スポットライトの鏡面反射光を求める
	float3 specSpot = CalcPhongSpecular(
		spLigDir,
		spotLight.color,
		psIn.worldPos,
		psIn.normal
	);

	//スポットライトの距離による影響率を計算
	float3 distanceSp = length(psIn.worldPos - spotLight.position);
	float affectSp = 1.0f - 1.0f / spotLight.range * distanceSp;
	//影響率が0を下回ったら0に補正する
	if (affectSp < 0.0f) {
		affectSp = 0.0f;
	}
	//影響のしかたを指数関数的にする
	affectSp = pow(affectSp, 3.0f);
	
	//スポットライトに影響率を乗算し、影響を弱める(1回目)
	diffSpot *= affectSp;
	specSpot *= affectSp;

	//スポットライトの入射光と射出方向の角度を求める
	float angleSp = dot(spLigDir, spotLight.direction);
	//angleSp = acos(angleSp);
	if (-1 < angleSp && angleSp < 1)
	{
		angleSp = acos(angleSp);
	}
	else if (angleSp > 0.9)
	{
		angleSp = 0;
	}
	else
	{
		angleSp = acos(-1.0f);
	}

	//角度による影響率を求める
	affectSp = 1.0f - 1.0f / spotLight.angle * angleSp;
	//0を下回る場合0に補正
	if (affectSp < 0.0f) {
		affectSp = 0.0f;
	}
	//影響の仕方を指数関数的にする
	affectSp = pow(affectSp, 0.5f);
	//スポットライトに影響率を乗算し、影響を弱める(2回目)
	diffSpot *= affectSp;
	specSpot *= affectSp;

	//リムの強さを求める///////////////////////////////
	
	//ディレクションライトによるリムライト
	//サーフェスの法線とディレクションライトの入射方向に依存するリムの強さを求める
	//max関数:受け取った引数のうち、値が大きい方を返す(if文より速いことがある)
	float power1 = 1.0f - max(0.0f, dot(directionLight.direction, psIn.normal));
	//サーフェスの法線と視線の方向に依存するリムの強さを求める
	float power2 = 1.0f - max(0.0f, psIn.normalInView.z * -1.0f);
	//最終的なリムの強さを求める
	float limPower = power1 * power2;
	limPower = pow(limPower, 1.3f);

	//ディレクションライトにリムライトの反射光を合算
	float3 limColorDir = limPower * directionLight.color;


	//最終カラーの決定
	
	//それぞれの拡散反射光を足す
	float3 diffuseLig = diffDirection + diffPoint + diffSpot;
	//それぞれの鏡面反射光を足す
	float3 specularLig = specDirection + specPoint + specSpot;
	//拡散反射光と鏡面反射光、環境光、リムライトを足す
	float3 lig = diffuseLig + specularLig + ambientLig + limColorDir;

	//アルベドカラーをサンプルして最終出力カラーのベースを作成
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	//影を描く処理
	
	//ライトビュースクリーン空間　→　UV空間　への座標変換
	//xyの値をwで除算することによって -1～1の範囲に収まる(=正規化スクリーン座標系への変換)
	float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
	//正規化スクリーン座標系(-1～1)の範囲に0.5(yは反転させるために-0.5)を掛け、-0.5～0.5の範囲にする
	shadowMapUV *= float2(0.5f, -0.5f);
	//それぞれに0.5を加算し、0～1の範囲にする
	shadowMapUV += 0.5f;

	//ライトビュースクリーン空間でのz値を計算
	float zInLVP = psIn.posInLVP.z / psIn.posInLVP.w;

	if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
		&& shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f) 
	{
		//シャドウマップに描き込まれたz値(深度値)と比較
		//シャドウマップに描かれているのはR成分だけなのでr成分をサンプルする
		float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;

		//遮蔽されているかどうかの判定
		//ライトビュースクリーン空間でのz値がシャドウマップに描かれたz値より大きい場合
		if (zInLVP > zInShadowMap) {
			//遮蔽されているので影を描画する
			albedoColor.xyz *= 0.5f;
		}
	}
	
	//最終出力カラーにライトを乗算して出力カラーを決める
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

