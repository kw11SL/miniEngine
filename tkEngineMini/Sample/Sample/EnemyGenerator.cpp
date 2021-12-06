#include "stdafx.h"
#include "EnemyGenerator.h"
#include<random>

namespace {
	const float BORDER_TIMEUP = 10.0f;						//スポーン周期を早くする時間のボーダー
	const float ENEMY_SPAWN_TIME = 2.0f;					//エネミーのスポーン周期
	const float ENEMY_SPAWN_TIME_NEAR_TIMEUP = 1.5f;		//タイムアップ前のエネミーのスポーン周期
	const float GENERATOR_ACTIVE_COUNT_SHOT = 45.0f;		//射撃型エネミー生成器をアクティブにする時間
	const float GENERATOR_ACTIVE_COUNT_BOMB = 30.0f;		//自爆型エネミー生成器をアクティブにする時間
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
	//残り時間が0以下だったら生成しない
	if (GameDirector::GetInstance()->GetTime() <= 0.0f) {
		return;
	}

	//エネミーの最大数を超えていなかったらスポーン
	if (GameDirector::GetInstance()->GetEnemyCount() <= GameDirector::GetInstance()->GetMaxEnemyNum()) {

		//乱数を生成
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_real_distribution<float> randFloat(0.0f, 0.5f);

		float interval = 0.0f;

		//乱数の値をスポーン周期に加える
		if (GameDirector::GetInstance()->GetTime() <= BORDER_TIMEUP) {
			interval = ENEMY_SPAWN_TIME_NEAR_TIMEUP;
			interval += randFloat(mt);
		}
		else {
			interval = ENEMY_SPAWN_TIME;
			interval += randFloat(mt);
		}

		//エネミーを生成。アクティブになるまでは生成しない。
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
	//射撃型エネミー生成器のとき
	if (m_spawnEnemyType == enShot &&
		GameDirector::GetInstance()->GetTime() <= GENERATOR_ACTIVE_COUNT_SHOT) {
		SetActive(true);
	}

	//生成器がボムのとき
	if (m_spawnEnemyType == enBomb &&
		GameDirector::GetInstance()->GetTime() <= GENERATOR_ACTIVE_COUNT_BOMB) {
		SetActive(true);
	}

	Move();
	Rotation();
	AddCounter();
	GenerateEnemy(m_spawnEnemyType);
}