#include "stdafx.h"
#include "GameDirector.h"

//�ÓI�����o�̏�����
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
	////��Ԃ̑J��
	////�^�C���A�b�v�Ń��U���g
	//if (m_time <= 0) {
	//	SetGameState(enResult);
	//}

	////�v���C���[�̃��C�t��0�ŃQ�[���I�[�o�[
	//if (m_playerLife <= 0) {
	//	SetGameState(enGameOver);
	//}
}