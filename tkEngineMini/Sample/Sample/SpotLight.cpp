#include "stdafx.h"
#include "SpotLight.h"

bool SpotLight::Start()
{
	return true;
}

void SpotLight::Init(const Vector3& position, const Vector3& color, const float& range, const Vector3& direction, const float& angle)
{
	//座標を決定
	m_spLight.spotLight.position.x = position.x;
	m_spLight.spotLight.position.y = position.y;
	m_spLight.spotLight.position.z = position.z;

	//カラーを決定
	m_spLight.spotLight.color.x = color.x;
	m_spLight.spotLight.color.y = color.y;
	m_spLight.spotLight.color.z = color.z;

	//影響範囲を決定
	m_spLight.spotLight.spRange = range;

	//射出方向を決定
	m_spLight.spotLight.spDirection.x = direction.x;
	m_spLight.spotLight.spDirection.y = direction.y;
	m_spLight.spotLight.spDirection.z = direction.z;

	//射出角度を決定
	m_spLight.spotLight.spAngle = angle;
}

void SpotLight::SetPosition(const Vector3& position)
{
	m_spLight.spotLight.position.x = position.x;
	m_spLight.spotLight.position.y = position.y;
	m_spLight.spotLight.position.z = position.z;
}

void SpotLight::SetColor(const Vector3& color)
{
	m_spLight.spotLight.color.x = color.x;
	m_spLight.spotLight.color.y = color.y;
	m_spLight.spotLight.color.z = color.z;
}

void SpotLight::SetRange(float range)
{
	m_spLight.spotLight.spRange = range;
}

void SpotLight::SetDirection(const Vector3& direction)
{
	m_spLight.spotLight.spDirection.x = direction.x;
	m_spLight.spotLight.spDirection.y = direction.y;
	m_spLight.spotLight.spDirection.z = direction.z;
}

void SpotLight::SetAngle(float angle)
{
	m_spLight.spotLight.spAngle = angle;
}

void SpotLight::Move()
{
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	if (!g_pad[0]->IsPress(enButtonRB1) && !g_pad[0]->IsPress(enButtonLB1)) {
		m_position.x -= x * 2.0f;
		m_position.y += y * 2.0f;
	}

	SetPosition(m_position);
}

void SpotLight::Rotation()
{

}

void SpotLight::Update()
{
	Move();

	//テスト
	//射出角度絞り
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_angle += 1.0f;
	}
	else if (g_pad[0]->IsPress(enButtonRight)) {
		m_angle -= 1.0f;
	}
	SetAngle(m_angle);

	//影響範囲変化
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_range += 1.0f;
	}
	else if (g_pad[0]->IsPress(enButtonDown)) {
		m_range -= 1.0f;
	}
	SetRange(m_range);

}