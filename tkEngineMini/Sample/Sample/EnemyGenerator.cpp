#include "stdafx.h"
#include "EnemyGenerator.h"

namespace {
	const float ENEMY_SPAWN_TIME = 2.0f;
}

EnemyGenerator::~EnemyGenerator()
{
	DeleteGO(m_enemy);
}

void EnemyGenerator::Init(const Vector3& pos, const Quaternion& rot,const EnEnemyType& enemyType)
{
	m_position = pos;
	m_rotation = rot;
	m_spawnEnemyType = enemyType;

	m_myCharaCon.Init(m_position);
	m_sphericalMove.Init(m_forward, m_right, m_up);

}

void EnemyGenerator::GenerateEnemy(const EnEnemyType& enemyType)
{
	if (GameDirector::GetInstance()->GetEnemyCount() <= GameDirector::GetInstance()->GetMaxEnemyNum()) {

		//エネミーを生成
		if (m_spawnCounter > ENEMY_SPAWN_TIME) {
			m_enemy = NewGO<Enemy>(0, "enemy");
			
			m_enemy->Init(
				*RenderingEngine::GetInstance(),
				m_position,
				m_up,
				enemyType);

			//カウンターを0にリセット
			m_spawnCounter = 0.0f;
		}
	}
	else
	{
		m_spawnCounter = 0.0f;
	}

}

void EnemyGenerator::Move()
{
	m_myCharaCon.Execute(m_moveSpeed, m_downVector, 0.0f);
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);
}

void EnemyGenerator::Rotation()
{
	m_sphericalMove.Rotation(m_forward,m_right,m_up,m_rotation);
}

void EnemyGenerator::AddCounter()
{
	m_spawnCounter += g_gameTime->GetFrameDeltaTime();
}

void EnemyGenerator::Update()
{
	Move();
	Rotation();
	AddCounter();
	GenerateEnemy(m_spawnEnemyType);
}