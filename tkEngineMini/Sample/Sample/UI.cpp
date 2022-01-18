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

	//時間を減少
	GameDirector::GetInstance()->DecTime();

	//スコアを更新
	m_score.UpdateScore();
	//時間を更新
	m_timer.UpdateTime();
	//外枠を更新
	m_uiOutline.Update();
	//ライフを更新
	m_life.Update();
	//ゲームオーバー機能を更新
	m_gameOver.Update();
	//リザルト画面を更新
	m_result.Update();
}