#include "stdafx.h"
#include "ShadowMapRender.h"

namespace 
{
	const int SHADOWMAP_WIDTH = 1024;
	const int SHADOWMAP_HEIGHT = 1024;
}

void ShadowMapRender::Init()
{
	//�V���h�E�}�b�v��������
	InitShadowMap();

}

void ShadowMapRender::InitShadowMap()
{
	//�N���A�J���[
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
//	//�N���A�J���[
//	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
//	
//	//�ߌi�p�V���h�E�}�b�v
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
//	//���i�p�V���h�E�}�b�v
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
//	//���i�p�V���h�E�}�b�v
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
	//�����_�����O�^�[�Q�b�g���g�p�\�ɂȂ�܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	rc.SetRenderTargetAndViewport(m_shadowMap);
	rc.ClearRenderTargetView(m_shadowMap);

	//���f�����V���h�E�}�b�v�ɕ`��
	for (auto& model : m_modelsArray) {
		if (model != nullptr) {
			model->Draw(rc , lightCamera);
		}
	}

	//�`�����݊����҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
}

void ShadowMapRender::DeleteModel(Model* model)
{
	//�C�e���[�^���쐬
	std::vector<Model*>::iterator itr;

	//���f���z�������
	itr = std::find(
		m_modelsArray.begin(),
		m_modelsArray.end(),
		model
	);

	//����������폜
	if (itr != m_modelsArray.end()) {
		m_modelsArray.erase(itr);
	}
}

//void ShadowMapRender::MultiRender(RenderContext& rc , Camera& lightCamera)
//{
//
//	int shadowMapNo = 0;
//	
//	//�����_�����O�^�[�Q�b�g�̐ݒ�
//	for (auto& shadowMap : m_shadowMaps) {
//		rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
//		rc.SetRenderTargetAndViewport(shadowMap);
//		rc.ClearRenderTargetView(shadowMap);
//	
//		//���f����`��
//		for (auto& model : m_models[shadowMapNo]) {
//			model->Draw(
//				rc,
//				lightCamera
//			);
//		}
//
//		//�`��I���ŃN���A
//		m_models[shadowMapNo].clear();
//		//�������ݏI���҂�
//		rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
//	
//		//���̃��f����
//		shadowMapNo++;
//	}
//
//
//}