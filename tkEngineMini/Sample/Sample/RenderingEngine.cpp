#include "stdafx.h"
#include "RenderingEngine.h"
#include "GraphicsEngine.h"
#include "RenderTarget.h"
#include "RenderContext.h"

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
	//RenderToShadowMap(rc, m_lightCamera);

	//ブルーム
	BloomRendering(rc, m_mainRenderTarget);

}

void RenderingEngine::CommonRender(RenderContext& rc)
{
	//モデルをドロー
	for (auto& model : m_commonModels) {
		model->Draw(rc);
	}
}

void RenderingEngine::RenderToShadowMap(RenderContext& rc , Camera camera)
{
	m_shadowMap.Render(rc, camera);
}

void RenderingEngine::InitLightCamera()
{
	m_lightCamera.SetPosition(0, 600, 0);
	m_lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	m_lightCamera.SetViewAngle(Math::DegToRad(20.0f));
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

void RenderingEngine::BloomRendering(RenderContext& rc, RenderTarget& mainRT)
{
	m_bloom.Render(rc, mainRT);
}