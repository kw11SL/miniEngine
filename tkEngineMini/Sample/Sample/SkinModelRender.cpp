#include "stdafx.h"
#include "SkinModelRender.h"

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

SkinModelRender::~SkinModelRender()
{
	//�����_�����O�G���W�����烂�f�����폜
	m_renderingEngine->DeleteCommonModel(m_model);
	
	//�V���h�E�L���X�^�[�t���O�������Ă�����e�p���f�����폜
	if (m_isShadowCaster) {
		m_renderingEngine->DeleteShadowModel(m_shadowModel);
	}

}

bool SkinModelRender::Start()
{

	return true;
}

void SkinModelRender::Init(
	const char* modelFilePath, 
	EnModelUpAxis upAxis, 
	RenderingEngine& renderingEngine, 
	bool shadowCasterFlag, 
	bool shadowRecieverFlag, 
	const char* skeletonFilePath)
{
	m_renderingEngine = &renderingEngine;
	
	//�V���h�E�L���X�^�[�t���O������
	SetShadowChastarFlag(shadowCasterFlag);
	
	//�V���h�E���V�[�o�[�t���O������
	SetShadowRecieverFlag(shadowRecieverFlag);

	//�X�P���g�����w�肳��Ă�����
	if (skeletonFilePath != nullptr) {
		//�X�P���g����������
		m_skeleton.Init(skeletonFilePath);
		//���f���̏��������̃����o�ɃX�P���g���ɃX�P���g����o�^
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	//�ʏ�`��p���f����������
	{
		//���ʏ���
		m_modelInitData.m_tkmFilePath = modelFilePath;
		m_modelInitData.m_vsEntryPointFunc = VS_ENTRYPOINT_NAME;
		m_modelInitData.m_vsSkinEntryPointFunc = VS_SKIN_ENTRYPOINT_NAME;
		m_modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		m_modelInitData.m_modelUpAxis = upAxis;

		if (shadowRecieverFlag == false) {
			//�V���h�E���V�[�o�[�t���O���I�t�Ȃ�ʏ�̃V�F�[�_�[���w��
			m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH;
		}
		else {
			//�V���h�E���V�[�o�[�t���O���I���̂Ƃ�
			//�V���h�E���V�[�o�[�p�̃V�F�[�_�[���w��
			m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH_SHADOWRECIEVER;
			//�V���h�E�}�b�v���g��SRV(�V�F�[�_�[���\�[�X�r���[)�ɐݒ肷��
			m_modelInitData.m_expandShaderResoruceView[0] = &RenderingEngine::GetInstance()->GetShadowMap().GetRenderTargetTexture();
			//���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�\�ɐݒ肷��
			m_modelInitData.m_expandConstantBuffer_3 = (void*)&RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
			m_modelInitData.m_expandConstantBufferSize_3 = sizeof(RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix());

		}

		
		//���f���̏�����
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
	
	//m_model.Init(m_modelInitData);
}

void SkinModelRender::InitPointLight(PointLight* ptLight)
{
	m_modelInitData.m_expandConstantBuffer_1 = ptLight->GetPointLightAddress();
	m_modelInitData.m_expandConstantBufferSize_1 = sizeof(ptLight->GetPointLight());

	//m_model.Init(m_modelInitData);
}

void SkinModelRender::InitSpotLight(SpotLight* spLight)
{
	m_modelInitData.m_expandConstantBuffer_2 = spLight->GetSpotLightAddress();
	m_modelInitData.m_expandConstantBufferSize_2 = sizeof(spLight->GetSpotLight());

	//m_model.Init(m_modelInitData);
}

void SkinModelRender::InitModel()
{
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
	//���f���̍X�V
	m_model.UpdateWorldMatrix(m_position,m_rot,m_scale);

	//�X�P���g���̍X�V
	m_skeleton.Update(m_model.GetWorldMatrix());

	//�V���h�E�L���X�^�[�t���O�����Ă�����e���f�����X�V
	if (m_isShadowCaster) {
		m_shadowModel.UpdateWorldMatrix(m_position, m_rot, m_scale);
	}

}