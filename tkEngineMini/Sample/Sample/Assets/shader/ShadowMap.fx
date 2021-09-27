/*!
 * @brief	シャドウマップ描画用シェーダ
 */


////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。

//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//頂点の法線
	float2 uv 		: TEXCOORD0;	//UV座標。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//頂点の法線
	float2 uv 			: TEXCOORD0;	//uv座標。
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_shadowMap : register(t10);			//シャドウマップ
sampler g_sampler : register(s0);						//サンプラステート。

/// <summary>
/// シャドウマップ描画用の頂点シェーダー
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	SPSIn psIn;
	psIn.pos = mul(mWorld,vsIn.pos);
	psIn.pos = mul(mView,psIn.pos);
	psIn.pos = mul(mProj,psIn.pos);
	psIn.uv = vsIn.uv;
	psIn.normal = mul(mWorld,vsIn.normal);
	
	return psIn;
}

/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//シャドウマップにz値を書き込む
	return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);
}



