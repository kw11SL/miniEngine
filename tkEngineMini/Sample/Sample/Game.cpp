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
	//ディレクションライトの初期化
	m_directionLight = NewGO<DirectionLight>(0, "directionlight");
	m_directionLight->Init(/*{ 1.0f,0.0f,1.0f }*/g_camera3D->GetPosition() - g_camera3D->GetTarget(), { 0.25f,0.25f,0.25f }, { 0.7f,0.7f,0.7f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//ポイントライトの初期化
	m_pointLight = NewGO<PointLight>(0, "pointlight");
	m_pointLight->Init({ 0.0f,0.0f,100.0f }, { 1.0f,0.0f,0.0f }, 500.0f);

	//スポットライトの初期化
	m_spotLight = NewGO<SpotLight>(0, "spotlight");
	Vector3 spDir = { 0.0f,0.0f,-1.0f };
	spDir.Normalize();
	float spEmitAngle = Math::DegToRad(25.0f);
	m_spotLight->Init({ 0.0f,0.0f,200.0f }, { 2.0f,2.0f,2.0f }, 1000.0f, spDir, spEmitAngle);

	//UIの初期化
	m_ui = NewGO<UI>(0, "ui");
	m_ui->Init();

	//スカイキューブの初期化
	m_skyCube = NewGO<SkyCube>(0, "skyCube");
	m_skyCube->Init("Assets/modelData/skyCube/sky.tkm");


	////レベル構築
	m_level.Init("Assets/level3D/level00_a.tkl", [&](LevelObjectData& objData) {
		//プレイヤー
		if (objData.EqualObjectName(L"player") == true) {
			m_player = NewGO<Player_new>(0, "player");

			//ライトを渡す処理
			if (m_player->GetSkinModelRender() != nullptr) {
				m_player->RecieveDirectionLight(m_directionLight);
				m_player->RecievePointLight(m_pointLight);
				m_player->RecieveSpotLight(m_spotLight);

				//m_player->InitModelFromInitData();
			}

			m_player->Init();
			m_player->SetPostion(objData.position);
			m_player->SetRotation(objData.rotation);
			////プレイヤーの前方、右、上ベクトルにレベルの回転を適用
			//m_player->SetVectorFromQuaternion(objData.rotation);
			m_player->InitCharaCon();

			

			return true;
		}

		////エネミー1の生成器
		//if (objData.EqualObjectName(L"enemy_01") == true) {
		//	m_enemyGenerators.push_back(NewGO<EnemyGenerator>(0, "enemyGenerator01"));
		//	m_enemyGenerators[m_enemyGenerators.size() - 1]->Init(
		//		objData.position,
		//		objData.rotation,
		//		true,
		//		enCommon
		//	);

		//	//敵生成器の前方、右、上ベクトルにレベルの回転を適用
		//	m_enemyGenerators[m_enemyGenerators.size() - 1]->SetVectorFromQuaternion(objData.rotation);

		//	return true;
		//}

		////エネミー2の生成器
		//if (objData.EqualObjectName(L"enemy_02") == true) {
		//	m_enemyGenerators.push_back(NewGO<EnemyGenerator>(0, "enemyGenerator02"));
		//	m_enemyGenerators[m_enemyGenerators.size() - 1]->Init(
		//		objData.position,
		//		objData.rotation,
		//		false,
		//		enShot
		//	);

		//	//敵生成器の前方、右、上ベクトルにレベルの回転を適用
		//	m_enemyGenerators[m_enemyGenerators.size() - 1]->SetVectorFromQuaternion(objData.rotation);

		//	return true;
		//}

		////エネミー3の生成器
		//if (objData.EqualObjectName(L"enemy_03") == true) {
		//	m_enemyGenerators.push_back(NewGO<EnemyGenerator>(0, "enemyGenerator03"));
		//	m_enemyGenerators[m_enemyGenerators.size() - 1]->Init(
		//		objData.position,
		//		objData.rotation,
		//		false,
		//		enBomb
		//	);

		//	//敵生成器の前方、右、上ベクトルにレベルの回転を適用
		//	m_enemyGenerators[m_enemyGenerators.size() - 1]->SetVectorFromQuaternion(objData.rotation);

		//	return true;
		//}

		//ステージ
		if (objData.EqualObjectName(L"stageBg") == true) {
			m_bg = NewGO<BG>(0, "bg");

			//ステージにライトを渡す処理
			if (m_bg->GetSkinModelRender() != nullptr) {
				m_bg->RecieveDirectionLight(m_directionLight);
				m_bg->RecievePointLight(m_pointLight);
				m_bg->RecieveSpotLight(m_spotLight);

				//m_bg->InitModelFromInitData();
			}

			m_bg->Init(
				objData.position,
				objData.rotation,
				Vector3::One
			);

			
			return true;
		}

		//背景
		if (objData.EqualObjectName(L"backGround") == true) {
			m_stageBackGround = NewGO<StageBackGround>(0, "stageBackGround");
			
			//背景にライトを渡す処理
			if (m_stageBackGround->GetSkinModelRender() != nullptr) {
				m_stageBackGround->RecieveDirectionLight(m_directionLight);
				m_stageBackGround->RecievePointLight(m_pointLight);
				m_stageBackGround->RecieveSpotLight(m_spotLight);

				//m_stageBackGround->InitModelFromInitData();
			}
			
			m_stageBackGround->Init(
				objData.position,
				objData.rotation,
				Vector3::One);
			return true;
		}

		//構築終了
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

	//m_pointLight->SetPosition(m_player->GetPosition()/* + m_player->GetUp()*100.0f*/);

	/*m_spotLight->SetPosition(m_player->GetPosition() + m_player->GetUp() * 20.0f);
	m_spotLight->SetDirection(m_player->GetPosition() - m_spotLight->GetPosition());*/

	////テスト：プレイヤーの削除
	//if (g_pad[0]->IsTrigger(enButtonX)) {
	//	DeleteGO(m_player);
	//}


	//if (g_pad[0]->IsTrigger(enButtonSelect)) {
	//	DeleteGO(this);
	//}

	

	/*m_sprite = NewGO<SpriteRender>(0,"testSprite");
	m_sprite->Init(
		"Assets/sprite/ui/1.dds",
		256,
		256,
		AlphaBlendMode_Trans
	);

	if (m_sprite != nullptr) {
		DeleteGO(m_sprite);
	}*/
	
	

}