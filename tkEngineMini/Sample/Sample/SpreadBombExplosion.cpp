#include "stdafx.h"
#include "SpreadBombExplosion.h"

namespace {
	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* EFFECT_BULLET_FILEPATH = u"Assets/effect/shot_spread_burst.efk";
	//��������
	const float LIFETIME = 2.0f;
	//�_���[�W��
	const float POWER = 5.0f;
	//�_���[�W���a
	const float DAMAGE_RADIUS = 200.0f;
	//�_���[�W��^����Ԋu
	const float DAMAGE_INTERVAL = 0.15f;

}

SpreadBombExplosion::~SpreadBombExplosion()
{

}

void SpreadBombExplosion::InitSub()
{
	m_effectFilePath = EFFECT_BULLET_FILEPATH;
	m_lifeTime = LIFETIME;
	m_power = POWER;
	m_damageArea = DAMAGE_RADIUS;
	m_damageInterval = DAMAGE_INTERVAL;
}

void SpreadBombExplosion::UpdateSub()
{
	DecLifeTime();
	Destroy();
	EffectUpdate();
}