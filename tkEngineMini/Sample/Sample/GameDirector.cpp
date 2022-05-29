#include "stdafx.h"
#include "GameDirector.h"

namespace{
	//タイムアップ前のラッシュタイム
	const float NEAR_TIMEUP = 10.0f;
}

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
	m_waveNumberPrev = m_waveNumber;
	m_isSwitchedWave = false;
	m_time = 60.0f;
	m_startToGameCounter = 3.0f;
	m_timeUpToResultCounter = 4.0f;
	m_enGameState = enTitle;
	m_enGameStatePrevFrame = enTitle;
	m_maxEnemy = MAX_ENEMY_NUM;
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
	if (CalcDestructionRate() <= 0.0f) {
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

void GameDirector::RecordCurrentFrame()
{
	//現フレームでのゲームの状態を記録
	m_enGameStatePrevFrame = m_enGameState;
	//wave数を記録
	m_waveNumberPrev = m_waveNumber;
}

void GameDirector::ChangeMaxEnemy()
{
	//制限時間が迫ると敵の最大数を増やす
	if (m_time <= NEAR_TIMEUP) {
		m_maxEnemy = MAX_ENEMY_NUM_NEAR_TIMEUP;
	}
}

void GameDirector::ExecuteUpdate()
{
	//ゲーム中、時間を減少
	if (GetGameState() == enGame) {
		DecTime();
	}

	//前フレームでwaveの切り替えが発生していたらフラグを元に戻す
	if(m_isSwitchedWave == true){
		m_isSwitchedWave = false;
	}

	//waveを進める処理
	if (m_time <= 45.0f && m_waveNumberPrev == 1) {
		AddWaveNumber();
	}
	if (m_time <= 30.0f && m_waveNumberPrev == 2) {
		AddWaveNumber();
	}
	if (m_time <= 10.0f && m_waveNumberPrev == 3) {
		AddWaveNumber();
	}

	//状態の遷移
	
	//ゲーム開始時に一定時間たつとゲームに移行する
	if (m_enGameState == enStart) {
		m_startToGameCounter -= g_gameTime->GetFrameDeltaTime();
		if (m_startToGameCounter <= 0.0f) {
			SetGameState(enGame);
		}
	}

	//タイムアップでリザルト
	if (m_time <= 0.0f) {
		SetGameState(enResult);
	}

	//プレイヤーのライフが0でゲームオーバー
	if (m_playerLife <= 0) {
		SetGameState(enGameOver);
	}

	//タイムアップ間近でエネミー最大数を増やす
	ChangeMaxEnemy();

	//現フレームの状況を記録
	RecordCurrentFrame();
	
}