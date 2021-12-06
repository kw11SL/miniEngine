#include "stdafx.h"
#include "UI.h"


namespace {

}

UI::~UI()
{
	
}

void UI::Init()
{
	m_score.Init();
	m_timer.Init();
}

void UI::Update()
{
	//時間を減少
	GameDirector::GetInstance()->DecTime();

	//スコアを更新
	m_score.UpdateScore();

	//時間を更新
	m_timer.UpdateTime();

}