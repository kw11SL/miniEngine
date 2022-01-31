#include "stdafx.h"
#include "ShadowMapRender.h"

namespace 
{
	const int SHADOWMAP_WIDTH = 1024;
	const int SHADOWMAP_HEIGHT = 1024;
}

void ShadowMapRender::Init()
{
	//シャドウマップを初期化
	InitShadowMap();

}

void ShadowMapRender::InitShadowMap()
{
	//クリアカラー
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	m_shadowMap.Create(
		SHADOWMAP_WIDTH,
		SHADOWMAP_HEIGHT,
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

}

//void ShadowMapRender::InitMultiShadowMap()
//{
//	//クリアカラー
//	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
//	
//	//近景用シャドウマップ
//	m_shadowMaps[0].Create(
//		2048,
//		2048,
//		1,
//		1,
//		DXGI_FORMAT_R32_FLOAT,
//		DXGI_FORMAT_D32_FLOAT,
//		clearColor
//	);
//
//	//中景用シャドウマップ
//	m_shadowMaps[0].Create(
//		1024,
//		1024,
//		1,
//		1,
//		DXGI_FORMAT_R32_FLOAT,
//		DXGI_FORMAT_D32_FLOAT,
//		clearColor
//	);
//
//	//遠景用シャドウマップ
//	m_shadowMaps[0].Create(
//		512,
//		512,
//		1,
//		1,
//		DXGI_FORMAT_R32_FLOAT,
//		DXGI_FORMAT_D32_FLOAT,
//		clearColor
//	);
//
//}

void ShadowMapRender::Render(RenderContext& rc, Camera& lightCamera)
{
	//レンダリングターゲットが使用可能になるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	rc.SetRenderTargetAndViewport(m_shadowMap);
	rc.ClearRenderTargetView(m_shadowMap);

	//モデルをシャドウマップに描画
	for (auto& model : m_modelsArray) {
		if (model != nullptr) {
			model->Draw(rc , lightCamera);
		}
	}

	//描き込み完了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
}

void ShadowMapRender::DeleteModel(Model* model)
{
	//イテレータを作成
	std::vector<Model*>::iterator itr;

	//モデル配列を検索
	itr = std::find(
		m_modelsArray.begin(),
		m_modelsArray.end(),
		model
	);

	//見つかったら削除
	if (itr != m_modelsArray.end()) {
		m_modelsArray.erase(itr);
	}
}

//void ShadowMapRender::MultiRender(RenderContext& rc , Camera& lightCamera)
//{
//
//	int shadowMapNo = 0;
//	
//	//レンダリングターゲットの設定
//	for (auto& shadowMap : m_shadowMaps) {
//		rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
//		rc.SetRenderTargetAndViewport(shadowMap);
//		rc.ClearRenderTargetView(shadowMap);
//	
//		//モデルを描画
//		for (auto& model : m_models[shadowMapNo]) {
//			model->Draw(
//				rc,
//				lightCamera
//			);
//		}
//
//		//描画終了でクリア
//		m_models[shadowMapNo].clear();
//		//書き込み終了待ち
//		rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
//	
//		//次のモデルへ
//		shadowMapNo++;
//	}
//
//
//}