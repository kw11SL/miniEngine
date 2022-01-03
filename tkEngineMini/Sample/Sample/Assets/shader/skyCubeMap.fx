//#include "ModelVSCommon.h"

//////////////////////////////
//構造体
//////////////////////////////

// 頂点シェーダーへの入力
struct SVSIn
{
	float4 pos : POSITION;          //頂点座標。
	float3 normal : NORMAL;         //法線。
	float2 uv : TEXCOORD0;          //UV座標。
	float3 tangent  : TANGENT;      //接ベクトル。
	float3 biNormal : BINORMAL;     //従ベクトル。
	int4  Indices  	: BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};

//ピクセルシェーダへの入力
struct SPSIn
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 biNormal : BINORMAL;
	float2 uv : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
};

//////////////////////////////
//定数バッファ
//////////////////////////////

// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//スカイキューブの明るさ
cbuffer SkyCubeCb : register(b1)
{
	float luminance;
};

//////////////////////////////
//シェーダリソース
//////////////////////////////
StructuredBuffer<float4x4> g_boneMatrix         : register(t3);	    //ボーン行列。
StructuredBuffer<float4x4> g_worldMatrixArray   : register(t10);	//ワールド行列の配列。インスタンシング描画の際に有効。

Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normal : register(t1);				//法線マップ
Texture2D<float4> g_specular : register(t2);			//スペキュラマップ
TextureCube<float4> g_skyCubeMap : register(t10);

//////////////////////////////
//サンプラステート
//////////////////////////////
sampler g_sampler : register(s0);

//////////////////////////////
//関数定義
//////////////////////////////

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SVSIn skinVert)
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

// 法線マップから法線を取得。
float3 GetNormalFromNormalMap(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normal.SampleLevel(g_sampler, uv, 0.0f).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

	float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

	return newNormal;
}

// モデル用の頂点シェーダーのエントリーポイント
//コア関数
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if (hasSkin) {
		m = CalcSkinMatrix(vsIn);
	}
	else {
		m = mWorld;
	}

	psIn.pos = mul(m, vsIn.pos); // モデルの頂点をワールド座標系に変換
	// 頂点シェーダーからワールド座標を出力
	psIn.worldPos = psIn.pos;

	psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
	psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
	psIn.normal = normalize(mul(m, vsIn.normal));
	psIn.tangent = normalize(mul(m, vsIn.tangent));
	psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	psIn.uv = vsIn.uv;

	return psIn;
}

//スキンなしモデルの頂点シェーダ
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}

//スキンありモデルの頂点シェーダ
//SPSIn VSMainSkin(SVSIn vsIn)
SPSIn VSSkinMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}

// ピクセルシェーダーのエントリー関数。
float4 PSMain(SPSIn psIn) : SV_Target0
{
	float4 albedoColor;
	float3 normal = normalize(psIn.normal);
	//albedoColor = g_skyCubeMap.Sample(g_sampler, psIn.normal);
	albedoColor = g_skyCubeMap.Sample(g_sampler, normal * -1.0f) * luminance;
	return albedoColor;
}