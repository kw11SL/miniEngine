#include "stdafx.h"
#include "SpotLight.h"
#include "SkinModelRender.h"

bool SpotLight::Start()
{
	return true;
}

void SpotLight::Init(const Vector3& position, const Vector3& color, const float& range, const Vector3& direction, const float& angle)
{
	//�����o�ϐ��ɋL�^
	m_position = position;
	m_color = color;
	m_range = range;
	//�����x�N�g���𐳋K��
	m_direction = direction;
	m_direction.Normalize();
	m_angle = angle;

	//���W������
	m_spLight.spotLight.position = m_position;
	//�J���[������
	m_spLight.spotLight.color = m_color;
	//�e���͈͂�����
	m_spLight.spotLight.spRange = m_range;
	//�ˏo����������
	m_spLight.spotLight.spDirection = m_direction;
	//�ˏo�p�x������
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
	////������]
	//Quaternion qRotH;
	//qRotH.SetRotation(Vector3::AxisY, g_pad[0]->GetLStickXF() * 0.1f);
	//qRotH.Apply(m_direction);

	////������]
	//Quaternion qRotV;
	////��]���̌���
	//Vector3 rotAxisV;
	////�c���ƌ����̊O�ς��璼�s����x�N�g�������߁A���Ƃ���
	//rotAxisV.Cross(Vector3::AxisY, m_direction);

	//qRotV.SetRotation(rotAxisV, g_pad[0]->GetLStickYF() * 0.1f);
	//qRotV.Apply(m_direction);

	//���݌����Ă���������擾
	Vector3 direction = GetDirection();
	//�X�e�B�b�N���͂��擾
	float angleH = g_pad[0]->GetLStickXF();
	float angleV = g_pad[0]->GetLStickYF();

	if (g_pad[0]->IsPress(enButtonLB1)) {
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

void SpotLight::Update()
{
	Move();
	Rotation();

	//�e�X�g
	//�ˏo�p�x�i��
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

	//�e���͈͕ω�
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