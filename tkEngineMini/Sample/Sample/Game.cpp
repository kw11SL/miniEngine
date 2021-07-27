#include "stdafx.h"
#include "Game.h"
#include "Player_new.h"
#include "SkinModelRender.h"
#include "BG.h"
#include "DirectionLight.h"
#include "PointLight.h"

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_bg);
	DeleteGO(m_directionLight);
	DeleteGO(m_pointLight);
}

bool Game::Start()
{
	//�f�B���N�V�������C�g�̏�����
	m_directionLight = NewGO<DirectionLight>(0, "directionlight");
	m_directionLight->Init({ 1.0f,0.0f,1.0f }, { 1.0f,1.0f,1.0f }, { 0.3f,0.3f,0.3f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//�|�C���g���C�g�̏�����
	m_pointLight = NewGO<PointLight>(0, "pointlight");
	m_pointLight->Init({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f }, 500.0f);

	//�v���C���[�̏�����
	m_player = NewGO<Player_new>(0, "player");	
	//�w�i�̏�����
	m_bg = NewGO<BG>(0, "bg");
	
	/*if (m_player->GetSkinModelRender() != nullptr) {
		m_player->InitDirectionLight(m_directionLight);
		m_player->SetIsInitDirLig(true);
	}*/

	//�v���C���[�Ƀ��C�g��n������
	if (m_player->GetSkinModelRender() != nullptr) {
		m_player->InitDirectionLight(m_directionLight);
		m_player->InitPointLight(m_pointLight);
	}

	//�w�i�Ƀ��C�g��n������
	if (m_bg->GetSkinModelRender() != nullptr) {
		m_bg->RecieveDirectionLight(m_directionLight);
		m_bg->RecievePointLight(m_pointLight);
	}
	
	return true;
}

void Game::Update()
{

}