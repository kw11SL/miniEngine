#include "stdafx.h"
#include "GameCamera.h"

namespace {
	const float CAMERA_MOOVESPEED = 5.0f;
	const float TARGET_MOVESPEED = 5.0f;

	const float DAMPING_CONST = 35.0f;			//�_���s���O�萔
	const float DAMPING_RATE_CONST = 100.0f;	//���������v�Z����ۂɎg���������̌Œ�l
}

void GameCamera::Init(const float maxMoveSpeed)
{
	//�J�����A�����_���ڕW��ǂ�������X�s�[�h��ݒ�
	m_cameraMoveSpeed = CAMERA_MOOVESPEED;
	m_targetMoveSpeed = TARGET_MOVESPEED;

	//�J�����̐ݒ�
	g_camera3D->SetPosition({ 0.0f,500.0f,200.0f });
	g_camera3D->SetTarget({ 0.0f,500.0f,0.0f });

	//�ړ����x�̍ő�l��ݒ�
	m_maxMoveSpeed = maxMoveSpeed;

	//�r���[�s��A�v���W�F�N�V�����s����X�V
	g_camera3D->Update();
}

void GameCamera::SetUp(const Vector3& up)
{
	g_camera3D->SetUp(up);
}

void GameCamera::ChaseCameraPosition()
{
	//return;
	//�J�������W����J�����ڕW�܂ł̃x�N�g�����쐬
	Vector3 posToTarget = m_cameraPositionTarget - m_cameraPosition;
	//�������擾
	float posToTargetLength = posToTarget.Length();

	//���K��
	posToTarget.Normalize();
	//targetToPos.Normalize();

	
	m_cameraPosition += posToTarget * m_cameraMoveSpeed;
	
	//�J�������W��ݒ�
	g_camera3D->SetPosition(m_cameraPosition);
}

void GameCamera::ChaseTargetPosition()
{
	//return;
	//�����_���璍���_�ڕW�܂ł̃x�N�g�����쐬
	Vector3 posToTarget = m_targetPositionTarget - m_targetPosition;
	float posToTargetLength = posToTarget.Length();

	//���K��
	posToTarget.Normalize();
	//targetToPos.Normalize();

	//�����_�ڕW�̕����ɒ����_���ړ�
	m_targetPosition += posToTarget * m_targetMoveSpeed;

	//�����_��ݒ�
	g_camera3D->SetTarget(m_targetPosition);
}

void GameCamera::LerpUpVector(float fraction, Vector3& up)
{
	//���`�⊮
	up.Lerp(fraction, up, m_upTarget);
	//���K��
	up.Normalize();
}

float& GameCamera::CalcDampingRate(
	float dampingRateCurrent,
	float dampingRateTarget,
	float& moveSpeed
)
{
	//1�t���[��������̌o�ߎ��Ԃ��擾
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	//�v�Z�Ŏg�����߂̃_���s���O�萔��ݒ肷��
	float dampingRate = DAMPING_RATE_CONST;

	//���݂̌���������ڕW�l�܂ł̍����l���v�Z
	float distance = dampingRateTarget - dampingRateCurrent;
	//�����l�̐�Βl���ق�0�̂Ƃ�
	if (fabsf(distance) < FLT_EPSILON) {
		//���݂̌������̕ω��ʂ�0�ɂ��A�ڕW�l��Ԃ�
		moveSpeed = 0.0f;
		return dampingRateTarget;
	}

	/////////////////////////////////////////////////////////////////////////
	//��������ω������邽�߂̉����x�����߂�
	//�����x���쐬
	float springAcceleration;

	//���݂̌��������ڕW�l�ɑ΂��āA�����̕����Ȃ̂������̕����Ȃ̂������߂邽�߂̕���
	//�����l���i�[
	float originalDir = distance;
	//�����l�̐�Βl�Ŋ����Đ��K������
	originalDir /= fabsf(distance);

	//�����x�ɂ͏�ŋ��߂������l���i�[���Ă���
	springAcceleration = distance;

	//�΂˕��������A F = ma = -(Ksx) - (Kdv)
	//����m = 1�Ƃ���ƁA�����x a = -(Ksx) - (Kdv)
	//Ks : �΂˒萔
	//x  : �ڕW�l�܂ł̍����l(�����ł́A�ڕW�l�@���@���ݒl)
	//Kd : �_���s���O�萔
	//v  : ���ݒl�̈ړ����x

	//�܂��A�_���s���O�萔�ƌ���������΂˒萔�����߂�
	float t = DAMPING_CONST / (2.0f * dampingRate);
	//�΂˒萔������
	float springK = t * t;
	

	//��L�΂˕������������x�����߂�
	//-Ksx  
	//�v�Z���������l�́A���ݒl�@���@�ڕW�l�@�Ȃ̂�-1���|����
	//���݁A�����x�ɂ͍����l�������Ă���̂ł΂˒萔���|����
	springAcceleration *= springK;

	//-Kdv
	//�ړ����x���i�[
	float vt = moveSpeed;
	//�ړ����x�Ƀ_���s���O�萔���|����
	vt *= DAMPING_CONST;

	//-Ksx - Kdv
	springAcceleration - vt;

	//�����x�Ɍo�ߎ��Ԃ��|����
	springAcceleration *= deltaTime;
	/////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////
	//�ړ����x�̍X�V
	//�ړ����x�ɉ����x�𑫂�����ł���
	moveSpeed += springAcceleration;
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	//�X�V�����ړ����x���g���Č��ݒl���X�V����
	//�X�V����\��̌��ݒl���쐬
	float newDampingRate = dampingRateCurrent;
	//���Z����l�͈ړ����x
	float addRate = moveSpeed;
	//addRate *= deltaTime;

	//���ݒl���X�V
	newDampingRate += addRate;
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	//�ڕW�l�ɒB�������ǂ����̃`�F�b�N
	//�����ŁA�X�V��̌��ݒl�ƖڕW�l�Ƃ̍����l���v�Z����
	float afterDir;
	afterDir = dampingRateTarget - newDampingRate;

	//�����l���ق�0�̂Ƃ�
	if (fabsf(afterDir) < FLT_EPSILON) {
		//��������ڕW�l�ɐݒ�
		newDampingRate = dampingRateTarget;
		//�ړ����x��0�ɐݒ�
		moveSpeed = 0.0f;
	}
	else {
		
		//���K��
		afterDir /= fabsf(afterDir);

		//�X�V�O�̍����l * �X�V��̍����l�@�����̒l�̂Ƃ��A�X�V��̌��ݒl�͖ڕW�l�𒴂��Ă���
		if (afterDir * originalDir < 0.0f) {
			//�ڕW�l�𒴂��Ă���̂Ō��ݒl��ڕW�l�ɐݒ�
			newDampingRate = dampingRateTarget;
			moveSpeed = 0.0f;
		}

	}
	/////////////////////////////////////////////////////////////////////////


	//�X�V���ꂽ��������Ԃ�
	return newDampingRate;
}

Vector3& GameCamera::CalcSpringVector(
	const Vector3& currentPos,
	const Vector3& targetPos,
	Vector3& moveSpeed,
	float maxMoveSpeed,
	float dampingRate
)
{
	//1�t���[���̌o�ߎ��Ԃ��擾
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	//���݈ʒu�ƖڕW�ʒu�Ƃ̍��������߂�(���݈ʒu�@���@�ڕW�ʒu)
	Vector3 distance = targetPos - currentPos;

	//���i�K�ł́A���݈ʒu����ڕW�ʒu�ւ̕�����ۑ����Ă���
	Vector3 originalDir = distance;
	originalDir.Normalize();

	/////////////////////////////////////////////////////////////////////////
	//�ڕW�ʒu���ړ������邽�߂̉����x�����߂�
	Vector3 springAcceleration;
	springAcceleration = distance;

	//F = ma = -Ksx - Kdv
	//m = 1�̂Ƃ� a = -Ksx - Kdv
	// 
	//�_���s���O�萔�ƌ���������΂˒萔�����߂�
	float t = DAMPING_CONST / (2.0f * dampingRate);
	float springK = t * t;

	//�����x�����߂�
	//-Ksx
	//�����l�́A���݈ʒu�@���@�ڕW�ʒu�@�Ȃ̂�-1���|�����Ă���
	//�����x�ɂ͍����l�������Ă���̂ł΂˒萔���|����
	springAcceleration *springK;

	//-Kdv
	Vector3 vt = moveSpeed;
	vt *= DAMPING_CONST;

	//-Ksx - Kdv
	springAcceleration -= vt;

	//�����x�Ɍo�ߎ��Ԃ��|����
	springAcceleration *= deltaTime;
	/////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////
	//�ړ����x���X�V
	moveSpeed += springAcceleration;
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	//�ڕW�ʒu�𒴂������ǂ����̃`�F�b�N
	//�X�V����\��̍��W���쐬
	Vector3 newPos = currentPos;
	//���W�ɉ��Z����x�N�g�����쐬
	Vector3 addPos = moveSpeed;
	//addPos *= deltaTime;

	//���W���X�V
	newPos += addPos;

	//�X�V��̈ʒu�ƖڕW�ʒu�Ƃ̍����l���v�Z
	Vector3 afterDir = targetPos - newPos;
	afterDir.Normalize();

	//�ō����x�𒴂����ꍇ
	if (moveSpeed.LengthSq() > maxMoveSpeed * maxMoveSpeed) {
		//�ړ����x���ō����x�ɂ���
		moveSpeed.Normalize();
		moveSpeed *= maxMoveSpeed;
	}

	//�ړ����x�����̒l�ɂȂ����Ƃ�
	if (moveSpeed.Length() < 1.0f) {
		newPos = targetPos;
		moveSpeed *= 0.0f;
	}
	else {
		//�X�V�O�̍����l�x�N�g���ƍX�V��̍����l�x�N�g���̓��ς��Ƃ�A���̒l��������ڕW�ʒu�𒴂��Ă���
		if (afterDir.Dot(originalDir) < 0.0f) {
			//�X�V��̈ʒu��ڕW�ʒu�ɂ���
			newPos = targetPos;
			moveSpeed *= 0.0f;
		}

	}
	/////////////////////////////////////////////////////////////////////////

	//�X�V��̈ʒu��Ԃ�
	return newPos;
}

void GameCamera::UpdateSpringCamera()
{
	//���������v�Z
	m_dampingRate = CalcDampingRate(m_dampingRate, m_dampingRateTarget, m_dampingRateVel);
	//�J�����̍��W���v�Z
	m_cameraPosition = CalcSpringVector(m_cameraPosition, m_cameraPositionTarget, m_cameraMoveSpeedVel, m_maxMoveSpeed, m_dampingRate);
	//�����_�̍��W���v�Z
	m_targetPosition = CalcSpringVector(m_targetPosition, m_targetPositionTarget, m_targetMoveSpeedVel, m_maxMoveSpeed, m_dampingRate);

	//�J�����̍��W�ƒ����_�̍��W���X�V
	SetCameraPosition(m_cameraPosition);
	SetTargetPosition(m_targetPosition);
}

void GameCamera::UpdateCamera()
{
	//�΂˃J�����̍X�V
	UpdateSpringCamera();

	//�r���[�s��A�v���W�F�N�V�����s����X�V
	g_camera3D->Update();
}