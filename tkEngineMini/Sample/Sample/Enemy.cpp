#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	//発生したらエネミー数に+1
	GameDirector::GetInstance()->AddEnemyCount();
	//エネミー総数に+1
	GameDirector::GetInstance()->AddEnemyTotalCount();
}

Enemy::~Enemy()
{
	//撃破されたらエネミー数に-1
	GameDirector::GetInstance()->DecEnemyCount();
}

bool Enemy::Start()
{
	return true;
}

void Enemy::Init(
	const Vector3& initPoint,
	const Vector3& initUp,
	const EnEnemyType& enemyType)
{
	//エネミーのタイプを設定
	m_enEnemyType = enemyType;

	//エネミーのタイプごとに基底クラスのオブジェクトに渡す
	//派生クラスのオブジェクトを振り分ける
	switch (m_enEnemyType) {
	//通常エネミー
	case enCommon:
		m_enemyBase = &m_enemyNormal;
		break;
	//射撃型エネミー
	case enShot:
		m_enemyBase = &m_enemyShot;
		break;
	//自爆型エネミー
	case enBomb:
		m_enemyBase = &m_enemyBomb;
		break;
	default:
		break;
	}

	//エネミー基底クラスのオブジェクトを初期化
	m_enemyBase->Init(
		initPoint,
		initUp
	);
}


void Enemy::Update()
{
	//ゲーム中以外なら処理しない
	if(GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//エネミー基底クラスのオブジェクトを更新
	m_enemyBase->Update();
}