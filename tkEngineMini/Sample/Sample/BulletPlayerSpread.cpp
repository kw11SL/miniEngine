#include "stdafx.h"
#include "BulletPlayerSpread.h"

namespace {
	//エフェクトのファイルパス
	const char16_t* EFFECT_BULLET_FILEPATH = u"Assets/effect/shot_pl_spread.efk";				//弾エフェクトのファイルパス
	const char16_t* EFFECT_BANISH_FILEPATH = u"Assets/effect/bullet_banish_normal.efk";			//消滅エフェクトのファイルパス
	//エフェクトの拡大率
	const Vector3 EFFECT_BULLET_SCALE = { 15.0f,15.0f,15.0f };		//弾エフェクトの拡大率
	const Vector3 EFFECT_BANISH_SCALE = { 4.0f,4.0f,4.0f };			//消滅エフェクトの拡大率
	//爆発に設定するエフェクトの拡大率
	const float BURST_EFFECT_SCALE_RATE = 10.0f;

	//爆発SE
	const wchar_t* BURST_SE_FILEPATH = L"Assets/wav/shot_spread_exp.wav";		//爆発SEのファイルパス
	float BURST_SE_VOLUME = 0.4f;												//設定するボリューム

	//性能
	const float LIFE = 1.0f;							//弾の耐久値
	const float MOVE_SPEED = 15.0f;						//弾速
	const float LIFETIME = 1.5f;						//弾の残存時間
	const float POWER = 1.0f;							//弾の威力
	const float DAMAGE_INTERVAL = 0.15f;				//ダメージを与える間隔(相手に付与する無敵時間)
	const float SPEED_DEC_RATE = 0.15f;					//スプレッドボムの速度減衰
}

BulletPlayerSpread::~BulletPlayerSpread()
{

}

void BulletPlayerSpread::InitSub()
{
	//各種パラメータを設定
	m_life = LIFE;								//耐久力
	m_speed = MOVE_SPEED;						//弾速
	m_lifeTime = LIFETIME;						//時間寿命
	m_power = POWER;							//威力
	m_damageInterval = DAMAGE_INTERVAL;			//ダメージ間隔
}

void BulletPlayerSpread::InitEffectSub()
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

void BulletPlayerSpread::MoveSub()
{
	//速度減衰処理
	m_speed -= SPEED_DEC_RATE;

	//0未満なら0に設定
	if (m_speed < 0.0f) {
		m_speed = 0.0f;
	}
}

void BulletPlayerSpread::RotationSub()
{

}

void BulletPlayerSpread::DestroySub()
{
	//爆発時のSEを再生
	CSoundSource* ssBurst = NewGO<CSoundSource>(0);
	ssBurst->Init(BURST_SE_FILEPATH);
	ssBurst->SetVolume(BURST_SE_VOLUME);
	ssBurst->Play(false);

	//爆発のマネージャー内に爆発を生成
	ExplosionManager::GetInstance()->InitExplosion(
		m_position,
		BURST_EFFECT_SCALE_RATE,
		enPlayer_Spread_Bomb
	);
}

void BulletPlayerSpread::EffectUpdateSub()
{

}

void BulletPlayerSpread::UpdateSub()
{

}