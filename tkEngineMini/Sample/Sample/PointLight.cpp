#include "stdafx.h"
#include "PointLight.h"


bool PointLight::Start()
{
	return true;
}

void PointLight::Init(const Vector3& position,const Vector3& color, float range)
{
	//�����o�ϐ��ɋL�^
	m_position = position;
	m_color = color;
	m_range = range;

	//���W������
	m_ptLight.pointLight.position = m_position;
	//�F������
	m_ptLight.pointLight.color = m_color;
	//�e���͈͂�����
	m_ptLight.pointLight.ptRange = m_range;
}

void PointLight::Move()
{

	//todo ���f���N���X�Ɠ����悤�ɓ�������悤�ɂ���

	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//�����ړ�
	if (g_pad[0]->IsPress(enButtonLB1)) {
		m_position.x -= x * 2.0f;
		m_position.y += y * 2.0f;
	}

	//�㉺�ړ�
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

	//���W������
	m_ptLight.pointLight.position = m_position;
}

void PointLight::SetColor(const Vector3& color)
{
	m_color = color;

	//�F������
	m_ptLight.pointLight.color = m_color;
}

void PointLight::SetRange(float range)
{
	m_range = range;

	//�e���͈͂�����
	m_ptLight.pointLight.ptRange = m_range;
}

void PointLight::Update()
{
	//Move();
}