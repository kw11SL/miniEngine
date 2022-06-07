#include "stdafx.h"
#include "SpreadBombExplosion.h"

namespace {
	//エフェクトのファイルパス
	const char16_t* EFFECT_BULLET_FILEPATH = u"Assets/effect/shot_spread_burst.efk";
	//生存時間
	const float LIFETIME = 2.0f;
	//ダメージ量
	const float POWER = 5.0f;
	//ダメージ半径
	const float DAMAGE_RADIUS = 200.0f;
	//ダメージを与える間隔
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