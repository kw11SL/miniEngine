#include "stdafx.h"
#include "GameDirector.h"

//Ã“Iƒƒ“ƒo‚Ì‰Šú‰»
GameDirector* GameDirector::m_gameDirector = nullptr;

void GameDirector::ResetGame()
{
	m_score = 0;
	m_playerLife = 3;
	m_waveNumber = 1;
	m_time = 60.0f;
	m_enGameState = enTitle;
}