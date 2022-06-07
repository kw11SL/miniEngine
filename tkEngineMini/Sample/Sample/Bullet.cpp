#include "stdafx.h"
#include "Bullet.h"
#include "ExplosionManager.h"

namespace{
	
}


Bullet::~Bullet()
{
	
}

bool Bullet::Start()
{
	return true;
}

void Bullet::Init(
	const Vector3& initPoint, 
	const Vector3& initUp,
	const Vector3& direction, 
	const EnBulletType& bulletType)
{
	
	//指定したタイプによって弾の基底クラスに入れる派生クラスのオブジェクトを振り分け
	switch (bulletType){
	case enPlayerNormal:
		m_bulletBase = &m_bulletPlayerNormal;
		break;

	case enPlayerSpreadBomb:
		m_bulletBase = &m_bulletPlayerSpread;
		break;

	case enEnemyNormal:
		m_bulletBase = &m_bulletEnemyNormal;
		break;

	default:
		break;
	};

	//弾のベースクラスの初期化
	m_bulletBase->Init(
		initPoint,
		initUp,
		direction
	);

}

void Bullet::Update()
{
	//ゲーム中以外なら処理しない
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//弾の基底クラスの更新
	m_bulletBase->Update();

}