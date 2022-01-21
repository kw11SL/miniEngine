#include "stdafx.h"
#include "GameDirector.h"

//�ÓI�����o�̏�����
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
	//��Ԃ̑J��
	//�^�C���A�b�v�Ń��U���g
	if (m_time <= 0) {
		SetGameState(enResult);
	}

	////�c����0�Ń^�C���A�b�v
	//if (m_time <= 0) {
	//	SetGameState(enTimeUp);
	//}
	//if (m_enGameState == enTimeUp) {
	//	//�^�C���A�b�v���莞�ԂŃ��U���g�ɑJ��
	//	m_timeUpToResultCounter -= g_gameTime->GetFrameDeltaTime();
	//	if (m_timeUpToResultCounter <= 0.0f) {
	//		SetGameState(enResult);
	//	}
	//}

	//�v���C���[�̃��C�t��0�ŃQ�[���I�[�o�[
	if (m_playerLife <= 0) {
		SetGameState(enGameOver);
	}

	//���t���[���ł̃Q�[���̏�Ԃ��L�^
	m_enGameStatePrevFrame = m_enGameState;
}