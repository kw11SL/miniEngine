#include "stdafx.h"
#include "UI.h"


namespace {

}

UI::UI()
{

}

UI::~UI()
{
}

bool UI::Start()
{
	m_uiOutline.Init();
	m_score.Init();
	m_timer.Init();
	m_life.Init();
	m_wave.Init();
	m_waveCutIn.Init();
	m_weaponIcon.Init();
	m_gameOver.Init();
	m_result.Init();

	return true;
}

void UI::Init()
{
	
}

void UI::Update()
{
	//// ゲーム中以外なら処理しない
	//if(GameDirector::GetInstance()->GetGameState() != enGame){
	//	
	//	return;
	//}

	//スコアを更新
	m_score.UpdateScore();
	//時間を更新
	m_timer.UpdateTime();
	//外枠を更新
	m_uiOutline.Update();
	//ライフ表示を更新
	m_life.Update();
	//wave表示を更新
	m_wave.Update();
	//wave開始表示を更新
	m_waveCutIn.Update();
	//武器アイコン表示を更新
	m_weaponIcon.Update();
	//ゲームオーバー機能を更新
	m_gameOver.Update();
	//リザルト画面を更新
	m_result.Update();
	
}