#include "stdafx.h"
#include "Title.h"

namespace {
	const wchar_t* PRESS_START_SE_FILEPATH = L"Assets/wav/decide_2.wav";
	const float PRESS_START_SE_VOLUME = 0.5f;
}

Title::Title()
{
	
}

Title::~Title()
{

}

bool Title::Start()
{
	//�Q�[���̏���������
	{
		//�Q�[����������Ԃɖ߂�
		GameDirector::GetInstance()->ResetGame();
		//�Q�[���̏�Ԃ��^�C�g����ʂɂ���
		GameDirector::GetInstance()->SetGameState(enTitle);
		//�o���b�g�}�l�[�W��������e���폜
		BulletManager::GetInstance()->DeleteBullets();
	}

	Init();
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

		//�{�^�����������Ƃ��̉����Đ�
		CSoundSource* ss = NewGO<CSoundSource>(0);
		ss->Init(PRESS_START_SE_FILEPATH, false);
		ss->SetVolume(PRESS_START_SE_VOLUME);
		ss->Play(false);
	}

	//�Q�[���J�n�t���O���I���Ȃ�^�C�g���̃t�F�[�h�A�E�g�J�n
	if (m_gameReady == true) {
		m_titleSprite.FadeOutSprite();
	}
	if (m_titleSprite.GetIsFinishFadeOut() == true) {
		//�^�C�g���̃t�F�[�h�A�E�g�����ŃQ�[����NewGO
		NewGO<Game>(0, GAME_SCENE_NAME);
		//�Q�[���̏�Ԃ��Q�[���J�n�ɕύX
		GameDirector::GetInstance()->SetGameState(enStart);

		//���g���폜
		DeleteGO(this);
	}
}
