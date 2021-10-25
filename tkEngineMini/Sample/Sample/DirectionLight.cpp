#include "stdafx.h"
#include "DirectionLight.h"
#include "Light.h"

bool DirectionLight::Start()
{

	return true;
}

void DirectionLight::Init(const Vector3& direction, const Vector3& color,const Vector3& ambLig)
{
	//�����o�ϐ��ɋL�^
	m_direction = direction;
	m_direction.Normalize();
	m_color = color;
	m_ambientLig = ambLig;

	//����������
	m_dirLight.directionLight.direction = m_direction;
	//�J���[������
	m_dirLight.directionLight.color = m_color;
	//�A���r�G���g���C�g������
	m_dirLight.ambientLight = m_ambientLig;
}

void DirectionLight::Rotation()
{
	//���݌����Ă���������擾
	Vector3 direction = GetDirection();
	//�X�e�B�b�N���͂��擾
	float angleH = g_pad[0]->GetRStickXF();
	float angleV = g_pad[0]->GetRStickYF();

	if (g_pad[0]->IsPress(enButtonRB1)) {
		//���������̉�]
		m_rotationH.SetRotationDeg(Vector3::AxisY, angleH * 6.0f);
		//���������̃N�H�[�^�j�I���ƃx�N�g��������
		m_rotationH.Apply(direction);
		//���������̉�]
		m_rotationV.SetRotationDeg(Vector3::AxisX, angleV * 6.0f);
		//���������̃N�H�[�^�j�I���ƃx�N�g��������
		m_rotationV.Apply(direction);
	}

	//�������ꂽ�x�N�g���𔽉f(=�������ς��)
	SetDirection(direction);

}

void DirectionLight::SetDirection(const Vector3& direction)
{
	m_direction = direction;
	//�����𐳋K��
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