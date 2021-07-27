#include "stdafx.h"
#include "Game.h"
#include "Player_new.h"
#include "SkinModelRender.h"
#include "DirectionLight.h"
#include "PointLight.h"

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_directionLight);
	DeleteGO(m_pointLight);
}

bool Game::Start()
{
	//ディレクションライトの初期化
	m_directionLight = NewGO<DirectionLight>(0, "directionlight");
	m_directionLight->Init({ 1.0f,0.0f,1.0f }, { 1.0f,1.0f,1.0f }, { 0.3f,0.3f,0.3f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//ポイントライトの初期化
	m_pointLight = NewGO<PointLight>(0, "pointlight");
	m_pointLight->Init({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f }, 500.0f);

	//プレイヤーの初期化
	m_player = NewGO<Player_new>(0, "player");	
	
	/*if (m_player->GetSkinModelRender() != nullptr) {
		m_player->InitDirectionLight(m_directionLight);
		m_player->SetIsInitDirLig(true);
	}*/

	if (m_player->GetSkinModelRender() != nullptr) {
		m_player->InitDirectionLight(m_directionLight);
		m_player->InitPointLight(m_pointLight);
	}
	
	return true;
}

void Game::Update()
{

}