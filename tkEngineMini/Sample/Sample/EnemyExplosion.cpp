#include "stdafx.h"
#include "EnemyExplosion.h"

namespace {
	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* EFFECT_FILEPATH = u"Assets/effect/enemy_explosion.efk";
	//��������
	const float LIFETIME = 1.2f;
	//�_���[�W���a
	const float DAMAGE_RADIUS = 200.0f;
}


EnemyExplosion::~EnemyExplosion()
{

}

void EnemyExplosion::InitSub()
{
	m_effectFilePath = EFFECT_FILEPATH;
	m_lifeTime = LIFETIME;
	m_damageArea = DAMAGE_RADIUS;
}

void EnemyExplosion::UpdateSub()
{
	DecLifeTime();
	Destroy();
	EffectUpdate();
}