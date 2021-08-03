#include "stdafx.h"
#include "Direction.h"
#include "SkinModelRender.h"

namespace
{
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const Vector3 INIT_POINT = { 0.0f,50.0f,50.0f };
}


Direction::Direction()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/direction/direction_yellow.tkm",enModelUpAxisZ);
	m_skinModelRender->InitShader(MODEL_SHADER_PATH, VS_ENTRYPOINT_NAME);

	m_skinModelRender->SetPosition(INIT_POINT);

}

Direction::~Direction()
{
	DeleteGO(m_skinModelRender);
}

bool Direction::Start()
{
	return true;
}

void Direction::Move()
{

}

void Direction::Rotation()
{
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_angle += 1.0f;
	}
	else if (g_pad[0]->IsPress(enButtonLeft)) {
		m_angle -= 1.0f;
	}
	Quaternion rotH = Quaternion::Identity;

	rotH.SetRotationDeg(Vector3::AxisY,m_angle);
	m_rotation = rotH;

	m_skinModelRender->SetRotation(m_rotation);

}

void Direction::SetPosition(const Vector3& pos)
{
	m_position.x = pos.x;
	m_position.y = pos.y;
	m_position.z = pos.z;

	m_skinModelRender->SetPosition(m_position);
}

void Direction::SetRotation(const Quaternion& qRot)
{
	m_rotation = qRot;
}

void Direction::SetAngle(const float angle)
{
	m_angle = angle;
}

void Direction::SetScale(const Vector3& scale)
{
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;

	m_skinModelRender->SetScale(m_scale);
}

void Direction::RecieveDirectionLight(DirectionLight* dirLight)
{
	m_skinModelRender->InitDirectionLight(dirLight);
}

void Direction::RecievePointLight(PointLight* ptLight)
{
	m_skinModelRender->InitPointLight(ptLight);
}

void Direction::RecieveSpotLight(SpotLight* spLight)
{
	m_skinModelRender->InitSpotLight(spLight);
}

void Direction::Update()
{
	Rotation();

}