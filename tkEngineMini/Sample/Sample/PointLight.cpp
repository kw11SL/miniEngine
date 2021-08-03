#include "stdafx.h"
#include "PointLight.h"


bool PointLight::Start()
{
	return true;
}

void PointLight::Update()
{
	Move();
}

void PointLight::Init(const Vector3& position,const Vector3& color, float range)
{
	//座標を決定
	m_ptLight.pointLight.position.x = position.x;
	m_ptLight.pointLight.position.y = position.y;
	m_ptLight.pointLight.position.z = position.z;
	//色を決定
	m_ptLight.pointLight.color.x = color.x;
	m_ptLight.pointLight.color.y = color.y;
	m_ptLight.pointLight.color.z = color.z;
	//影響範囲を決定
	m_ptLight.pointLight.ptRange = range;
}

void PointLight::Move()
{

	//todo モデルクラスと同じように動かせるようにする

	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//水平移動
	if (g_pad[0]->IsPress(enButtonLB1)) {
		m_position.x -= x * 2.0f;
		m_position.y += y * 2.0f;
	}

	//上下移動
	if (g_pad[0]->IsPress(enButtonRB2)) {
		m_position.z += 2.0f;
	}
	else if (g_pad[0]->IsPress(enButtonLB2)) {
		m_position.z -= 2.0f;
	}

	SetPosition(m_position);
}

void PointLight::SetPosition(const Vector3& position)
{
	//座標を決定
	m_ptLight.pointLight.position.x = position.x;
	m_ptLight.pointLight.position.y = position.y;
	m_ptLight.pointLight.position.z = position.z;
}

void PointLight::SetColor(const Vector3& color)
{
	//色を決定
	m_ptLight.pointLight.color.x = color.x;
	m_ptLight.pointLight.color.y = color.y;
	m_ptLight.pointLight.color.z = color.z;
}

void PointLight::SetRange(float range)
{
	//影響範囲を決定
	m_ptLight.pointLight.ptRange = range;
}