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
	//���Ԃ�����
	GameDirector::GetInstance()->DecTime();

	//�X�R�A���X�V
	m_score.UpdateScore();
	//���Ԃ��X�V
	m_timer.UpdateTime();
	//�O�g���X�V
	m_uiOutline.Update();
	//���C�t���X�V
	m_life.Update();

}