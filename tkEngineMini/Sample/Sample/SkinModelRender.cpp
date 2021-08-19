#include "stdafx.h"
#include "SkinModelRender.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace
{
	const char* MODEL_FX_FILEPATH = "Assets/shader/model.fx";
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

void SkinModelRender::Init(const char* modelFilePath, EnModelUpAxis upAxis)
{
	m_modelInitData.m_tkmFilePath = modelFilePath;
	m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH;
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
	m_model.Draw(rc);
}

void SkinModelRender::Update()
{
	m_model.UpdateWorldMatrix(m_position,m_rot,m_scale);
}