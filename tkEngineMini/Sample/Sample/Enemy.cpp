#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	//����������G�l�~�[����+1
	GameDirector::GetInstance()->AddEnemyCount();
	//�G�l�~�[������+1
	GameDirector::GetInstance()->AddEnemyTotalCount();
}

Enemy::~Enemy()
{
	//���j���ꂽ��G�l�~�[����-1
	GameDirector::GetInstance()->DecEnemyCount();
}

bool Enemy::Start()
{
	return true;
}

void Enemy::Init(
	const Vector3& initPoint,
	const Vector3& initUp,
	const EnEnemyType& enemyType)
{
	//�G�l�~�[�̃^�C�v��ݒ�
	m_enEnemyType = enemyType;

	//�G�l�~�[�̃^�C�v���ƂɊ��N���X�̃I�u�W�F�N�g�ɓn��
	//�h���N���X�̃I�u�W�F�N�g��U�蕪����
	switch (m_enEnemyType) {
	//�ʏ�G�l�~�[
	case enCommon:
		m_enemyBase = &m_enemyNormal;
		break;
	//�ˌ��^�G�l�~�[
	case enShot:
		m_enemyBase = &m_enemyShot;
		break;
	//�����^�G�l�~�[
	case enBomb:
		m_enemyBase = &m_enemyBomb;
		break;
	default:
		break;
	}

	//�G�l�~�[���N���X�̃I�u�W�F�N�g��������
	m_enemyBase->Init(
		initPoint,
		initUp
	);
}


void Enemy::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if(GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//�G�l�~�[���N���X�̃I�u�W�F�N�g���X�V
	m_enemyBase->Update();
}