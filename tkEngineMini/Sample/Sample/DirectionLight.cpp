#include "stdafx.h"
#include "DirectionLight.h"
#include "Light.h"

bool DirectionLight::Start()
{

	return true;
}

void DirectionLight::Init(const Vector3& direction, const Vector3& color,const Vector3& ambLig)
{
	//メンバ変数に記録
	m_direction = direction;
	m_direction.Normalize();
	m_color = color;
	m_ambientLig = ambLig;

	//方向を決定
	m_dirLight.directionLight.direction = m_direction;
	//カラーを決定
	m_dirLight.directionLight.color = m_color;
	//アンビエントライトを決定
	m_dirLight.ambientLight = m_ambientLig;
}

void DirectionLight::Rotation()
{
	//現在向いている方向を取得
	Vector3 direction = GetDirection();
	//スティック入力を取得
	float angleH = g_pad[0]->GetRStickXF();
	float angleV = g_pad[0]->GetRStickYF();

	if (g_pad[0]->IsPress(enButtonRB1)) {
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

void DirectionLight::SetDirection(const Vector3& direction)
{
	m_direction = direction;
	//方向を正規化
	m_direction.Normalize();

	m_dirLight.directionLight.direction = m_direction;
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
	m_color = color;

	m_dirLight.directionLight.color = m_color;
}

void DirectionLight::SetEyePos(const Vector3& pos)
{
	m_eyePos = pos;

	m_dirLight.eyePos = m_eyePos;
}

void DirectionLight::Update()
{
	//Rotation();
}