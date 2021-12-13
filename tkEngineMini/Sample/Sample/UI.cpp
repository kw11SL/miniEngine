#include "stdafx.h"
#include "UI.h"


namespace {

}

UI::~UI()
{
	
}

void UI::Init()
{
	m_uiOutline.Init();
	m_score.Init();
	m_timer.Init();
	m_life.Init();

	/*m_score.SetColor({ 1.0f,0.0f,0.0f,1.0f });
	m_timer.SetColor({ 0.0f,1.0f,0.0f,1.0f });  */                                                                                                                                                                                                                                                                                                              
}

void UI::Update()
{
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

}