#include "stdafx.h"
#include "Game.h"
#include "Player_new.h"
#include "Enemy.h"
#include "BG.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_enemy);
	DeleteGO(m_bg);
	DeleteGO(m_directionLight);
	DeleteGO(m_pointLight);
	DeleteGO(m_spotLight);

	//�e�X�g
	DeleteGO(m_sprite);
}

bool Game::Start()
{
	
	return true;
}

void Game::Init(RenderingEngine& renderingEngine)
{
	//�f�B���N�V�������C�g�̏�����
	m_directionLight = NewGO<DirectionLight>(0, "directionlight");
	m_directionLight->Init({ 1.0f,0.0f,1.0f }, { 0.5f,0.5f,0.5f }, { 0.3f,0.3f,0.3f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//�|�C���g���C�g�̏�����
	m_pointLight = NewGO<PointLight>(0, "pointlight");
	m_pointLight->Init({ 0.0f,0.0f,50.0f }, { 1.0f,0.0f,0.0f }, 500.0f);

	//�X�|�b�g���C�g�̏�����
	m_spotLight = NewGO<SpotLight>(0, "spotlight");
	Vector3 spDir = { 0.0f,0.0f,-1.0f };
	spDir.Normalize();
	float spEmitAngle = Math::DegToRad(25.0f);
	m_spotLight->Init({ 0.0f,0.0f,200.0f }, { 2.0f,2.0f,2.0f }, 1000.0f, spDir, spEmitAngle);

	//�v���C���[�̏�����
	m_player = NewGO<Player_new>(0, "player");
	m_player->Init(renderingEngine);

	//�G�l�~�[�̏�����
	m_enemy = NewGO<Enemy>(0, "enemy");
	m_enemy->Init(renderingEngine, { 0.0f,700.0f,-300.0f });

	//�w�i�̏�����
	m_bg = NewGO<BG>(0, "bg");
	m_bg->Init(renderingEngine);

	//�v���C���[�Ƀ��C�g��n������
	if (m_player->GetSkinModelRender() != nullptr) {
		m_player->RecieveDirectionLight(m_directionLight);
		m_player->RecievePointLight(m_pointLight);
		m_player->RecieveSpotLight(m_spotLight);
		
		m_player->InitModelFromInitData();
	}

	//�G�l�~�[�Ƀ��C�g��n������
	if (m_enemy->GetSkinModelRender() != nullptr) {
		m_enemy->RecieveDirectionLight(m_directionLight);
		m_enemy->RecievePointLight(m_pointLight);
		m_enemy->RecieveSpotLight(m_spotLight);

		m_enemy->InitModelFromInitData();
	}

	//�w�i�Ƀ��C�g��n������
	if (m_bg->GetSkinModelRender() != nullptr) {
		m_bg->RecieveDirectionLight(m_directionLight);
		m_bg->RecievePointLight(m_pointLight);
		m_bg->RecieveSpotLight(m_spotLight);

		m_bg->InitModelFromInitData();
	}

	

	////�X�v���C�g�\���e�X�g
	//m_sprite = NewGO<SpriteRender>(0, "sprite");
	//m_sprite->Init("Assets/sprite/finish.dds",256,256,AlphaBlendMode_Trans);
	//m_sprite->SetColor({ 1.0f,0.0f,0.0f,0.5f });

}

void Game::Update()
{
	//�e�X�g�F�v���C���[�̍폜
	if (g_pad[0]->IsTrigger(enButtonX)) {
		DeleteGO(m_player);
		m_player = nullptr;
	}

	if (m_player != nullptr) {
		//�e�X�g�F���C�g�̎󂯓n��
		if ((g_pad[0]->IsTrigger(enButtonA))) {
			m_player->RecieveDirectionLight(m_directionLight);
			m_player->RecievePointLight(m_pointLight);
			m_player->RecieveSpotLight(m_spotLight);

			m_player->InitModelFromInitData();
		}
	}

}