#include "stdafx.h"
#include "Title.h"

namespace {
	const wchar_t* PRESS_START_SE_FILEPATH = L"Assets/wav/decide_2.wav";
	const float PRESS_START_SE_VOLUME = 0.5f;
}

Title::Title()
{
	
}

Title::~Title()
{

}

bool Title::Start()
{
	//ゲームの初期化処理
	{
		//ゲームを初期状態に戻す
		GameDirector::GetInstance()->ResetGame();
		//ゲームの状態をタイトル画面にする
		GameDirector::GetInstance()->SetGameState(enTitle);
		//バレットマネージャ内から弾を削除
		BulletManager::GetInstance()->DeleteBullets();
	}

	Init();
	return true;
}

void Title::Init()
{
	m_titleSprite.Init();
	m_pressStart.Init();
}

void Title::Update()
{
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

	//ゲーム開始フラグがオンならタイトルのフェードアウト開始
	if (m_gameReady == true) {
		m_titleSprite.FadeOutSprite();
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
