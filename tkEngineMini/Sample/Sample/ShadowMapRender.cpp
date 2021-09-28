#include "stdafx.h"
#include "ShadowMapRender.h"

void ShadowMapRender::Init()
{
	//�N���A�J���[
	float clearColor[4] = {1.0f,1.0f,1.0f,1.0f};
	
	m_shadowMaps[0].Create(
		1024,
		1024,
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

}

void ShadowMapRender::Render(RenderContext& rc , Camera& lightCamera)
{

	int shadowMapNo = 0;
	//�����_�����O�^�[�Q�b�g�̐ݒ�
	for (auto& shadowMap : m_shadowMaps) {

		rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
		rc.SetRenderTargetAndViewport(shadowMap);
		rc.ClearRenderTargetView(shadowMap);
	
		//���f����`��
		for (auto& model : m_models[shadowMapNo]) {
			model->Draw(
				rc,
				lightCamera
			);
		}

		//�`��I���ŃN���A
		m_models[shadowMapNo].clear();
		//�������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
	
		//���̃��f����
		shadowMapNo++;
	}


}