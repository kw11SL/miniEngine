#include "stdafx.h"
#include "EnemyGenerator.h"
#include<random>

namespace {
	const float ENEMY_SPAWN_TIME = 2.0f;
	const float GENERATOR_AVTIVE_COUNT = 45.0f;
}

EnemyGenerator::~EnemyGenerator()
{
	DeleteGO(m_enemy);
}

void EnemyGenerator::Init(const Vector3& pos, const Quaternion& rot, const bool isActive ,const EnEnemyType& enemyType)
{
	m_position = pos;
	m_rotation = rot;
	m_spawnEnemyType = enemyType;

	//初期化時にアクティブにするかどうかを選択
	SetActive(isActive);

	m_myCharaCon.Init(m_position);
	m_sphericalMove.Init(m_forward, m_right, m_up);

}

void EnemyGenerator::GenerateEnemy(const EnEnemyType& enemyType)
{
	if (GameDirector::GetInstance()->GetEnemyCount() <= GameDirector::GetInstance()->GetMaxEnemyNum()) {

		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_real_distribution<float> randFloat(0.0f, 0.5f);

		float interval = ENEMY_SPAWN_TIME;
		interval += randFloat(mt);

		//エネミーを生成
		if (m_spawnCounter > interval && m_isActive == true) {
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

	//生成器がボムのとき
	if (m_spawnEnemyType == enBomb && 
		GameDirector::GetInstance()->GetTime() <= GENERATOR_AVTIVE_COUNT) {
		SetActive(true);
	}

}