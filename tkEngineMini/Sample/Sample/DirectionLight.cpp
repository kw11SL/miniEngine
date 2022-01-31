#include "stdafx.h"
#include "DirectionLight.h"
#include "Light.h"

namespace {
	const Vector3 MAX_COLOR = { 0.25f,0.25f,0.25f };
	const Vector3 MIN_COLOR = { 0.0f,0.0f,0.0f };
	const Vector3 MAX_COLOR_AMB = { 0.7f,0.7f,0.7f };
	const Vector3 MIN_COLOR_AMB = { 0.0f,0.0f,0.0f };

	const float MAX_COLOR_RATE = 1.0f;
	const float MAX_COLOR_AMB_RATE = 1.0f;

	const float COLOR_FADEIN_RATE = 0.01f;
}

bool DirectionLight::Start()
{

	return true;
}

void DirectionLight::Init(const Vector3& direction, const Vector3& color,const Vector3& ambLig)
{
	//�����o�ϐ��ɋL�^
	m_direction = direction;
	m_direction.Normalize();
	/*m_color = color * 1.0f;
	m_ambientLig = ambLig * 0.9f;*/
	/*m_color = color * 0.0f;
	m_ambientLig = ambLig * 0.0f;*/

	//����������
	m_dirLight.directionLight.direction = m_direction;
	//�J���[������
	m_dirLight.directionLight.color = MIN_COLOR;
	//�A���r�G���g���C�g������
	m_dirLight.ambientLight = MIN_COLOR_AMB;
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

void DirectionLight::SetAmbColor(const Vector3& ambColor)
{
	m_colorAmb = ambColor;

	m_dirLight.ambientLight = m_colorAmb;
}

void DirectionLight::SetEyePos(const Vector3& pos)
{
	m_eyePos = pos;

	m_dirLight.eyePos = m_eyePos;
}

void DirectionLight::FadeIn(const float addRate)
{
	//���C�g�̃J���[�ݒ�
	if (m_colorRate < 1.0f) {
		m_colorRate += addRate;
	}

	if (m_colorRate >= 1.0f) {
		m_colorRate = 1.0f;
	}

	m_color.Lerp(m_colorRate, MIN_COLOR, MAX_COLOR * MAX_COLOR_RATE);
	SetColor(m_color);

	//�A���r�G���g���C�g�̃J���[�ݒ�
	if (m_colorAmbRate < 1.0f) {
		m_colorAmbRate += addRate;
	}

	if (m_colorAmbRate >= 1.0f) {
		m_colorAmbRate = 1.0f;
	}

	m_colorAmb.Lerp(m_colorAmbRate, MIN_COLOR_AMB, MAX_COLOR_AMB * MAX_COLOR_AMB_RATE);
	SetAmbColor(m_colorAmb);
}


void DirectionLight::Update()
{
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	FadeIn(COLOR_FADEIN_RATE);

	//���_��ݒ�
	m_eyePos = g_camera3D->GetPosition();
	SetEyePos(m_eyePos);

	m_direction = g_camera3D->GetPosition() - g_camera3D->GetTarget();
	//m_direction = g_camera3D->GetTarget() - g_camera3D->GetPosition();
	SetDirection(m_direction);


	//Rotation();
}