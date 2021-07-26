#include "stdafx.h"
#include "Game.h"
#include "Player_new.h"
//#include "Player.h"
#include "SkinModelRender.h"
//#include "DirectionLight.h"

Game::~Game()
{

	//DeleteGO(m_player);
	DeleteGO(m_player);
	//DeleteGO(m_skinModelRender);
	DeleteGO(m_directionLight);
}

bool Game::Start()
{
	m_directionLight = NewGO<DirectionLight>(0, "directionlight");
	m_directionLight->Init({ 1.0f,0.0f,1.0f }, { 1.0f,1.0f,1.0f }, { 0.3f,0.3f,0.3f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//m_player = NewGO<Player>(0);

	m_player = NewGO<Player_new>(0, "player");
	
	if (m_player->GetSkinModelRender() != nullptr) {
		m_player->InitDirectionLight(m_directionLight);
	}
	
	//m_skinModelRender = NewGO<SkinModelRender>(0);
	////m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain");
	////m_skinModelRender->InitDirectionLight(m_dirLight);
	//m_skinModelRender->Init("Assets/modelData/unityChan.tkm", enModelUpAxisZ); 
	//m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain");
	//m_skinModelRender->InitDirectionLight(m_directionLight);
	//m_skinModelRender->SetPosition({ 0.0f,0.0f,0.0f });
	

	return true;
}

void Game::Update()
{
	if (m_player->GetSkinModelRender() != nullptr && m_player->GetIsInitDirLig() == false) {
		m_player->InitDirectionLight(m_directionLight);
		m_player->SetIsInitDirLig(true);
	}

	//m_player->InitDirectionLight(m_directionLight);
	//m_skinModelRender->InitDirectionLight(m_directionLight);

}