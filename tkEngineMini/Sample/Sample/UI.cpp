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
	//// �Q�[�����ȊO�Ȃ珈�����Ȃ�
	//if(GameDirector::GetInstance()->GetGameState() != enGame){
	//	
	//	return;
	//}

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
	//�Q�[���I�[�o�[�@�\���X�V
	m_gameOver.Update();
	//���U���g��ʂ��X�V
	m_result.Update();
}