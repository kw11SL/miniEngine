#include "stdafx.h"
#include "Bloom.h"

namespace
{
	const int WINDOW_WIDTH = 1280;	//幅
	const int WINDOW_HEIGHT = 720;	//高さ

	const char* POSTEFFECT_FILEPATH = "Assets/shader/PostEffect.fx";
	const char* GAUSSIAN_BLUR_EFFECT_FILEPATH = "Assets/shader/GaussianBlur.fx";
}

void Bloom::Init(RenderTarget& mainRenderTarget)
{
	//輝度抽出用レンダリングターゲットの設定
	m_luminanceRenderTarget.Create(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

	//輝度抽出用スプライトを初期化
	SpriteInitData luminanceSpriteInitData;
	luminanceSpriteInitData.m_fxFilePath = POSTEFFECT_FILEPATH;
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	luminanceSpriteInitData.m_width = mainRenderTarget.GetWidth();
	luminanceSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//テクスチャはメインレンダリングターゲット
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//書き込むレンダリングターゲットのフォーマットを指定
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//スプライトを初期化
	m_luminanceSprite.Init(luminanceSpriteInitData);

	//輝度抽出用レンダリングターゲットのテクスチャで初期化(スプライトの方ではない)
	m_gaussBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
	//ブラーを掛けたテクスチャにブラーを掛けていく
	m_gaussBlur[1].Init(&m_gaussBlur[0].GetExecutedTexture());
	m_gaussBlur[2].Init(&m_gaussBlur[1].GetExecutedTexture());
	m_gaussBlur[3].Init(&m_gaussBlur[2].GetExecutedTexture());

	//ブラー画像を加算合成するためのスプライトを初期化
	SpriteInitData addBrendSpriteInitData;
	//テクスチャはブラー処理をした輝度抽出テクスチャ
	addBrendSpriteInitData.m_textures[0] = &m_gaussBlur[0].GetExecutedTexture();
	addBrendSpriteInitData.m_textures[1] = &m_gaussBlur[1].GetExecutedTexture();
	addBrendSpriteInitData.m_textures[2] = &m_gaussBlur[2].GetExecutedTexture();
	addBrendSpriteInitData.m_textures[3] = &m_gaussBlur[3].GetExecutedTexture();

	//解像度はレンダリングターゲットと同じ
	addBrendSpriteInitData.m_width = mainRenderTarget.GetWidth();
	addBrendSpriteInitData.m_height = mainRenderTarget.GetHeight();

	//ブラー画像を合成するためにシェーダを専用のものに変更
	addBrendSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
	addBrendSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//ブルーム用のエントリーポイントを指定
	addBrendSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
	//アルファブレンドモードを加算合成にする
	addBrendSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	addBrendSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	
	m_finalSprite.Init(addBrendSpriteInitData);

}

void Bloom::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//輝度抽出処理
	//輝度抽出用レンダリングターゲットに変更
	rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
	//レンダリングターゲットを設定
	rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
	//レンダリングターゲットをクリア
	rc.ClearRenderTargetView(m_luminanceRenderTarget);
	//輝度抽出を行う
	m_luminanceSprite.Draw(rc);
	//レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

	//ガウシアンブラーを複数回実行する
	m_gaussBlur[0].ExecuteOnGPU(rc, 20.0f);
	m_gaussBlur[1].ExecuteOnGPU(rc, 20.0f);
	m_gaussBlur[2].ExecuteOnGPU(rc, 20.0f);
	m_gaussBlur[3].ExecuteOnGPU(rc, 20.0f);

	//ブラー画像をメインレンダリングターゲットに加算合成
	//レンダリングターゲットとして利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//レンダリングターゲットを設定
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	//合成(=スプライトの描画)
	m_finalSprite.Draw(rc);
	//レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

}