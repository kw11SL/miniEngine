#include "stdafx.h"
#include "EnemyBomb.h"
#include "Player_new.h"

namespace {

	//モデルのファイルパス
	const char* MODELPATH = "Assets/modelData/enemy/enemy_bomb.tkm";

	//エフェクトのファイルパス
	const char16_t* DESTROY_EFFECT_FILEPATH = u"Assets/effect/destroy.efk";			//撃破エフェクトのファイルパス
	const char16_t* HIT_EFFECT_FILEPATH = u"Assets/effect/hit.efk";					//ヒットエフェクトのファイルパス
	const char16_t* LIFE_EFFECT_FILEPATH = u"Assets/effect/enemy_life_ring.efk";	//時間寿命エフェクトのファイルパス
	
	//エフェクトのスケールなど
	const Vector3 EFFECT_DESTROY_SCALE = { 20.0f,20.0f,20.0f };						//撃破エフェクトのスケール
	const Vector3 EFFECT_HIT_SCALE = { 10.0f,10.0f,10.0f };							//ヒットエフェクトのスケール
	const Vector3 EFFECT_LIFE_INIT_SCALE = { 6.0f,6.0f,6.0f };						//時間寿命エフェクトのスケール(開始)
	const Vector3 EFFECT_LIFE_END_SCALE = { 1.0f,1.0f,1.0f };						//時間寿命エフェクトのスケール(終了)
	const float EFFECT_BOMB_SCALE_RATE = 10.0f;										//爆発エフェクトのスケール

	//パラメータ各種
	const float MOVE_SPEED = 3.0f;		//移動速度
	const float LIFE = 15.0f;			//耐久度
	const float DURABILITY = 1.0f;		//弾への強度。数値分だけ弾の耐久度を削る。
	const int SCORE = 1000;				//撃破時に加算されるスコア
	const float ACTIVATE_COUNT = 0.7f;	//接触判定が有効になるまでのカウンター
	const float HIT_RANGE = 100.0f;		//当たり判定の範囲
	const float STOP_RANGE = 60.0f;		//移動停止する範囲
	const float RE_MOVE_RANGE = 70.0f;	//移動再開する範囲
	const float LIFE_TIME_BOMB = 5.0f;	//時間寿命
	const int BULLET_WAY_NUM = 6;		//自爆時にばらまく弾の数

}

EnemyBomb::~EnemyBomb()
{
	//モデルの削除
	DeleteGO(m_skinModelRender);
	//時間寿命エフェクトの停止
	m_lifeRingEffect.Stop();
}

void EnemyBomb::InitSub()
{
	//モデルをNewGO
	//m_skinModelRender = NewGO<SkinModelRender>(0);
	
	//モデルの初期化
	if (m_skinModelRender != nullptr) {
		m_skinModelRender->Init(MODELPATH, enModelUpAxisZ, true, false);
	}

	//各種パラメータを設定
	m_life = LIFE;
	m_speed = MOVE_SPEED;
	m_score = SCORE;
	m_durability = DURABILITY;
	m_lifeTime = LIFE_TIME_BOMB;

	//初期化時、寿命エフェクトを再生開始
	m_lifeRingEffect.SetPosition(m_position);
	m_lifeRingEffect.SetRotation(m_rot);
	m_lifeRingEffect.SetScale(EFFECT_LIFE_INIT_SCALE);
	m_lifeRingEffect.Play(false);
}

void EnemyBomb::InitEffect()
{
	//撃破エフェクトの初期化
	m_destroyEffect.Init(DESTROY_EFFECT_FILEPATH);
	m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });
	
	//ヒットエフェクトの初期化
	m_hitEffect.Init(HIT_EFFECT_FILEPATH);
	m_hitEffect.SetScale({ 10.0f,10.0f,10.0f });
	
	//時間寿命エフェクトの初期化
	m_lifeRingEffect.Init(LIFE_EFFECT_FILEPATH);
	m_lifeRingEffect.SetScale(EFFECT_LIFE_INIT_SCALE);
}

void EnemyBomb::UpdateEffect()
{
	m_destroyEffect.Update();
	m_hitEffect.Update();
	m_lifeRingEffect.Update();
}

void EnemyBomb::MoveSub()
{
	//プレイヤー追尾処理
	Vector3 toPlayer = Vector3::Zero;
	float toPlayerLength = 0.0f;

	//プレイヤーを検索
	if (m_player == nullptr) {
		m_player = FindGO<Player_new>(PLAYER_NAME);
	}

	//プレイヤーへのベクトルを取り、その方向を移動方向にする
	if (m_player != nullptr) {
		toPlayer = m_player->GetPosition() - m_position;
		toPlayerLength = toPlayer.Length();
		toPlayer.Normalize();

		m_moveSpeed = toPlayer * m_speed;
	}

	Vector3 playerToMe = m_position - m_player->GetPosition();

	//プレイヤーの位置までのベクトルを取り、一定距離まで近づくと速度を0にする
	if (toPlayerLength < STOP_RANGE) {
		m_speed = 0.0f;
	}
	//離れると移動再開
	else if (toPlayerLength >= RE_MOVE_RANGE) {
		m_speed = MOVE_SPEED;
	}
}

void EnemyBomb::RotationSub()
{

}

void EnemyBomb::SelfDestroySub()
{
	//爆発を発生させる
	ExplosionManager::GetInstance()->InitExplosion(
		m_position,
		10.0f,
		enEnemy_Explosion
	);

	//自爆時に弾をばらまく
	FireBulletEqually(BULLET_WAY_NUM,enEnemyNormal);
}

void EnemyBomb::LifeRingScaling()
{
	//拡大率
	Vector3 scale;
	//現在の時間寿命 / 時間寿命の初期値 で0.0～1.0の補間率を出す
	float fraction = m_lifeTime / LIFE_TIME_BOMB;
	//出した補間率で拡大率を最大値から最小値まで補間
	scale.Lerp(fraction, EFFECT_LIFE_END_SCALE, EFFECT_LIFE_INIT_SCALE);
	
	//座標、回転、拡大率を設定
	m_lifeRingEffect.SetPosition(m_position);
	m_lifeRingEffect.SetRotation(m_rot);
	m_lifeRingEffect.SetScale(scale);
	
	//エフェクトを再生
	if (m_lifeRingEffect.IsPlay() != true) {
		m_lifeRingEffect.Play();
	}
}

void EnemyBomb::UpdateSub()
{
	//時間寿命を減少
	DecLifeTime();
	//時間寿命エフェクトを縮小
	LifeRingScaling();
}