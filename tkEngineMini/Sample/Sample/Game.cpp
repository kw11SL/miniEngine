#include "stdafx.h"
#include "Game.h"
#include "Player_new.h"
#include "BG.h"
#include "Direction.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

Game::~Game()
{
	DeleteGO(m_player);
	//DeleteGO(m_player2);
	DeleteGO(m_bg);
	//DeleteGO(m_direction);
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
	m_directionLight->Init({ 1.0f,0.0f,1.0f }, { 1.0f,1.0f,1.0f }, { 0.3f,0.3f,0.3f });
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

	/*m_player2 = NewGO<Player_new>(0, "player");
	m_player2->Init(renderingEngine);
	m_player2->SetPostion({ 200.0f,0.0f,-200.0f });*/


	//�w�i�̏�����
	m_bg = NewGO<BG>(0, "bg");
	m_bg->Init(renderingEngine);

	////�����w���̏�����
	//m_direction = NewGO<Direction>(0, "direction");
	//m_direction->SetScale({ 1.0f,1.0f,1.0f });

	//�v���C���[�Ƀ��C�g��n������
	if (m_player->GetSkinModelRender() != nullptr) {
		m_player->RecieveDirectionLight(m_directionLight);
		m_player->RecievePointLight(m_pointLight);
		m_player->RecieveSpotLight(m_spotLight);
		
		m_player->InitModelFromInitData();
	}

	/*if (m_player2->GetSkinModelRender() != nullptr) {
		m_player2->RecieveDirectionLight(m_directionLight);
		m_player2->RecievePointLight(m_pointLight);
		m_player2->RecieveSpotLight(m_spotLight);

		m_player2->InitModelFromInitData();
	}*/

	//�w�i�Ƀ��C�g��n������
	if (m_bg->GetSkinModelRender() != nullptr) {
		m_bg->RecieveDirectionLight(m_directionLight);
		m_bg->RecievePointLight(m_pointLight);
		m_bg->RecieveSpotLight(m_spotLight);

		m_bg->InitModelFromInitData();
	}

	////�����\�����f���Ƀ��C�g��n������
	//if (m_direction->GetSkinModelRender() != nullptr) {
	//	m_direction->RecieveDirectionLight(m_directionLight);
	//	//m_direction->RecievePointLight(m_pointLight);
	//	m_direction->RecieveSpotLight(m_spotLight);
	//}

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
	}

	//�e�X�g�F���C�g�̎󂯓n��
	if ((g_pad[0]->IsTrigger(enButtonA))) {
		m_player->RecieveDirectionLight(m_directionLight);
		m_player->RecievePointLight(m_pointLight);
		m_player->RecieveSpotLight(m_spotLight);

		m_player->InitModelFromInitData();
	}

}