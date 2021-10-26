#include "stdafx.h"
#include "RenderingEngine.h"
#include "GraphicsEngine.h"
#include "RenderTarget.h"
#include "RenderContext.h"

RenderingEngine* RenderingEngine::m_renderingEngine = nullptr;

void RenderingEngine::Init()
{
	//ライトカメラの初期化
	InitLightCamera();
	//メインレンダリングターゲットの初期化
	InitMainRenderTarget();
	//シャドウマップの初期化
	InitShadowMap();
	
	
	//ブルームの初期化
	InitBloom(m_mainRenderTarget);
}

void RenderingEngine::Execute(RenderContext& rc)
{
	//シャドウマップへの描画
	RenderToShadowMap(rc, m_lightCamera);

	//ブルーム処理
	BloomRendering(rc, m_mainRenderTarget);

}

void RenderingEngine::CommonRender(RenderContext& rc)
{
	//モデルをドロー
	for (auto& model : m_commonModels) {
		model->Draw(rc);
	}

	//レンダリングターゲットの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}

void RenderingEngine::RenderToShadowMap(RenderContext& rc , Camera camera)
{
	//シャドウマップに描き込み
	m_shadowMap.Render(rc, camera);

}

void RenderingEngine::BloomRendering(RenderContext& rc, RenderTarget& mainRT)
{
	//ブルーム処理
	m_bloom.Render(rc, mainRT);
}

void RenderingEngine::InitLightCamera()
{
	m_lightCamera.SetPosition(0.0f, 2000.0f, 0.0f);
	m_lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	m_lightCamera.SetViewAngle(Math::DegToRad(60.0f));
	m_lightCamera.Update();
}

void RenderingEngine::InitMainRenderTarget()
{
	m_mainRenderTarget.Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
}

void RenderingEngine::InitShadowMap()
{
	m_shadowMap.Init();
}

void RenderingEngine::InitBloom(RenderTarget& mainRT)
{
	m_bloom.Init(mainRT);
}

void RenderingEngine::DeleteCommonModel(Model& model)
{
	//イテレータを作成
	std::vector<Model*>::iterator itr;
	
	//モデルを検索
	itr = std::find(
		m_commonModels.begin(),
		m_commonModels.end(),
		&model);

	//モデルが見つかったら削除
	if (itr != m_commonModels.end()) {
		m_commonModels.erase(itr);
	}
}

