#include "stdafx.h"
#include "EnemyExplosion.h"

namespace {
	//エフェクトのファイルパス
	const char16_t* EFFECT_FILEPATH = u"Assets/effect/enemy_explosion.efk";
	//生存時間
	const float LIFETIME = 1.2f;
	//ダメージ半径
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