#include "stdafx.h"
#include "EnemyNormal.h"
#include "Player_new.h"

namespace {
	
	//モデルのファイルパス
	const char* MODELPATH = "Assets/modelData/enemy/enemy_common.tkm";

	//エフェクトのファイルパス
	const char16_t* DESTROY_EFFECT_FILEPATH = u"Assets/effect/destroy.efk";		//撃破エフェクトのファイルパス
	const char16_t* HIT_EFFECT_FILEPATH = u"Assets/effect/hit.efk";				//ヒットエフェクトのファイルパス

	//パラメータ各種
	const float MOVE_SPEED = 5.0f;		//移動速度
	const float LIFE = 1.0f;			//耐久度
	const float DURABILITY = 1.0f;		//弾への強度。数値分だけ弾の耐久度を削る。
	const int SCORE = 100;				//撃破時に加算されるスコア
	const float ACTIVATE_COUNT = 0.7f;	//接触判定が有効になるまでのカウンター
	const float HIT_RANGE = 100.0f;		//当たり判定の範囲
	const float STOP_RANGE = 60.0f;		//移動停止する範囲
	const float RE_MOVE_RANGE = 70.0f;	//移動再開する範囲
}

EnemyNormal::~EnemyNormal()
{
	DeleteGO(m_skinModelRender);
}

void EnemyNormal::InitSub()
{
	//モデルの初期化
	m_skinModelRender->Init(MODELPATH, enModelUpAxisZ, true, false);

	//各種パラメータを設定
	m_life = LIFE;
	m_speed = MOVE_SPEED;
	m_score = SCORE;
	m_durability = DURABILITY;
}

void EnemyNormal::InitEffect()
{
	m_destroyEffect.Init(DESTROY_EFFECT_FILEPATH);
	m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });

	m_hitEffect.Init(HIT_EFFECT_FILEPATH);
	m_hitEffect.SetScale({ 10.0f,10.0f,10.0f });
}

void EnemyNormal::UpdateEffect()
{
	m_destroyEffect.Update();
	m_hitEffect.Update();
}

void EnemyNormal::MoveSub()
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

void EnemyNormal::RotationSub()
{

}

void EnemyNormal::SelfDestroySub()
{

}

void EnemyNormal::UpdateSub()
{

}
