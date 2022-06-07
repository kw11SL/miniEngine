#include "stdafx.h"
#include "Bullet.h"
#include "ExplosionManager.h"

namespace{
	
}


Bullet::~Bullet()
{
	
}

bool Bullet::Start()
{
	return true;
}

void Bullet::Init(
	const Vector3& initPoint, 
	const Vector3& initUp,
	const Vector3& direction, 
	const EnBulletType& bulletType)
{
	
	//�w�肵���^�C�v�ɂ���Ēe�̊��N���X�ɓ����h���N���X�̃I�u�W�F�N�g��U�蕪��
	switch (bulletType){
	case enPlayerNormal:
		m_bulletBase = &m_bulletPlayerNormal;
		break;

	case enPlayerSpreadBomb:
		m_bulletBase = &m_bulletPlayerSpread;
		break;

	case enEnemyNormal:
		m_bulletBase = &m_bulletEnemyNormal;
		break;

	default:
		break;
	};

	//�e�̃x�[�X�N���X�̏�����
	m_bulletBase->Init(
		initPoint,
		initUp,
		direction
	);

}

void Bullet::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//�e�̊��N���X�̍X�V
	m_bulletBase->Update();

}