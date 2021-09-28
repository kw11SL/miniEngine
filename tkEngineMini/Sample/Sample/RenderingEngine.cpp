#include "stdafx.h"
#include "RenderingEngine.h"
#include "GraphicsEngine.h"
#include "RenderTarget.h"
//#include "Bloom.h"
#include "RenderContext.h"

void RenderingEngine::Init()
{
	InitMainRenderTarget();
	InitBloom(m_mainRenderTarget);

}

void RenderingEngine::Execute(RenderContext& rc)
{
	BloomRendering(rc, m_mainRenderTarget);
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

void RenderingEngine::InitBloom(RenderTarget& mainRT)
{
	m_bloom.Init(mainRT);
}

void RenderingEngine::BloomRendering(RenderContext& rc, RenderTarget& mainRT)
{
	m_bloom.Render(rc, mainRT);
}