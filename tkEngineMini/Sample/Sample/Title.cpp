#include "stdafx.h"
#include "Title.h"

namespace {
	//bgm関連
	//ゲーム開始時のSE
	const wchar_t* PRESS_START_SE_FILEPATH = L"Assets/wav/decide_2.wav";	//ファイルパス
	const float PRESS_START_SE_VOLUME = 0.5f;								//音量

	//タイトル画面の曲
	const wchar_t* TITLE_BGM_FILEPATH = L"Assets/wav/title_bgm.wav";		//ファイルパス
	const float TITLE_BGM_INIT_VOLUME = 0.2f;								//音量
	const float TITLE_BGM_FADEOUT_RATE = 0.02f;								//フェードアウト割合
}

Title::Title()
{
	
}

Title::~Title()
{
	//nullptrでなければ
	if (m_ssTitleBgm != nullptr) {
		DeleteGO(m_ssTitleBgm);
	}
}

bool Title::Start()
{
	//ゲームの初期化処理
	{
		//ゲームを初期状態に戻す
		GameDirector::GetInstance()->ResetGame();
		//ゲームの状態をタイトル画面にする
		GameDirector::GetInstance()->SetGameState(enTitle);
		//マネージャ内から要素を削除
		BulletManager::GetInstance()->DeleteBullets();
		EnemyManager::GetInstance()->DeleteEnemies();
		ExplosionManager::GetInstance()->DeleteExplosions();
	}

	//bgm再生
	m_ssTitleBgm = NewGO<CSoundSource>(0);
	m_ssTitleBgm->Init(TITLE_BGM_FILEPATH);
	m_ssTitleBgmVolume = TITLE_BGM_INIT_VOLUME;
	m_ssTitleBgm->SetVolume(m_ssTitleBgmVolume);
	m_ssTitleBgm->Play(true);
	

	Init();

	return true;
}

void Title::Init()
{
	m_titleSprite.Init();
	m_pressStart.Init();
}

void Title::BGMFadeOut(const float fadeOutRate)
{
	//ボリュームを下げる
	m_ssTitleBgmVolume -= fadeOutRate;
	
	//ボリュームが0以下になったら
	if (m_ssTitleBgmVolume <= 0.0f) {
		//削除
		DeleteGO(m_ssTitleBgm);
		//nullにしておく
		m_ssTitleBgm = nullptr;
	}
	
	//再生音量を設定。nullだったら設定しない
	if (m_ssTitleBgm != nullptr) {
		m_ssTitleBgm->SetVolume(m_ssTitleBgmVolume);
	}
}

void Title::Update()
{
	//スプライトの更新処理
	m_titleSprite.Update();

	//タイトルスプライトのフェードインが完了したらpressstartのUpdateを処理し始める
	if (m_titleSprite.GetIsFinishFadeIn() == true) {
		m_pressStart.Update();
	}

	//フェードインが完了した状態でスタートボタンを押すとゲーム開始フラグをオン
	if (g_pad[0]->IsTrigger(enButtonStart) && 
		m_titleSprite.GetIsFinishFadeIn() == true) {
		//ゲーム開始フラグをオン
		m_gameReady = true;

		//ボタンを押したときの音を再生
		CSoundSource* ss = NewGO<CSoundSource>(0);
		ss->Init(PRESS_START_SE_FILEPATH, false);
		ss->SetVolume(PRESS_START_SE_VOLUME);
		ss->Play(false);
	}

	//ゲーム開始フラグがオンなら
	if (m_gameReady == true) {
		//タイトルスプライトのフェードアウト開始
		m_titleSprite.FadeOutSprite();
		//bgmのフェードアウトも開始
		BGMFadeOut(TITLE_BGM_FADEOUT_RATE);
	}
	if (m_titleSprite.GetIsFinishFadeOut() == true) {
		//タイトルのフェードアウト完了でゲームをNewGO
		NewGO<Game>(0, GAME_SCENE_NAME);
		//ゲームの状態をゲーム開始に変更
		GameDirector::GetInstance()->SetGameState(enStart);

		//自身を削除
		DeleteGO(this);
	}
}
