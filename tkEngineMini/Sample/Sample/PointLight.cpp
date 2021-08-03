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
	//���W������
	m_ptLight.pointLight.position.x = position.x;
	m_ptLight.pointLight.position.y = position.y;
	m_ptLight.pointLight.position.z = position.z;
	//�F������
	m_ptLight.pointLight.color.x = color.x;
	m_ptLight.pointLight.color.y = color.y;
	m_ptLight.pointLight.color.z = color.z;
	//�e���͈͂�����
	m_ptLight.pointLight.ptRange = range;
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
	//���W������
	m_ptLight.pointLight.position.x = position.x;
	m_ptLight.pointLight.position.y = position.y;
	m_ptLight.pointLight.position.z = position.z;
}

void PointLight::SetColor(const Vector3& color)
{
	//�F������
	m_ptLight.pointLight.color.x = color.x;
	m_ptLight.pointLight.color.y = color.y;
	m_ptLight.pointLight.color.z = color.z;
}

void PointLight::SetRange(float range)
{
	//�e���͈͂�����
	m_ptLight.pointLight.ptRange = range;
}