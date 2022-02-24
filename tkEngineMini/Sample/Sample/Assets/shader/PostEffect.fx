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

//変数
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

//ボケ画像にアクセスするための変数
Texture2D<float4> g_bokeTexture_0 : register(t0);
Texture2D<float4> g_bokeTexture_1 : register(t1);
Texture2D<float4> g_bokeTexture_2 : register(t2);
Texture2D<float4> g_bokeTexture_3 : register(t3);

//ブラー画像をサンプルして合成し、平均を出力するピクセルシェーダ
float4 PSBloomFinal(PSInput In) : SV_Target0
{
	//ブラー画像をサンプリングして、平均を取って出力
	float4 combineColor = g_bokeTexture_0.Sample(Sampler,In.uv);
	combineColor += g_bokeTexture_1.Sample(Sampler, In.uv);
	combineColor += g_bokeTexture_2.Sample(Sampler, In.uv);
	combineColor += g_bokeTexture_3.Sample(Sampler, In.uv);
	//平均を計算
	combineColor /= 4.0f;
	combineColor.a = 1.0f;

	return combineColor;
}

