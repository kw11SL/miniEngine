#include "stdafx.h"
#include "Title.h"


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
	m_pressStart.Init();
}

void Title::Update()
{
	m_titleSprite.Update();

	//�^�C�g���X�v���C�g�̃t�F�[�h�C��������������pressstart��Update���������n�߂�
	if (m_titleSprite.GetIsFinishFadeIn() == true) {
		m_pressStart.Update();
	}

	//�t�F�[�h�C��������������ԂŃX�^�[�g�{�^���������ƃQ�[���J�n�t���O���I��
	if (g_pad[0]->IsTrigger(enButtonStart) && 
		m_titleSprite.GetIsFinishFadeIn() == true) {
		//�Q�[���J�n�t���O���I��
		m_gameReady = true;

		CSoundSource* ss = NewGO<CSoundSource>(0);
		ss->Init(L"Assets/wav/decide_2.wav", false);
		ss->SetVolume(0.5f);
		ss->Play(false);
	}

	//�Q�[���J�n�t���O���I���Ȃ�^�C�g���̃t�F�[�h�A�E�g�J�n
	if (m_gameReady == true) {
		m_titleSprite.FadeOutSprite();
	}
	if (m_titleSprite.GetIsFinishFadeOut() == true) {
		//�^�C�g���̃t�F�[�h�A�E�g�����ŃQ�[����NewGO
		NewGO<Game>(0, "game");
		////�Q�[���̏�Ԃ�ύX
		//GameDirector::GetInstance()->SetGameState(enGame);
		//�Q�[���̏�Ԃ��Q�[���J�n�ɕύX
		GameDirector::GetInstance()->SetGameState(enStart);

		//���g���폜
		DeleteGO(this);
	}
}
