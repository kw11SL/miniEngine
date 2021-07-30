#include "stdafx.h"
#include "Game.h"
#include "Player_new.h"
#include "SkinModelRender.h"
#include "BG.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_bg);
	DeleteGO(m_directionLight);
	DeleteGO(m_pointLight);
	DeleteGO(m_spotLight);
}

bool Game::Start()
{
	//ディレクションライトの初期化
	m_directionLight = NewGO<DirectionLight>(0, "directionlight");
	m_directionLight->Init({ 1.0f,0.0f,1.0f }, { 1.0f,1.0f,1.0f }, { 0.3f,0.3f,0.3f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//ポイントライトの初期化
	m_pointLight = NewGO<PointLight>(0, "pointlight");
	m_pointLight->Init({ 0.0f,0.0f,0.0f }, { 0.5f,0.0f,0.0f }, 300.0f);

	//スポットライトの初期化
	m_spotLight = NewGO<SpotLight>(0, "spotLight");
	m_spotLight->Init({ 0.0f,100.0f,0.0f }, { 0.0f,1.0f,0.0f }, 10.0f, { 0.0f,-1.0f,0.0f }, 2.0f);


	//プレイヤーの初期化
	m_player = NewGO<Player_new>(0, "player");	
	//背景の初期化
	m_bg = NewGO<BG>(0, "bg");
	
	/*if (m_player->GetSkinModelRender() != nullptr) {
		m_player->InitDirectionLight(m_directionLight);
		m_player->SetIsInitDirLig(true);
	}*/

	//プレイヤーにライトを渡す処理
	if (m_player->GetSkinModelRender() != nullptr) {
		m_player->RecieveDirectionLight(m_directionLight);
		m_player->RecievePointLight(m_pointLight);
		//m_player->RecieveSpotLight(m_spotLight);
	}

	//背景にライトを渡す処理
	if (m_bg->GetSkinModelRender() != nullptr) {
		m_bg->RecieveDirectionLight(m_directionLight);
		m_bg->RecievePointLight(m_pointLight);
		//m_bg->RecieveSpotLight(m_spotLight);
	}
	
	return true;
}

void Game::Update()
{

}