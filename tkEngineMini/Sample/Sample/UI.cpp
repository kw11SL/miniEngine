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
}

void UI::Update()
{
	//スコアを更新
	m_score.UpdateScore();
}