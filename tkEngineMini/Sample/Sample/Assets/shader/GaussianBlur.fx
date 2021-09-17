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

Texture2D<float4> colorTexture : register(t0);	// メインレンダリングターゲットのテクスチャ
sampler Sampler : register(s0);					// サンプラ

//ブラー
struct PS_BlurInput
{
	float4 pos : SV_POSITION;
	float4 tex0 : TEXCOORD0;
	float4 tex1 : TEXCOORD1;
	float4 tex2 : TEXCOORD2;
	float4 tex3 : TEXCOORD3;
	float4 tex4 : TEXCOORD4;
	float4 tex5 : TEXCOORD5;
	float4 tex6 : TEXCOORD6;
	float4 tex7 : TEXCOORD7;
};

Texture2D<float4> sceneTexture : register(t0);		//シーンテクスチャ

//ブラー用定数バッファ
cbuffer CBBlur : register(b1)
{
	float4 weight[2]; //重み
}

//横ブラー頂点シェーダ
PS_BlurInput VSXBlur(VSInput In)
{
	PS_BlurInput Out;

	//座標を変換
	Out.pos = mul(mvp, In.pos);

	//テクスチャサイズを取得
	float2 texSize;					//サイズ格納用
	float level;
	sceneTexture.GetDimensions(0, texSize.x, texSize.y, level);

	//基準テクセルのUV座標を記録
	float2 tex = In.uv;

	//基準テクセルからU座標を+1テクセルずらすためのオフセットを計算
	Out.tex0.xy = float2(1.0f / texSize.x, 0.0f);
	//+3
	Out.tex1.xy = float2(3.0f / texSize.x, 0.0f);
	//+5
	Out.tex2.xy = float2(5.0f / texSize.x, 0.0f);
	//+7
	Out.tex3.xy = float2(7.0f / texSize.x, 0.0f);
	//+9
	Out.tex4.xy = float2(9.0f / texSize.x, 0.0f);
	//+11
	Out.tex5.xy = float2(11.0f / texSize.x, 0.0f);
	//+13
	Out.tex6.xy = float2(13.0f / texSize.x, 0.0f);
	//+15
	Out.tex7.xy = float2(15.0f / texSize.x, 0.0f);

	//オフセット量に-1.0を掛けてマイナス方向のオフセットも計算する
	Out.tex0.zw = Out.tex0.xy * -1.0f;
	Out.tex1.zw = Out.tex1.xy * -1.0f;
	Out.tex2.zw = Out.tex2.xy * -1.0f;
	Out.tex3.zw = Out.tex3.xy * -1.0f;
	Out.tex4.zw = Out.tex4.xy * -1.0f;
	Out.tex5.zw = Out.tex5.xy * -1.0f;
	Out.tex6.zw = Out.tex6.xy * -1.0f;
	Out.tex7.zw = Out.tex7.xy * -1.0f;

	//オフセットに基準テクセルのUV座標を足し、サンプリングするUV座標に変換する
	Out.tex0 += float4(tex, tex);
	Out.tex1 += float4(tex, tex);
	Out.tex2 += float4(tex, tex);
	Out.tex3 += float4(tex, tex);
	Out.tex4 += float4(tex, tex);
	Out.tex5 += float4(tex, tex);
	Out.tex6 += float4(tex, tex);
	Out.tex7 += float4(tex, tex);

	return Out;
}

//縦ブラー用頂点シェーダ
PS_BlurInput VSYBlur(VSInput In)
{
	PS_BlurInput Out;

	//座標を変換
	Out.pos = mul(mvp, In.pos);

	//テクスチャサイズを取得
	float2 texSize;					//サイズ格納用
	float level;
	sceneTexture.GetDimensions(0, texSize.x, texSize.y, level);

	//基準テクセルのUV座標を記録
	float2 tex = In.uv;

	//基準テクセルからV座標を+1テクセルずらすためのオフセットを計算
	Out.tex0.xy = float2(0.0f, 1.0f / texSize.y);
	//+3
	Out.tex1.xy = float2(0.0f, 3.0f / texSize.y);
	//+5
	Out.tex2.xy = float2(0.0f, 5.0f / texSize.y);
	//+7
	Out.tex3.xy = float2(0.0f, 7.0f / texSize.y);
	//+9
	Out.tex4.xy = float2(0.0f, 9.0f / texSize.y);
	//+11
	Out.tex5.xy = float2(0.0f, 11.0f / texSize.y);
	//+13
	Out.tex6.xy = float2(0.0f, 13.0f / texSize.y);
	//+15
	Out.tex7.xy = float2(0.0f, 15.0f / texSize.y);

	//オフセット量に-1.0を掛けてマイナス方向のオフセットも計算する
	Out.tex0.zw = Out.tex0.xy * -1.0f;
	Out.tex1.zw = Out.tex1.xy * -1.0f;
	Out.tex2.zw = Out.tex2.xy * -1.0f;
	Out.tex3.zw = Out.tex3.xy * -1.0f;
	Out.tex4.zw = Out.tex4.xy * -1.0f;
	Out.tex5.zw = Out.tex5.xy * -1.0f;
	Out.tex6.zw = Out.tex6.xy * -1.0f;
	Out.tex7.zw = Out.tex7.xy * -1.0f;

	//オフセットに基準テクセルのUV座標を足し、サンプリングするUV座標に変換する
	Out.tex0 += float4(tex, tex);
	Out.tex1 += float4(tex, tex);
	Out.tex2 += float4(tex, tex);
	Out.tex3 += float4(tex, tex);
	Out.tex4 += float4(tex, tex);
	Out.tex5 += float4(tex, tex);
	Out.tex6 += float4(tex, tex);
	Out.tex7 += float4(tex, tex);

	return Out;
}


//ブラーピクセルシェーダ

float4 PSBlur(PS_BlurInput In) : SV_Target0
{
	float4 color;

	//基準テクセルからプラス方向に8テクセル、重み付きでサンプリング
	color  = weight[0].x * sceneTexture.Sample(Sampler, In.tex0.xy);
	color += weight[0].y * sceneTexture.Sample(Sampler, In.tex1.xy);
	color += weight[0].z * sceneTexture.Sample(Sampler, In.tex2.xy);
	color += weight[0].w * sceneTexture.Sample(Sampler, In.tex3.xy);
	color += weight[1].x * sceneTexture.Sample(Sampler, In.tex4.xy);
	color += weight[1].y * sceneTexture.Sample(Sampler, In.tex5.xy);
	color += weight[1].z * sceneTexture.Sample(Sampler, In.tex6.xy);
	color += weight[1].w * sceneTexture.Sample(Sampler, In.tex7.xy);

	//基準テクセルからマイナス方向に8テクセル、重み付きでサンプリング
	color += weight[0].x * sceneTexture.Sample(Sampler, In.tex0.zw);
	color += weight[0].y * sceneTexture.Sample(Sampler, In.tex1.zw);
	color += weight[0].z * sceneTexture.Sample(Sampler, In.tex2.zw);
	color += weight[0].w * sceneTexture.Sample(Sampler, In.tex3.zw);
	color += weight[1].x * sceneTexture.Sample(Sampler, In.tex4.zw);
	color += weight[1].y * sceneTexture.Sample(Sampler, In.tex5.zw);
	color += weight[1].z * sceneTexture.Sample(Sampler, In.tex6.zw);
	color += weight[1].w * sceneTexture.Sample(Sampler, In.tex7.zw);

	return float4(color.xyz, 1.0f);
}