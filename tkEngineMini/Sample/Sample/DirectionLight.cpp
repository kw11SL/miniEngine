#include "stdafx.h"
#include "DirectionLight.h"
#include "Light.h"

DirectionLight::DirectionLight()
{

}

DirectionLight::~DirectionLight()
{

}

bool DirectionLight::Start()
{

	return true;
}

void DirectionLight::Update()
{
	Rotation();
}

void DirectionLight::Init(const Vector3& direction, const Vector3& color,const Vector3& ambLig)
{
	m_light.directionLight.direction.x = direction.x;
	m_light.directionLight.direction.y = direction.y;
	m_light.directionLight.direction.z = direction.z;
	//ï˚å¸Çê≥ãKâª
	m_light.directionLight.direction.Normalize();

	m_light.directionLight.color.x = color.x;
	m_light.directionLight.color.y = color.y;
	m_light.directionLight.color.z = color.z;

	m_light.ambientLight.x = ambLig.x;
	m_light.ambientLight.y = ambLig.y;
	m_light.ambientLight.z = ambLig.z;
}

void DirectionLight::Rotation()
{
	Vector3 direction = GetDirection();

	float angleH = g_pad[0]->GetRStickXF();
	float angleV = g_pad[0]->GetRStickYF();

	if (g_pad[0]->IsPress(enButtonRB1)) {
		m_rotationH.SetRotationDeg(Vector3::AxisY, angleH * 6.0f);
		m_rotationH.Apply(direction);
		m_rotationV.SetRotationDeg(Vector3::AxisX, angleV * 6.0f);
		m_rotationV.Apply(direction);
	}

	SetDirection(direction);

}

void DirectionLight::SetDirection(const Vector3& direction)
{
	m_light.directionLight.direction.x = direction.x;
	m_light.directionLight.direction.y = direction.y;
	m_light.directionLight.direction.z = direction.z;
	//ê≥ãKâª
	m_light.directionLight.direction.Normalize();
}

void DirectionLight::SetRotationH(const Quaternion& rotH)
{
	m_rotationH = rotH;
}

void DirectionLight::SetRotationV(const Quaternion& rotV)
{
	m_rotationV = rotV;
}

void DirectionLight::SetColor(const Vector3& color)
{
	m_light.directionLight.color.x = color.x;
	m_light.directionLight.color.y = color.y;
	m_light.directionLight.color.z = color.z;
}

void DirectionLight::SetEyePos(const Vector3& pos)
{
	m_light.eyePos.x = pos.x;
	m_light.eyePos.y = pos.y;
	m_light.eyePos.z = pos.z;
}