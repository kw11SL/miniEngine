#include "stdafx.h"
#include "Title.h"
//#include "TitleSprite.h"

Title::Title()
{
	//�������������s��
	Init();
}

bool Title::Start()
{
	return true;
}

void Title::Init()
{
	m_titleSprite.Init();
}

void Title::Update()
{
	m_titleSprite.Update();

	//�t�F�[�h�C��������������ԂŃX�^�[�g�{�^���������ƃQ�[���J�n�t���O���I��
	if (g_pad[0]->IsPress(enButtonStart) && 
		m_titleSprite.GetIsFinishFadeIn() == true) {
		//�Q�[���J�n�t���O���I��
		m_gameReady = true;
	}

	//�Q�[���J�n�t���O���I���Ȃ�^�C�g���̃t�F�[�h�A�E�g�J�n
	if (m_gameReady == true) {
		m_titleSprite.FadeOutSprite();
	}
	if (m_titleSprite.GetIsFinishFadeOut() == true) {
		//�^�C�g���̃t�F�[�h�A�E�g�����ŃQ�[����NewGO
		NewGO<Game>(0, "game");
		//���g���폜
		DeleteGO(this);
	}
}
