#include "stdafx.h"
#include "Title.h"


Title::Title()
{
	//初期化処理を行う
	Init();
}

bool Title::Start()
{
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

		CSoundSource* ss = NewGO<CSoundSource>(0);
		ss->Init(L"Assets/wav/decide_2.wav", false);
		ss->SetVolume(0.5f);
		ss->Play(false);
	}

	//ゲーム開始フラグがオンならタイトルのフェードアウト開始
	if (m_gameReady == true) {
		m_titleSprite.FadeOutSprite();
	}
	if (m_titleSprite.GetIsFinishFadeOut() == true) {
		//タイトルのフェードアウト完了でゲームをNewGO
		NewGO<Game>(0, "game");
		////ゲームの状態を変更
		//GameDirector::GetInstance()->SetGameState(enGame);
		//ゲームの状態をゲーム開始に変更
		GameDirector::GetInstance()->SetGameState(enStart);

		//自身を削除
		DeleteGO(this);
	}
}
