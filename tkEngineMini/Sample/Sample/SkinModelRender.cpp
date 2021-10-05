#include "stdafx.h"
#include "SkinModelRender.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace
{
	//�V�F�[�_�̃t�@�C���p�X
	//�ʏ�`��p�̃V�F�[�_�[�t�@�C���p�X
	const char* MODEL_FX_FILEPATH = "Assets/shader/model.fx";
	//�V���h�E�}�b�v�`��p�̃V�F�[�_�[�t�@�C���p�X
	const char* MODEL_FX_FILEPATH_SHADOWMAP = "Assets/shader/ShadowMap.fx";
	//�V���h�E���V�[�o�[�p�̃V�F�[�_�[�t�@�C���p�X
	const char* MODEL_FX_FILEPATH_SHADOWRECIEVER = "Assets/shader/ShadowReciever.fx";

	//�V�F�[�_�̃G���g���[�|�C���g��
	//�ʏ�̃G���g���[�|�C���g
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	//�X�L�����f���̃G���g���[�|�C���g
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
}

bool SkinModelRender::Start()
{

	return true;
}

void SkinModelRender::Init(const char* modelFilePath, EnModelUpAxis upAxis , RenderingEngine& renderingEngine , bool shadowCasterFlag)
{
	m_renderingEngine = &renderingEngine;
	//�V���h�E�L���X�^�[�t���O������
	m_isShadowCaster = shadowCasterFlag;

	//�ʏ�`��p���f����������
	{
		m_modelInitData.m_tkmFilePath = modelFilePath;

		m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH;
		m_modelInitData.m_vsEntryPointFunc = VS_ENTRYPOINT_NAME;
		m_modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		/*m_modelInitData.m_vsEntryPointFunc = "VSMain";
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";*/
		m_modelInitData.m_modelUpAxis = upAxis;

		m_model.Init(m_modelInitData);
	}

	//�e�p���f����������
	{
		m_shadowModelInitData.m_tkmFilePath = modelFilePath;

		m_shadowModelInitData.m_fxFilePath = MODEL_FX_FILEPATH_SHADOWMAP;
		m_shadowModelInitData.m_vsEntryPointFunc = VS_ENTRYPOINT_NAME;
		m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

		m_shadowModelInitData.m_modelUpAxis = upAxis;

		m_shadowModel.Init(m_shadowModelInitData);
	}

	//�ʏ탂�f���������_�����O�G���W���ɒǉ�
	m_renderingEngine->Add3DModelToCommonModel(m_model);

	//�V���h�E�L���X�^�[�t���O�������Ă�����e�p���f���������_�����O�G���W���ɒǉ�
	if (m_isShadowCaster == true) {
		m_renderingEngine->Add3DModelToShadowModel(m_shadowModel);
	}
	
}

void SkinModelRender::InitDirectionLight(DirectionLight* dirLight)
{
	m_modelInitData.m_expandConstantBuffer = dirLight->GetDirLightAddress();
	m_modelInitData.m_expandConstantBufferSize = sizeof(dirLight->GetDirLight());
	
	m_model.Init(m_modelInitData);
}

void SkinModelRender::InitPointLight(PointLight* ptLight)
{
	m_modelInitData.m_expandConstantBuffer_1 = ptLight->GetPointLightAddress();
	m_modelInitData.m_expandConstantBufferSize_1 = sizeof(ptLight->GetPointLight());

	m_model.Init(m_modelInitData);
}

void SkinModelRender::InitSpotLight(SpotLight* spLight)
{
	m_modelInitData.m_expandConstantBuffer_2 = spLight->GetSpotLightAddress();
	m_modelInitData.m_expandConstantBufferSize_2 = sizeof(spLight->GetSpotLight());

	m_model.Init(m_modelInitData);
}

void SkinModelRender::SetPosition(const Vector3& pos)
{
	m_position = pos;
}

void SkinModelRender::SetScale(const Vector3& scale)
{
	m_scale = scale;
}

void SkinModelRender::SetRotation(const Quaternion& rot)
{
	m_rot = rot;
}

void SkinModelRender::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

void SkinModelRender::Update()
{
	m_model.UpdateWorldMatrix(m_position,m_rot,m_scale);
}