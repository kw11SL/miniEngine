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
	//���Ԃ�����
	GameDirector::GetInstance()->DecTime();

	//�X�R�A���X�V
	m_score.UpdateScore();

	//���Ԃ��X�V
	m_timer.UpdateTime();

}