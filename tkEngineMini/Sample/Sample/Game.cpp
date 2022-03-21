#include "stdafx.h"
#include "Game.h"
#include "Player_new.h"
#include "Enemy.h"
#include "BG.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "UI.h"
#include "EnemyGenerator.h"
#include "Bullet.h"
#include "RenderingEngine.h"
#include "StageBackGround.h"
#include "SkyCube.h"


namespace {
	const char* CAPSULE_LEVEL_FILEPATH = "Assets/level3D/level00_a.tkl";	//���x���̃t�@�C���p�X
}

Game::Game()
{
}

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_bg);
	DeleteGO(m_ui);
	DeleteGO(m_skyCube);
	DeleteGO(m_stageBackGround);
	DeleteGenerators();
	DeleteGO(m_directionLight);
	DeleteGO(m_pointLight);
	DeleteGO(m_spotLight);
}

bool Game::Start()
{
	//�f�B���N�V�������C�g�̏�����
	m_directionLight = NewGO<DirectionLight>(0, DIRECTION_LIGHT_NAME);
	m_directionLight->Init({ 1.0f,0.0f,0.0f }/*g_camera3D->GetPosition() - g_camera3D->GetTarget()*/, { 0.25f,0.25f,0.25f }, { 0.7f,0.7f,0.7f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//�|�C���g���C�g�̏�����
	m_pointLight = NewGO<PointLight>(0, POINT_LIGHT_NAME);
	m_pointLight->Init({ 0.0f,0.0f,100.0f }, { 2.4f,2.2f,2.0f }, 1000.0f);

	//�X�|�b�g���C�g�̏�����
	m_spotLight = NewGO<SpotLight>(0, SPOT_LIGHT_NAME);
	Vector3 spDir = { 0.0f,0.0f,-1.0f };
	spDir.Normalize();
	float spEmitAngle = Math::DegToRad(90.0f);
	m_spotLight->Init({ 0.0f,0.0f,200.0f }, { 0.0f,1.0f,0.0f }, 1000.0f, spDir, spEmitAngle);

	//UI�̏�����
	m_ui = NewGO<UI>(0, "ui");
	m_ui->Init();

	//�X�J�C�L���[�u�̏�����
	m_skyCube = NewGO<SkyCube>(0, SKYCUBE_NAME);
	m_skyCube->Init();


	//���x���\�z
	m_level.Init(CAPSULE_LEVEL_FILEPATH, [&](LevelObjectData& objData) {
		
		//�v���C���[
		if (objData.EqualObjectName(L"player") == true) {
			m_player = NewGO<Player_new>(0, PLAYER_NAME);
			m_player->Init();

			m_player->SetPostion(objData.position);
			m_player->SetRotation(objData.rotation);
			////�v���C���[�̑O���A�E�A��x�N�g���Ƀ��x���̉�]��K�p
			//m_player->SetVectorFromQuaternion(objData.rotation);
			m_player->InitCharaCon();

			return true;
		}

		//�G�l�~�[1�̐�����
		if (objData.EqualObjectName(L"enemy_01") == true) {
			m_enemyGenerators.push_back(NewGO<EnemyGenerator>(0, "enemyGenerator01"));
			m_enemyGenerators[m_enemyGenerators.size() - 1]->Init(
				objData.position,
				objData.rotation,
				true,
				enCommon
			);

			//�G������̑O���A�E�A��x�N�g���Ƀ��x���̉�]��K�p
			m_enemyGenerators[m_enemyGenerators.size() - 1]->SetVectorFromQuaternion(objData.rotation);

			return true;
		}

		//�G�l�~�[2�̐�����
		if (objData.EqualObjectName(L"enemy_02") == true) {
			m_enemyGenerators.push_back(NewGO<EnemyGenerator>(0, "enemyGenerator02"));
			m_enemyGenerators[m_enemyGenerators.size() - 1]->Init(
				objData.position,
				objData.rotation,
				false,
				enShot
			);

			//�G������̑O���A�E�A��x�N�g���Ƀ��x���̉�]��K�p
			m_enemyGenerators[m_enemyGenerators.size() - 1]->SetVectorFromQuaternion(objData.rotation);

			return true;
		}

		//�G�l�~�[3�̐�����
		if (objData.EqualObjectName(L"enemy_03") == true) {
			m_enemyGenerators.push_back(NewGO<EnemyGenerator>(0, "enemyGenerator03"));
			m_enemyGenerators[m_enemyGenerators.size() - 1]->Init(
				objData.position,
				objData.rotation,
				false,
				enBomb
			);

			//�G������̑O���A�E�A��x�N�g���Ƀ��x���̉�]��K�p
			m_enemyGenerators[m_enemyGenerators.size() - 1]->SetVectorFromQuaternion(objData.rotation);

			return true;
		}

		//�X�e�[�W
		if (objData.EqualObjectName(L"stageBg") == true) {
			m_bg = NewGO<BG>(0, STAGE_NAME);

			m_bg->Init(
				objData.position,
				objData.rotation,
				Vector3::One
			);

			return true;
		}

		//�w�i
		if (objData.EqualObjectName(L"backGround") == true) {
			m_stageBackGround = NewGO<StageBackGround>(0, STAGE_BACK_GROUND_NAME);
			
			m_stageBackGround->Init(
				objData.position,
				objData.rotation,
				Vector3::One);

			return true;
		}

		//�\�z�I��
		return true;
	});

	return true;
}

void Game::Init()
{
}

void Game::DeleteGenerators()
{
	for (auto& generator : m_enemyGenerators) {
		DeleteGO(generator);
	}
}

void Game::Update()
{
	//�|�C���g���C�g�̈ʒu���v���C���[�ʒu�ɐݒ�
	m_pointLight->SetPosition(m_player->GetPosition() + m_player->GetUp()*100.0f);
	
	/*m_spotLight->SetPosition(m_player->GetPosition() + m_player->GetUp() * 200.0f);
	m_spotLight->SetDirection(m_player->GetPosition() - m_spotLight->GetPosition());*/

	/*m_spotLight->SetPosition(m_player->GetPosition() + m_player->GetUp() * 20.0f);
	m_spotLight->SetDirection(m_player->GetPosition() - m_spotLight->GetPosition());*/


}