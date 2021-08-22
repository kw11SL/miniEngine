/*!
 * @brief	ブルーム
 */

cbuffer cb : register(b0){
	float4x4 mvp;		// MVP行列
	float4 mulColor;	// 乗算カラー
};
struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

/*!
 * @brief 頂点シェーダー
 */
PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = mul(mvp, In.pos);
	psIn.uv = In.uv;
	return psIn;
}

Texture2D<float4> mainRenderTargetTexture : register(t0);	// メインレンダリングターゲットのテクスチャ
sampler Sampler : register(s0);					// サンプラ

//輝度抽出用ピクセルシェーダ
float4 PSSamplingLuminance(PSInput In) : SV_Target0
{
	//メインレンダリングターゲットからカラーをサンプリング
	float4 samplingColor = mainRenderTargetTexture.Sample(Sampler,In.uv);
	//サンプリングしたカラーの明るさを計算
	float luminance = dot(samplingColor.xyz, float3(0.2125f, 0.7154f, 0.0721f));
	//clip関数でカラーの明るさが1未満ならピクセルキルする
	//clip関数内の引数の値が0未満なら処理をスキップする
	//この場合1.0未満の際に処理がスキップされる(=ピクセルキル)
	clip(luminance - 1.0f);

	return samplingColor;
}

