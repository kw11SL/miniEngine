#include "stdafx.h"
#include "SpotLight.h"
#include "SkinModelRender.h"

bool SpotLight::Start()
{
	return true;
}

void SpotLight::Init(const Vector3& position, const Vector3& color, const float& range, const Vector3& direction, const float& angle)
{
	//メンバ変数に記録
	m_position = position;
	m_color = color;
	m_range = range;
	//方向ベクトルを正規化
	m_direction = direction;
	m_direction.Normalize();
	m_angle = angle;

	//座標を決定
	m_spLight.spotLight.position = m_position;
	//カラーを決定
	m_spLight.spotLight.color = m_color;
	//影響範囲を決定
	m_spLight.spotLight.spRange = m_range;
	//射出方向を決定
	m_spLight.spotLight.spDirection = m_direction;
	//射出角度を決定
	m_spLight.spotLight.spAngle = m_angle;
}

void SpotLight::SetPosition(const Vector3& position)
{
	m_position = position;

	m_spLight.spotLight.position = m_position;
}

void SpotLight::SetColor(const Vector3& color)
{
	m_color = color;

	m_spLight.spotLight.color = m_color;
}

void SpotLight::SetRange(float range)
{
	m_range = range;

	m_spLight.spotLight.spRange = m_range;
}

void SpotLight::SetDirection(const Vector3& direction)
{
	m_direction = direction;

	m_spLight.spotLight.spDirection = m_direction;
}

void SpotLight::SetAngle(float angle)
{
	m_angle = angle;

	m_spLight.spotLight.spAngle = m_angle;
}

void SpotLight::Move()
{
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	if (!g_pad[0]->IsPress(enButtonRB1) && !g_pad[0]->IsPress(enButtonLB1)) {
		m_position.x -= x * 2.0f;
		m_position.y += y * 2.0f;
	}

	//m_skinModelRender->SetPosition(m_position);
	SetPosition(m_position);
}

void SpotLight::Rotation()
{
	////水平回転
	//Quaternion qRotH;
	//qRotH.SetRotation(Vector3::AxisY, g_pad[0]->GetLStickXF() * 0.1f);
	//qRotH.Apply(m_direction);

	////垂直回転
	//Quaternion qRotV;
	////回転軸の決定
	//Vector3 rotAxisV;
	////縦軸と向きの外積から直行するベクトルを求め、軸とする
	//rotAxisV.Cross(Vector3::AxisY, m_direction);

	//qRotV.SetRotation(rotAxisV, g_pad[0]->GetLStickYF() * 0.1f);
	//qRotV.Apply(m_direction);

	//現在向いている方向を取得
	Vector3 direction = GetDirection();
	//スティック入力を取得
	float angleH = g_pad[0]->GetLStickXF();
	float angleV = g_pad[0]->GetLStickYF();

	if (g_pad[0]->IsPress(enButtonLB1)) {
		//水平方向の回転
		m_rotationH.SetRotationDeg(Vector3::AxisY, angleH * 6.0f);
		//水平方向のクォータニオンとベクトルを合成
		m_rotationH.Apply(direction);
		//垂直方向の回転
		m_rotationV.SetRotationDeg(Vector3::AxisX, angleV * 6.0f);
		//垂直方向のクォータニオンとベクトルを合成
		m_rotationV.Apply(direction);
	}

	//合成されたベクトルを反映(=向きが変わる)
	SetDirection(direction);

}

void SpotLight::Update()
{
	Move();
	Rotation();

	//テスト
	//射出角度絞り
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_angle -= Math::DegToRad(1.0f);
	}
	else if (g_pad[0]->IsPress(enButtonRight)) {
		m_angle += Math::DegToRad(1.0f);
	}

	if (m_angle < 0.0f) {
		m_angle = 0.0f;
	}
	
	/*if (m_angle <= Math::DegToRad(0.0f)) {
		m_angle = Math::DegToRad(0.0f);
	}
	else if (m_angle >= Math::DegToRad(60.0f)) {
		m_angle = Math::DegToRad(60.0f);
	}*/

	SetAngle(m_angle);

	//影響範囲変化
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_range += 1.0f;
	}
	else if (g_pad[0]->IsPress(enButtonDown)) {
		m_range -= 1.0f;
	}
	
	if (m_range < 0.0f) {
		m_range = 0.0f;
	}

	SetRange(m_range);

}