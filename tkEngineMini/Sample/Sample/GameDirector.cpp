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
	m_waveNumberPrev = m_waveNumber;
	m_isSwitchedWave = false;
	m_time = 60.0f;
	m_timeUpToResultCounter = 4.0f;
	m_enGameState = enTitle;
	m_enGameStatePrevFrame = enTitle;

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
	//���t���[���ł̃Q�[���̏�Ԃ��L�^
	m_enGameStatePrevFrame = m_enGameState;
	//wave�����L�^
	m_waveNumberPrev = m_waveNumber;
}

void GameDirector::ExecuteUpdate()
{
	//�O�t���[����wave�̐؂�ւ����������Ă�����t���O�����ɖ߂�
	if(m_isSwitchedWave == true){
		m_isSwitchedWave = false;
	}

	//wave��i�߂鏈��
	if (m_time <= 45.0f && m_waveNumberPrev == 1) {
		AddWaveNumber();
	}
	if (m_time <= 30.0f && m_waveNumberPrev == 2) {
		AddWaveNumber();
	}
	if (m_time <= 10.0f && m_waveNumberPrev == 3) {
		AddWaveNumber();
	}



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

	//���t���[���̏󋵂��L�^
	RecordCurrentFrame();
	
}