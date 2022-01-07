#include "stdafx.h"
#include "GameDirector.h"

//静的メンバの初期化
GameDirector* GameDirector::m_gameDirector = nullptr;

void GameDirector::ResetGame()
{
	m_score = 0;
	m_playerLife = 3;
	m_enemyNum = 0;
	m_totalSpawnEnemyNum = 0;
	m_totalDestroyedEnemyNum = 0;
	m_waveNumber = 1;
	m_time = 60.0f;
	m_enGameState = enTitle;

}

void GameDirector::ExecuteUpdate()
{
	////状態の遷移
	////タイムアップでリザルト
	//if (m_time <= 0) {
	//	SetGameState(enResult);
	//}

	////プレイヤーのライフが0でゲームオーバー
	//if (m_playerLife <= 0) {
	//	SetGameState(enGameOver);
	//}
}