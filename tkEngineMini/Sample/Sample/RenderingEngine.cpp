#include "stdafx.h"
#include "RenderingEngine.h"
#include "GraphicsEngine.h"
#include "RenderTarget.h"
#include "RenderContext.h"

RenderingEngine* RenderingEngine::m_renderingEngine = nullptr;

void RenderingEngine::Init()
{
	//���C�g�J�����̏�����
	InitLightCamera();
	//���C�������_�����O�^�[�Q�b�g�̏�����
	InitMainRenderTarget();
	//�V���h�E�}�b�v�̏�����
	InitShadowMap();
	
	
	//�u���[���̏�����
	InitBloom(m_mainRenderTarget);
}

void RenderingEngine::Execute(RenderContext& rc)
{
	//�V���h�E�}�b�v�ւ̕`��
	RenderToShadowMap(rc, m_lightCamera);

	//�u���[������
	BloomRendering(rc, m_mainRenderTarget);

}

void RenderingEngine::CommonRender(RenderContext& rc)
{
	//���f�����h���[
	for (auto& model : m_commonModels) {
		model->Draw(rc);
	}

	//�����_�����O�^�[�Q�b�g�̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}

void RenderingEngine::RenderToShadowMap(RenderContext& rc , Camera camera)
{
	//�V���h�E�}�b�v�ɕ`������
	m_shadowMap.Render(rc, camera);

}

void RenderingEngine::BloomRendering(RenderContext& rc, RenderTarget& mainRT)
{
	//�u���[������
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
	//�C�e���[�^���쐬
	std::vector<Model*>::iterator itr;
	
	//���f��������
	itr = std::find(
		m_commonModels.begin(),
		m_commonModels.end(),
		&model);

	//���f��������������폜
	if (itr != m_commonModels.end()) {
		m_commonModels.erase(itr);
	}
}

