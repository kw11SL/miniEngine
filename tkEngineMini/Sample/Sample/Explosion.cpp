#include "stdafx.h"
#include "Explosion.h"

void Explosion::Init(
	const Vector3& pos,
	const float scaleRate,
	const EnExplosionType& explosionType
	)
{
	//指定された爆発のタイプで基底クラスに入れるオブジェクトを振り分け
	switch (explosionType) {
	case enPlayer_Spread_Bomb:
		m_explosionBase = &m_sBomb;
		break;
	case enEnemy_Explosion:
		m_explosionBase = &m_enemyExplosion;
		break;
	default:
		break;
	}


	//座標、拡大率を設定し、初期化
	m_explosionBase->Init(
		pos,
		scaleRate
	);
}

bool Explosion::Start()
{
	return true;
}

void Explosion::Update()
{
	m_explosionBase->Update();

	if (m_explosionBase->GetIsExist() == false) {
		DeleteGO(this);
	}

}