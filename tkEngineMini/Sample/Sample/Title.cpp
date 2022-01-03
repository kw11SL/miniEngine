#include "stdafx.h"
#include "Title.h"
//#include "TitleSprite.h"

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
}

void Title::Update()
{
	m_titleSprite.Update();

	//フェードインが完了した状態でスタートボタンを押すとゲーム開始フラグをオン
	if (g_pad[0]->IsPress(enButtonStart) && 
		m_titleSprite.GetIsFinishFadeIn() == true) {
		//ゲーム開始フラグをオン
		m_gameReady = true;
	}

	//ゲーム開始フラグがオンならタイトルのフェードアウト開始
	if (m_gameReady == true) {
		m_titleSprite.FadeOutSprite();
	}
	if (m_titleSprite.GetIsFinishFadeOut() == true) {
		//タイトルのフェードアウト完了でゲームをNewGO
		NewGO<Game>(0, "game");
		//自身を削除
		DeleteGO(this);
	}
}
