#include "stdafx.h"
#include "Player_new.h"
#include "SkinModelRender.h"

namespace{
	const char* MODELPATH_UTC = "Assets/modelData/unityChan.tkm";
	const float UTC_RADIUS = 40.0f;
	const float UTC_HEIHGT = 100.0f;
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	const Vector3 INIT_POINT = {0.0f,0.0f,0.0f};
}

Player_new::Player_new()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(MODELPATH_UTC, enModelUpAxisZ);
	m_skinModelRender->InitShader(MODEL_SHADER_PATH, VS_ENTRYPOINT_NAME);

	m_skinModelRender->SetPosition(INIT_POINT);
	m_skinModelRender->SetScale(m_scale);

}

Player_new::~Player_new()
{
	DeleteGO(m_skinModelRender);
}

void Player_new::Init()
{

}

bool Player_new::Start()
{
	return true;
}

void Player_new::SetPostion(const Vector3& pos)
{
	m_position.x = pos.x;
	m_position.y = pos.y;
	m_position.z = pos.z;

	m_skinModelRender->SetPosition(m_position);
}

void Player_new::SetScale(const Vector3& scale)
{
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;

	m_skinModelRender->SetScale(m_scale);
}

void Player_new::SetRotation(const Quaternion rot)
{
	m_rot = rot;
}
void Player_new::SetAngle(const float& angle)
{
	m_angle = angle;
}

void Player_new::Move()
{
	
}

void Player_new::Rotation()
{
	m_angle += g_pad[0]->GetLStickXF() * 2.0f;

	if (m_angle > 360.0f) {
		m_angle = 0.0f;
	}

	m_rot.SetRotationDeg(Vector3::AxisY, m_angle);
}

void Player_new::RecieveDirectionLight(DirectionLight* dirLight)
{
	m_skinModelRender->InitDirectionLight(dirLight);
}

void Player_new::RecievePointLight(PointLight* ptLight)
{
	m_skinModelRender->InitPointLight(ptLight);
}

void Player_new::RecieveSpotLight(SpotLight* spLight)
{
	m_skinModelRender->InitSpotLight(spLight);
}

SkinModelRender* Player_new::GetSkinModelRender()
{
	return m_skinModelRender;
}


void Player_new::Update()
{
	Rotation();

	m_skinModelRender->SetRotation(m_rot);
}