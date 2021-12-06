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

Game::~Game()
{
	DeleteGO(m_player);
	//DeleteGO(m_enemy);
	DeleteGO(m_bg);
	DeleteGO(m_directionLight);
	DeleteGO(m_pointLight);
	DeleteGO(m_spotLight);
	DeleteGO(m_ui);

	//テスト
	DeleteGO(m_sprite);

	for (int i = 0; i < 3; i++) {
		DeleteGO(m_enemy[i]);
	}

	for (int i = 0; i < 10; i++) {
		DeleteGO(m_enemyGenerator[i]);
	}
}

bool Game::Start()
{
	
	return true;
}

void Game::Init(RenderingEngine& renderingEngine)
{
	m_renderingEngine = &renderingEngine;

	//ディレクションライトの初期化
	m_directionLight = NewGO<DirectionLight>(0, "directionlight");
	m_directionLight->Init({ 1.0f,0.0f,1.0f }, { 0.4f,0.4f,0.4f }, { 0.7f,0.7f,0.7f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//ポイントライトの初期化
	m_pointLight = NewGO<PointLight>(0, "pointlight");
	m_pointLight->Init({ 0.0f,0.0f,50.0f }, { 1.0f,0.0f,0.0f }, 500.0f);

	//スポットライトの初期化
	m_spotLight = NewGO<SpotLight>(0, "spotlight");
	Vector3 spDir = { 0.0f,0.0f,-1.0f };
	spDir.Normalize();
	float spEmitAngle = Math::DegToRad(25.0f);
	m_spotLight->Init({ 0.0f,0.0f,200.0f }, { 2.0f,2.0f,2.0f }, 1000.0f, spDir, spEmitAngle);

	
	//UIの初期化
	m_ui = NewGO<UI>(0, "ui");
	m_ui->Init();


	//レベル構築
	m_level.Init("Assets/level3D/level00.tkl", [&](LevelObjectData& objData) {
		//プレイヤー
		if (objData.EqualObjectName(L"player") == true) {
			m_player = NewGO<Player_new>(0, "player");
			
			m_player->Init(renderingEngine);

			m_player->SetPostion(objData.position);
			m_player->SetRotation(objData.rotation);

			m_player->InitCharaCon();

			//ライトを渡す処理
			if (m_player->GetSkinModelRender() != nullptr) {
				m_player->RecieveDirectionLight(m_directionLight);
				m_player->RecievePointLight(m_pointLight);
				m_player->RecieveSpotLight(m_spotLight);
		
				m_player->InitModelFromInitData();
			}

			return true;
		}

		//エネミー1の生成器
		if (objData.EqualObjectName(L"enemy_01") == true) {
			
			m_enemyGenerator[0] = NewGO<EnemyGenerator>(0, "enemyGenerator01");
			m_enemyGenerator[0]->Init(
				objData.position,
				objData.rotation,
				true,
				enCommon
			);

			return true;
		}

		//エネミー2の生成器
		if (objData.EqualObjectName(L"enemy_02") == true) {
			m_enemyGenerator[1] = NewGO<EnemyGenerator>(0, "enemyGenerator02");
			m_enemyGenerator[1]->Init(
				objData.position,
				objData.rotation,
				true,
				enShot
			);
			return true;
		}

		//エネミー3
		if (objData.EqualObjectName(L"enemy_03") == true) {
			m_enemyGenerator[2] = NewGO<EnemyGenerator>(0, "enemyGenerator03");
			m_enemyGenerator[2]->Init(
				objData.position,
				objData.rotation,
				false,
				enBomb
			);

			return true;
		}

		//ステージ
		if (objData.EqualObjectName(L"stageBg") == true) {
			m_bg = NewGO<BG>(0, "bg");
			m_bg->Init(renderingEngine,objData.position,objData.rotation,Vector3::One);


			//背景にライトを渡す処理
			if (m_bg->GetSkinModelRender() != nullptr) {
				m_bg->RecieveDirectionLight(m_directionLight);
				m_bg->RecievePointLight(m_pointLight);
				m_bg->RecieveSpotLight(m_spotLight);

				m_bg->InitModelFromInitData();
			}
			return true;
		}

		//背景
		if (objData.EqualObjectName(L"backGround") == true) {
			m_stageBackGround = NewGO<StageBackGround>(0, "stageBackGround");
			m_stageBackGround->Init(renderingEngine, objData.position, objData.rotation, Vector3::One);


			//背景にライトを渡す処理
			if (m_stageBackGround->GetSkinModelRender() != nullptr) {
				m_stageBackGround->RecieveDirectionLight(m_directionLight);
				m_stageBackGround->RecievePointLight(m_pointLight);
				m_stageBackGround->RecieveSpotLight(m_spotLight);

				m_stageBackGround->InitModelFromInitData();
			}
			return true;
		}


		//構築終了
		return true;

	});



}

void Game::Update()
{
	//テスト：プレイヤーの削除
	if (g_pad[0]->IsTrigger(enButtonX)) {
		DeleteGO(m_player);
	}


	if (g_pad[0]->IsTrigger(enButtonSelect)) {
		DeleteGO(this);
	}


}