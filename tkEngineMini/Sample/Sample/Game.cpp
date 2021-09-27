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
	DeleteGO(m_bg);
	DeleteGO(m_direction);
	DeleteGO(m_directionLight);
	DeleteGO(m_pointLight);
	DeleteGO(m_spotLight);

	//テスト
	DeleteGO(m_sprite);
}

bool Game::Start()
{
	//ライトカメラの初期化
	//視点の設定
	m_lightCamera.SetPosition(0.0f, 500.0f, 0.0f);
	//注視点の設定
	m_lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	//上方向の設定
	m_lightCamera.SetUp({ 1.0f, 0.0f, 0.0f });
	//ライトビュープロジェクション行列の更新
	m_lightCamera.Update();


	//ディレクションライトの初期化
	m_directionLight = NewGO<DirectionLight>(0, "directionlight");
	m_directionLight->Init({ 1.0f,0.0f,1.0f }, { 1.0f,1.0f,1.0f }, { 0.3f,0.3f,0.3f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//ポイントライトの初期化
	m_pointLight = NewGO<PointLight>(0, "pointlight");
	m_pointLight->Init({ 0.0f,0.0f,50.0f }, { 1.0f,0.0f,0.0f }, 300.0f);

	//スポットライトの初期化
	m_spotLight = NewGO<SpotLight>(0, "spotlight");
	Vector3 spDir = { 0.0f,0.0f,-1.0f };
	spDir.Normalize();
	float spEmitAngle = Math::DegToRad(25.0f);
	m_spotLight->Init({ 0.0f,0.0f,200.0f }, { 2.0f,2.0f,2.0f },100.0f, spDir, spEmitAngle);


	//プレイヤーの初期化
	m_player = NewGO<Player_new>(0, "player");	
	
	//背景の初期化
	m_bg = NewGO<BG>(0, "bg");
	
	////方向指示の初期化
	//m_direction = NewGO<Direction>(0, "direction");
	//m_direction->SetScale({ 1.0f,1.0f,1.0f });

	//プレイヤーにライトを渡す処理
	if (m_player->GetSkinModelRender() != nullptr) {
		m_player->RecieveDirectionLight(m_directionLight);
		m_player->RecievePointLight(m_pointLight);
		m_player->RecieveSpotLight(m_spotLight);
	}

	//背景にライトを渡す処理
	if (m_bg->GetSkinModelRender() != nullptr) {
		m_bg->RecieveDirectionLight(m_directionLight);
		m_bg->RecievePointLight(m_pointLight);
		m_bg->RecieveSpotLight(m_spotLight);
	}

	////方向表示モデルにライトを渡す処理
	//if (m_direction->GetSkinModelRender() != nullptr) {
	//	m_direction->RecieveDirectionLight(m_directionLight);
	//	//m_direction->RecievePointLight(m_pointLight);
	//	m_direction->RecieveSpotLight(m_spotLight);
	//}
	
	////スプライト表示テスト
	//m_sprite = NewGO<SpriteRender>(0, "sprite");
	//m_sprite->Init("Assets/sprite/finish.dds",256,256,AlphaBlendMode_Trans);
	//m_sprite->SetColor({ 1.0f,0.0f,0.0f,0.5f });

	return true;
}

void Game::Update()
{

}