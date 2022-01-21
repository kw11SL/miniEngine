#include "stdafx.h"
#include "GameDirector.h"

//静的メンバの初期化
GameDirector* GameDirector::m_gameDirector = nullptr;

void GameDirector::ResetGame()
{
	m_score = 0;
	m_finalScore = 0;
	m_playerLife = 3;
	m_enemyNum = 0;
	m_totalSpawnEnemyNum = 0;
	m_totalDestroyedEnemyNum = 0;
	m_destructionRate = 0.0f;
	m_waveNumber = 1;
	m_time = 60.0f;
	m_timeUpToResultCounter = 4.0f;
	m_enGameState = enTitle;

}

const int GameDirector::CalcDestructionBonus()
{
	if (CalcDestructionRate() >= 100.0f){
		return 100000;
	}
	if (CalcDestructionRate() >= 80.0f && CalcDestructionRate() < 100.0f) {
		return 50000;
	}
	if (CalcDestructionRate() >= 50.0f && CalcDestructionRate() < 100.0f) {
		return 10000;
	}
	if (CalcDestructionRate() > 0.0f && CalcDestructionRate() < 100.0f) {
		return 5000;
	}
	if (CalcDestructionRate() == 0.0f) {
		return 100000;
	}
}

const int GameDirector::CalcFinalScore()
{
	m_finalScore = m_score;
	m_finalScore += CalcRemainBonus();
	m_finalScore += CalcDestructionBonus();

	return m_finalScore;
}


void GameDirector::ExecuteUpdate()
{
	//状態の遷移
	//タイムアップでリザルト
	if (m_time <= 0) {
		SetGameState(enResult);
	}

	////残時間0でタイムアップ
	//if (m_time <= 0) {
	//	SetGameState(enTimeUp);
	//}
	//if (m_enGameState == enTimeUp) {
	//	//タイムアップ後一定時間でリザルトに遷移
	//	m_timeUpToResultCounter -= g_gameTime->GetFrameDeltaTime();
	//	if (m_timeUpToResultCounter <= 0.0f) {
	//		SetGameState(enResult);
	//	}
	//}

	//プレイヤーのライフが0でゲームオーバー
	if (m_playerLife <= 0) {
		SetGameState(enGameOver);
	}

	//現フレームでのゲームの状態を記録
	m_enGameStatePrevFrame = m_enGameState;
}