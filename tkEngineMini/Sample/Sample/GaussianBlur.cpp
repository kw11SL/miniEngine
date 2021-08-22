#include "stdafx.h"
#include "GaussianBlur.h"

namespace
{
	//ガウシアンブラー用シェーダのファイルパス
	const char* GAUSSIANBLUR_SHADER_FILEPATH = "Assets/shader/GaussianBlur.fx";
	//横ブラー用シェーダの頂点シェーダエントリーポイント名
	const char* XBLUR_VS_ENTRY_POINT_NAME = "VSXBlur";
	//縦ブラー用シェーダの頂点シェーダエントリーポイント名
	const char* YBLUR_VS_ENTRY_POINT_NAME = "VSYBlur";
	//ガウシアンブラー用シェーダのピクセルシェーダエントリーポイント名
	const char* GAUSSIANBLUR_PS_ENTRY_POINT_NAME = "PSBlur";
}


void GaussianBlur::Init(Texture* baseTexture)
{
	m_baseTexture = baseTexture;

	//レンダリングターゲットを初期化
	InitRenderTargets();
	
	//スプライトを初期化
	InitSprites();
}

void GaussianBlur::ExecuteOnGPU(RenderContext& rc, float blurPower)
{
	//重みテーブルの更新
	UpdateWeightsTable(blurPower);

	//横ブラーの実行
	//レンダリングターゲットとして利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(m_xBlurRenderTarget);
	//レンダリングターゲットの設定
	rc.SetRenderTargetAndViewport(m_xBlurRenderTarget);
	//レンダリングターゲットのクリア
	rc.ClearRenderTargetView(m_xBlurRenderTarget);
	//ドロー
	m_xBlurSprite.Draw(rc);
	//レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_xBlurRenderTarget);

	//縦ブラーの実行
	//レンダリングターゲットとして利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(m_yBlurRenderTarget);
	//レンダリングターゲットの設定
	rc.SetRenderTargetAndViewport(m_yBlurRenderTarget);
	//レンダリングターゲットのクリア
	rc.ClearRenderTargetView(m_yBlurRenderTarget);
	//ドロー
	m_yBlurSprite.Draw(rc);
	//レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_yBlurRenderTarget);

}

void GaussianBlur::InitRenderTargets()
{
	//横ブラー用レンダリングターゲットを作成
	m_xBlurRenderTarget.Create(
		//横幅は元になるテクスチャの半分
		m_baseTexture->GetWidth() / 2,
		//高さはそのまま
		m_baseTexture->GetHeight(),
		1,
		1,
		//フォーマットは元になるテクスチャと同じ
		m_baseTexture->GetFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

	//縦ブラー用レンダリングターゲットを作成
	m_yBlurRenderTarget.Create(
		//横幅は元になるテクスチャの半分
		m_baseTexture->GetWidth() / 2,
		//高さも半分
		m_baseTexture->GetHeight() / 2,
		1,
		1,
		//フォーマットは元になるテクスチャと同じ
		m_baseTexture->GetFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

}

void GaussianBlur::InitSprites()
{
	//横ブラー用のスプライトを初期化
	{
		//横ブラー用初期化データを作成
		SpriteInitData xBlurSpriteInitData;
		xBlurSpriteInitData.m_fxFilePath = GAUSSIANBLUR_SHADER_FILEPATH;
		xBlurSpriteInitData.m_vsEntryPointFunc = XBLUR_VS_ENTRY_POINT_NAME;
		xBlurSpriteInitData.m_psEntryPoinFunc = GAUSSIANBLUR_PS_ENTRY_POINT_NAME;
		//スプライトの解像度は横ブラー用レンダリングターゲットと同じ
		xBlurSpriteInitData.m_width = m_xBlurRenderTarget.GetWidth();
		//高さも同様
		xBlurSpriteInitData.m_height = m_xBlurRenderTarget.GetHeight();
		//テクスチャはベースのテクスチャ
		xBlurSpriteInitData.m_textures[0] = m_baseTexture;
		//書き込むレンダリングターゲットのフォーマットを指定する
		xBlurSpriteInitData.m_colorBufferFormat[0] = m_xBlurRenderTarget.GetColorBufferFormat();
		//定数バッファにブラー用パラメータを設定
		//ブラーの重みテーブルを設定
		xBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
		xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

		//初期化データを基に横ブラー用スプライトを初期化
		m_xBlurSprite.Init(xBlurSpriteInitData);
	}

	//縦ブラー用のスプライトを初期化
	{
		//縦ブラー用初期化データを作成
		SpriteInitData yBlurSpriteInitData;
		yBlurSpriteInitData.m_fxFilePath = GAUSSIANBLUR_SHADER_FILEPATH;
		yBlurSpriteInitData.m_vsEntryPointFunc = YBLUR_VS_ENTRY_POINT_NAME;
		yBlurSpriteInitData.m_psEntryPoinFunc = GAUSSIANBLUR_PS_ENTRY_POINT_NAME;
		//スプライトの解像度は縦ブラー用レンダリングターゲットと同じ
		yBlurSpriteInitData.m_width = m_yBlurRenderTarget.GetWidth();
		//高さも同様
		yBlurSpriteInitData.m_height = m_yBlurRenderTarget.GetHeight();
		//テクスチャは横ブラーを掛けたもの
		yBlurSpriteInitData.m_textures[0] = &m_xBlurRenderTarget.GetRenderTargetTexture();
		//書き込むレンダリングターゲットのフォーマットを指定する
		yBlurSpriteInitData.m_colorBufferFormat[0] = m_yBlurRenderTarget.GetColorBufferFormat();
		//定数バッファにブラー用パラメータを設定
		//ブラーの重みテーブルを設定
		yBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
		yBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

		//初期化データを基に横ブラー用スプライトを初期化
		m_yBlurSprite.Init(yBlurSpriteInitData);
	}
}

void GaussianBlur::UpdateWeightsTable(float blurPower)
{
	//重みの合計を記録する変数
	float totalWeight = 0;

	//ガウス関数を用いて重みテーブルの各重みを計算
	//ループ変数numFarは基準テクセルからの距離
	for (int numFar = 0; numFar < NUM_WEIGHTS; numFar++) {
		m_weights[numFar] = expf(-0.5f * (float)(numFar * numFar) / blurPower);
		totalWeight += 2.0f * m_weights[numFar];
	}

	//重みの合計で除算し、重みの合計値を1にする
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_weights[i] /= totalWeight;
	}

}