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
	//// �Q�[�����ȊO�Ȃ珈�����Ȃ�
	//if(GameDirector::GetInstance()->GetGameState() != enGame){
	//	
	//	return;
	//}

	//�X�R�A���X�V
	m_score.UpdateScore();
	//���Ԃ��X�V
	m_timer.UpdateTime();
	//�O�g���X�V
	m_uiOutline.Update();
	//���C�t�\�����X�V
	m_life.Update();
	//wave�\�����X�V
	m_wave.Update();
	//wave�J�n�\�����X�V
	m_waveCutIn.Update();
	//����A�C�R���\�����X�V
	m_weaponIcon.Update();
	//�Q�[���I�[�o�[�@�\���X�V
	m_gameOver.Update();
	//���U���g��ʂ��X�V
	m_result.Update();
	
}