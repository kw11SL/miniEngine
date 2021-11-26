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

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_enemy);
	DeleteGO(m_bg);
	DeleteGO(m_directionLight);
	DeleteGO(m_pointLight);
	DeleteGO(m_spotLight);
	DeleteGO(m_ui);
	DeleteGO(m_enemyGenerator);

	//テスト
	DeleteGO(m_sprite);
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
	m_directionLight->Init({ 1.0f,0.0f,1.0f }, { 0.5f,0.5f,0.5f }, { 0.3f,0.3f,0.3f });
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

	//プレイヤーの初期化
	m_player = NewGO<Player_new>(0, "player");
	m_player->Init(renderingEngine);

	////エネミーの初期化
	//m_enemy = NewGO<Enemy>(0, "enemy");
	//m_enemy->Init(renderingEngine, { 0.0f,700.0f,-300.0f },enCommon);
	
	//エネミー生成器の初期化
	m_enemyGenerator = NewGO<EnemyGenerator>(0, "enemygenerator");
	m_enemyGenerator->Init({ 0.0f,700.0f,-300.0f }, Quaternion::Identity, enBomb);

	//背景の初期化
	m_bg = NewGO<BG>(0, "bg");
	m_bg->Init(renderingEngine);

	//UIの初期化
	m_ui = NewGO<UI>(0, "ui");
	m_ui->Init();

	//プレイヤーにライトを渡す処理
	if (m_player->GetSkinModelRender() != nullptr) {
		m_player->RecieveDirectionLight(m_directionLight);
		m_player->RecievePointLight(m_pointLight);
		m_player->RecieveSpotLight(m_spotLight);
		
		m_player->InitModelFromInitData();
	}

	////エネミーにライトを渡す処理
	//if (m_enemy->GetSkinModelRender() != nullptr) {
	//	m_enemy->RecieveDirectionLight(m_directionLight);
	//	m_enemy->RecievePointLight(m_pointLight);
	//	m_enemy->RecieveSpotLight(m_spotLight);

	//	m_enemy->InitModelFromInitData();
	//}

	//背景にライトを渡す処理
	if (m_bg->GetSkinModelRender() != nullptr) {
		m_bg->RecieveDirectionLight(m_directionLight);
		m_bg->RecievePointLight(m_pointLight);
		m_bg->RecieveSpotLight(m_spotLight);

		m_bg->InitModelFromInitData();
	}

	////スプライト表示テスト
	//m_sprite = NewGO<SpriteRender>(0, "sprite");
	//m_sprite->Init("Assets/sprite/finish.dds",256,256,AlphaBlendMode_Trans);
	//m_sprite->SetColor({ 1.0f,0.0f,0.0f,0.5f });


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