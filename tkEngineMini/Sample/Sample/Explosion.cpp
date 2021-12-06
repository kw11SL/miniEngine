#include "stdafx.h"
#include "Explosion.h"

void Explosion::Init(
	const Vector3& pos,
	const float scaleRate,
	const EnExplosionType& explosionType
	)
{
	//�w�肳�ꂽ�����̃^�C�v�Ŋ��N���X�ɓ����I�u�W�F�N�g��U�蕪��
	switch (explosionType) {
	case enPlayer_Spread_Bomb:
		m_explosionBase = &m_sBomb;
		break;
	case enEnemy_Explosion:
		m_explosionBase = &m_enemyExplosion;
		break;
	default:
		break;
	}


	//���W�A�g�嗦��ݒ肵�A������
	m_explosionBase->Init(
		pos,
		scaleRate
	);
}

bool Explosion::Start()
{
	return true;
}

void Explosion::Update()
{
	m_explosionBase->Update();

	if (m_explosionBase->GetIsExist() == false) {
		DeleteGO(this);
	}

}