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

SkinModelRender::SkinModelRender()
{

}

SkinModelRender::~SkinModelRender()
{

}

bool SkinModelRender::Start()
{

	return true;
}

void SkinModelRender::Init(const char* modelFilePath, EnShadingMode shadingMode, EnModelUpAxis upAxis)
{
	m_modelInitData.m_tkmFilePath = modelFilePath;
	
	//�w�肵���V�F�[�f�B���O���[�h���ɃV�F�[�_�̃t�@�C���p�X��U�蕪��
	switch (shadingMode) {
		case enCommonShading:
			m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH;
			break;
		case enShadowMap:
			m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH_SHADOWMAP;
			break;
		case enShadowReciever:
			m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH_SHADOWRECIEVER;
			break;
		default:
			break;
	}

	/*m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";*/
	m_modelInitData.m_modelUpAxis = upAxis;
	
	m_model.Init(m_modelInitData);
}

void SkinModelRender::InitShader(const char* fxFilePath, const char* entryPoint)
{
	m_modelInitData.m_fxFilePath = fxFilePath;
	m_modelInitData.m_vsEntryPointFunc = entryPoint;
	
	m_model.Init(m_modelInitData);
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
	if (m_shadingMode == enCommonShading) {
		m_model.Draw(rc);
	}
}

void SkinModelRender::Update()
{
	m_model.UpdateWorldMatrix(m_position,m_rot,m_scale);
}