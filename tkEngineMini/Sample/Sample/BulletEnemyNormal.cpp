#include "stdafx.h"
#include "BulletEnemyNormal.h"

namespace {
	//エフェクトのファイルパス
	const char16_t* EFFECT_BULLET_FILEPATH = u"Assets/effect/enemy_bullet_1.efk";						//弾エフェクトのファイルパス
	const char16_t* EFFECT_BANISH_FILEPATH = u"Assets/effect/bullet_banish_enemy.efk";			//消滅エフェクトのファイルパス
	//エフェクトの拡大率
	const Vector3 EFFECT_BULLET_SCALE = { 15.0f,15.0f,15.0f };		//弾エフェクトの拡大率
	const Vector3 EFFECT_BANISH_SCALE = { 4.0f,4.0f,4.0f };			//消滅エフェクトの拡大率

	//性能
	const float LIFE = 1.0f;							//弾の耐久値
	const float MOVE_SPEED = 8.0f;						//弾速
	const float LIFETIME = 1.0f;						//弾の残存時間
	const float POWER = 1.0f;							//弾の威力
	const float DAMAGE_INTERVAL = 0.15f;				//ダメージを与える間隔(相手に付与する無敵時間)
}

BulletEnemyNormal::~BulletEnemyNormal()
{

}

void BulletEnemyNormal::InitSub()
{
	//各種パラメータを設定
	m_life = LIFE;								//耐久力
	m_speed = MOVE_SPEED;						//弾速
	m_lifeTime = LIFETIME;						//時間寿命
	m_power = POWER;							//威力
	m_damageInterval = DAMAGE_INTERVAL;			//ダメージ間隔
}

void BulletEnemyNormal::InitEffectSub()
{
	//弾エフェクトの初期化
	m_shotEffect.Init(EFFECT_BULLET_FILEPATH);
	//弾エフェクトの拡大率を設定
	m_shotEffect.SetScale(EFFECT_BULLET_SCALE);

	//消滅エフェクトの初期化
	m_banishEffect.Init(EFFECT_BANISH_FILEPATH);
	//消滅エフェクトの拡大率を設定
	m_banishEffect.SetScale(EFFECT_BANISH_SCALE);
}

void BulletEnemyNormal::MoveSub()
{

}

void BulletEnemyNormal::RotationSub()
{

}

void BulletEnemyNormal::DestroySub()
{
	//消滅エフェクトの位置を設定
	m_banishEffect.SetPosition(m_position);
	//拡大率を設定
	m_banishEffect.SetScale(EFFECT_BANISH_SCALE);
	//再生
	m_banishEffect.Play(false);
}

void BulletEnemyNormal::EffectUpdateSub()
{

}

void BulletEnemyNormal::UpdateSub()
{

}