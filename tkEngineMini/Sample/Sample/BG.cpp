#include "stdafx.h"
#include "BG.h"

namespace
{
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* MODEL_FILEPATH = "Assets/modelData/bg/bg.tkm";
	const Vector3 INIT_POINT = { 0.0f,50.0f,50.0f };
}

BG::BG()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(MODEL_FILEPATH,enModelUpAxisZ);
	m_skinModelRender->InitShader(MODEL_SHADER_PATH, VS_ENTRYPOINT_NAME);
}

BG::~BG()
{
	DeleteGO(m_skinModelRender);
}

bool BG::Start()
{
	return true;
}

void BG::Update()
{

}

void BG::RecieveDirectionLight(DirectionLight* dirLight)
{
	m_skinModelRender->InitDirectionLight(dirLight);
}

void BG::RecievePointLight(PointLight* ptLight)
{
	m_skinModelRender->InitPointLight(ptLight);
}

void BG::RecieveSpotLight(SpotLight* spLight)
{
	m_skinModelRender->InitSpotLight(spLight);
}