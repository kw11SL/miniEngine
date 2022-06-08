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
	//レベル関連
	const char* CAPSULE_LEVEL_FILEPATH = "Assets/level3D/level02.tkl";	//レベルのファイルパス

	//bgm関連
	const wchar_t* GAME_SCENE_BGM_FILEPATH = L"Assets/wav/gamescene_bgm1.wav";
	const float GAME_SCENE_BGM_INIT_VOLUME = 0.1f;
	const float GAME_SCENE_BGM_FADEOUT_RATE = 0.005f;
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
	//ディレクションライトの初期化
	m_directionLight = NewGO<DirectionLight>(0, DIRECTION_LIGHT_NAME);
	m_directionLight->Init({ 1.0f,0.0f,0.0f }/*g_camera3D->GetPosition() - g_camera3D->GetTarget()*/, { 0.25f,0.25f,0.25f }, { 0.7f,0.7f,0.7f });
	m_directionLight->SetEyePos(g_camera3D->GetPosition());

	//ポイントライトの初期化
	m_pointLight = NewGO<PointLight>(0, POINT_LIGHT_NAME);
	m_pointLight->Init({ 0.0f,0.0f,100.0f }, { 2.4f,2.2f,2.0f }, 1000.0f);

	//スポットライトの初期化
	m_spotLight = NewGO<SpotLight>(0, SPOT_LIGHT_NAME);
	Vector3 spDir = { 0.0f,0.0f,-1.0f };
	spDir.Normalize();
	float spEmitAngle = Math::DegToRad(90.0f);
	m_spotLight->Init({ 0.0f,0.0f,200.0f }, { 0.0f,1.0f,0.0f }, 1000.0f, spDir, spEmitAngle);

	//UIの初期化
	m_ui = NewGO<UI>(0, "ui");
	m_ui->Init();

	//スカイキューブの初期化
	m_skyCube = NewGO<SkyCube>(0, SKYCUBE_NAME);
	m_skyCube->Init();


	//レベル構築
	m_level.Init(CAPSULE_LEVEL_FILEPATH, [&](LevelObjectData& objData) {
		
		//プレイヤー
		if (objData.EqualObjectName(L"player") == true) {
			m_player = NewGO<Player_new>(0, PLAYER_NAME);
			m_player->Init();

			m_player->SetPostion(objData.position);
			m_player->SetRotation(objData.rotation);
			//プレイヤーの前方、右、上ベクトルにレベルの回転を適用
			m_player->SetVectorFromQuaternion(objData.rotation);
			m_player->InitCharaCon();

			return true;
		}

		//エネミー1の生成器
		if (objData.EqualObjectName(L"enemy_01") == true) {
			m_enemyGenerators.push_back(NewGO<EnemyGenerator>(0, "enemyGenerator01"));
			m_enemyGenerators[m_enemyGenerators.size() - 1]->Init(
				objData.position,
				objData.rotation,
				true,
				enCommon
			);

			//敵生成器の前方、右、上ベクトルにレベルの回転を適用
			m_enemyGenerators[m_enemyGenerators.size() - 1]->SetVectorFromQuaternion(objData.rotation);

			return true;
		}

		//エネミー2の生成器
		if (objData.EqualObjectName(L"enemy_02") == true) {
			m_enemyGenerators.push_back(NewGO<EnemyGenerator>(0, "enemyGenerator02"));
			m_enemyGenerators[m_enemyGenerators.size() - 1]->Init(
				objData.position,
				objData.rotation,
				false,
				enShot
			);

			//敵生成器の前方、右、上ベクトルにレベルの回転を適用
			m_enemyGenerators[m_enemyGenerators.size() - 1]->SetVectorFromQuaternion(objData.rotation);

			return true;
		}

		//エネミー3の生成器
		if (objData.EqualObjectName(L"enemy_03") == true) {
			m_enemyGenerators.push_back(NewGO<EnemyGenerator>(0, "enemyGenerator03"));
			m_enemyGenerators[m_enemyGenerators.size() - 1]->Init(
				objData.position,
				objData.rotation,
				false,
				enBomb
			);

			//敵生成器の前方、右、上ベクトルにレベルの回転を適用
			m_enemyGenerators[m_enemyGenerators.size() - 1]->SetVectorFromQuaternion(objData.rotation);

			return true;
		}

		//ステージ
		if (objData.EqualObjectName(L"stageBg") == true) {
			m_bg = NewGO<BG>(0, STAGE_NAME);

			m_bg->Init(
				objData.position,
				objData.rotation,
				Vector3::One
			);

			return true;
		}

		//背景
		if (objData.EqualObjectName(L"backGround") == true) {
			m_stageBackGround = NewGO<StageBackGround>(0, STAGE_BACK_GROUND_NAME);
			
			m_stageBackGround->Init(
				objData.position,
				objData.rotation,
				Vector3::One);

			return true;
		}

		//構築終了
		return true;
	});

	//BGMの再生
	m_ssBgm = NewGO<CSoundSource>(0);
	m_ssBgm->Init(GAME_SCENE_BGM_FILEPATH);
	m_ssBgmVolume = GAME_SCENE_BGM_INIT_VOLUME;
	m_ssBgm->SetVolume(m_ssBgmVolume);
	m_ssBgm->Play(true);

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

void Game::BGMFadeOut(const float fadeOutRate)
{
	m_ssBgmVolume -= fadeOutRate;
	if (m_ssBgmVolume <= 0.0f) {
		DeleteGO(m_ssBgm);
		m_ssBgm = nullptr;
	}

	if (m_ssBgm != nullptr) {
		m_ssBgm->SetVolume(m_ssBgmVolume);
	}
}

void Game::Update()
{
	//ポイントライトの位置をプレイヤー位置に設定
	m_pointLight->SetPosition(m_player->GetPosition() + m_player->GetUp()*100.0f);
	

	//ゲームがリザルト画面かゲームオーバー画面のとき
	if (GameDirector::GetInstance()->GetGameState() == enResult ||
		GameDirector::GetInstance()->GetGameState() == enGameOver) {
		//ゲーム終了フラグがオンでなければ
		if (m_gameEndFlag == false) {
			//ゲーム終了フラグをオン
			m_gameEndFlag = true;
		}
	}

	//ゲーム終了フラグがオンのとき
	if (m_gameEndFlag == true) {
		//フェードアウトを実行
		BGMFadeOut(GAME_SCENE_BGM_FADEOUT_RATE);
	}



}