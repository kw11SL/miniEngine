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
	//�X�R�A���X�V
	m_score.UpdateScore();
}