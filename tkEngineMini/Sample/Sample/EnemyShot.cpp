#include "stdafx.h"
#include "EnemyShot.h"
#include "Player_new.h"

namespace {

	//モデルのファイルパス
	const char* MODELPATH = "Assets/modelData/enemy/enemy_shot.tkm";

	//エフェクトのファイルパス
	const char16_t* DESTROY_EFFECT_FILEPATH = u"Assets/effect/destroy.efk";					//撃破エフェクトのファイルパス
	const char16_t* HIT_EFFECT_FILEPATH = u"Assets/effect/hit.efk";							//ヒットエフェクトのファイルパス
	const char16_t* SHOT_NOTICE_EFFECT_FILEPATH = u"Assets/effect/enemy_shot_notice.efk";	//弾を撃つ予告エフェクトのファイルパス

	//エフェクトのスケール
	const Vector3 EFFECT_DESTROY_SCALE = { 20.0f,20.0f,20.0f };					//撃破エフェクトのスケール
	const Vector3 EFFECT_HIT_SCALE = { 10.0f,10.0f,10.0f };						//ヒットエフェクトのスケール
	const Vector3 EFFECT_SHOT_NOTICE_INIT_SCALE = { 15.0f,15.0f,15.0f };		//射撃予兆エフェクトのスケール(開始)
	const Vector3 EFFECT_SHOT_NOTICE_END_SCALE = { 1.0f,1.0f,1.0f };			//射撃予兆エフェクトのスケール(終了)

	//パラメータ各種
	const float MOVE_SPEED = 2.0f;		//移動速度
	const float LIFE = 1.0f;			//耐久度
	const float DURABILITY = 1.0f;		//弾への強度。数値分だけ弾の耐久度を削る。
	const int SCORE = 300;				//撃破時に加算されるスコア
	const float ACTIVATE_COUNT = 0.7f;	//接触判定が有効になるまでのカウンター
	const float HIT_RANGE = 100.0f;		//当たり判定の範囲
	const float STOP_RANGE = 60.0f;		//移動停止する範囲
	const float RE_MOVE_RANGE = 70.0f;	//移動再開する範囲
	const float SHOT_COUNTER = 2.0f;	//射撃間隔
	const int SHOT_WAY_NUM = 1;			//1度の発射弾数

	const float EFFECT_SHOT_NOTICE_ACTIVATE_TIME = 1.7f;	//射撃直前のエフェクトを発生させるタイミング

}

EnemyShot::~EnemyShot()
{
	//モデルの削除
	DeleteGO(m_skinModelRender);
	//予兆エフェクトの停止
	m_shotNoticeEffect.Stop();
}

void EnemyShot::InitSub()
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
}

void EnemyShot::InitEffect()
{
	//撃破エフェクトの初期化
	m_destroyEffect.Init(DESTROY_EFFECT_FILEPATH);
	m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });
	
	//ヒットエフェクトの初期化
	m_hitEffect.Init(HIT_EFFECT_FILEPATH);
	m_hitEffect.SetScale({ 10.0f,10.0f,10.0f });
	
	//射撃の予兆エフェクトの初期化
	m_shotNoticeEffect.Init(SHOT_NOTICE_EFFECT_FILEPATH);
	m_shotNoticeEffect.SetScale(EFFECT_SHOT_NOTICE_INIT_SCALE);
}

void EnemyShot::UpdateEffect()
{
	m_destroyEffect.Update();
	m_hitEffect.Update();
	m_shotNoticeEffect.Update();
}

void EnemyShot::MoveSub()
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

void EnemyShot::RotationSub()
{

}

void EnemyShot::SelfDestroySub()
{

}

void EnemyShot::ShotNoticeScaling(const float activateTime)
{
	//拡大率
	Vector3 scale;
	//縮小を開始する時間を設定
	const float remain = activateTime;
	
	//スケーリングに使う補間率を計算
	//例:射撃カウンタ:2.0 再生開始時間:1.7;　のとき
	//2.0 - 1.7 = 0.3sec で スケーリング
	float fraction = (m_shotCounter - remain) / (SHOT_COUNTER - remain);
	
	//射撃カウンタ変数が再生開始時間に達していない
	//(=補間率が負の値)場合、補間率は0
	if (m_shotCounter < remain) {
		fraction = 0.0f;
	}

	//拡大率を線形補完、スケーリング
	scale.Lerp(fraction, EFFECT_SHOT_NOTICE_INIT_SCALE, EFFECT_SHOT_NOTICE_END_SCALE);
	m_shotNoticeEffect.SetPosition(m_position);
	m_shotNoticeEffect.SetRotation(m_rot);
	m_shotNoticeEffect.SetScale(scale);


	if (m_shotCounter >= remain && m_shotCounter < SHOT_COUNTER) {
		//再生中でなければ再生
		if (m_shotNoticeEffect.IsPlay() == false) {
			m_shotNoticeEffect.Play();
		}
	}

	//ショットカウンターが0になっていれば再生を停止
	else if (m_shotCounter == 0.0f) {
		m_shotNoticeEffect.Stop();
	}
}

void EnemyShot::UpdateSub()
{
	//射撃カウンタを増加
	AddShotCounter();
	
	//カウンター一定値で射撃(射撃後は射撃カウンタが0になる)
	if (m_shotCounter >= SHOT_COUNTER) {
		FireBulletEqually(SHOT_WAY_NUM, enEnemyNormal);
	}

	//発射予兆エフェクトを発生(射撃カウンタ0を判定に使うため)
	ShotNoticeScaling(EFFECT_SHOT_NOTICE_ACTIVATE_TIME);
}