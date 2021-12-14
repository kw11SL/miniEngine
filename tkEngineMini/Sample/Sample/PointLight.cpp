#include "stdafx.h"
#include "PointLight.h"


bool PointLight::Start()
{
	return true;
}

void PointLight::Init(const Vector3& position,const Vector3& color, float range)
{
	//メンバ変数に記録
	m_position = position;
	m_color = color;
	m_range = range;

	//座標を決定
	m_ptLight.pointLight.position = m_position;
	//色を決定
	m_ptLight.pointLight.color = m_color;
	//影響範囲を決定
	m_ptLight.pointLight.ptRange = m_range;
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
	m_position = position;

	//座標を決定
	m_ptLight.pointLight.position = m_position;
}

void PointLight::SetColor(const Vector3& color)
{
	m_color = color;

	//色を決定
	m_ptLight.pointLight.color = m_color;
}

void PointLight::SetRange(float range)
{
	m_range = range;

	//影響範囲を決定
	m_ptLight.pointLight.ptRange = m_range;
}

void PointLight::Update()
{
	//Move();
}