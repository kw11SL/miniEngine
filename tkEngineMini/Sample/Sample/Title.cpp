#include "stdafx.h"
#include "Title.h"

namespace {
	//bgm�֘A
	//�Q�[���J�n����SE
	const wchar_t* PRESS_START_SE_FILEPATH = L"Assets/wav/decide_2.wav";	//�t�@�C���p�X
	const float PRESS_START_SE_VOLUME = 0.5f;								//����

	//�^�C�g����ʂ̋�
	const wchar_t* TITLE_BGM_FILEPATH = L"Assets/wav/title_bgm.wav";		//�t�@�C���p�X
	const float TITLE_BGM_INIT_VOLUME = 0.2f;								//����
	const float TITLE_BGM_FADEOUT_RATE = 0.02f;								//�t�F�[�h�A�E�g����
}

Title::Title()
{
	
}

Title::~Title()
{
	//nullptr�łȂ����
	if (m_ssTitleBgm != nullptr) {
		DeleteGO(m_ssTitleBgm);
	}
}

bool Title::Start()
{
	//�Q�[���̏���������
	{
		//�Q�[����������Ԃɖ߂�
		GameDirector::GetInstance()->ResetGame();
		//�Q�[���̏�Ԃ��^�C�g����ʂɂ���
		GameDirector::GetInstance()->SetGameState(enTitle);
		//�}�l�[�W��������v�f���폜
		BulletManager::GetInstance()->DeleteBullets();
		EnemyManager::GetInstance()->DeleteEnemies();
		ExplosionManager::GetInstance()->DeleteExplosions();
	}

	//bgm�Đ�
	m_ssTitleBgm = NewGO<CSoundSource>(0);
	m_ssTitleBgm->Init(TITLE_BGM_FILEPATH);
	m_ssTitleBgmVolume = TITLE_BGM_INIT_VOLUME;
	m_ssTitleBgm->SetVolume(m_ssTitleBgmVolume);
	m_ssTitleBgm->Play(true);
	

	Init();

	return true;
}

void Title::Init()
{
	m_titleSprite.Init();
	m_pressStart.Init();
}

void Title::BGMFadeOut(const float fadeOutRate)
{
	//�{�����[����������
	m_ssTitleBgmVolume -= fadeOutRate;
	
	//�{�����[����0�ȉ��ɂȂ�����
	if (m_ssTitleBgmVolume <= 0.0f) {
		//�폜
		DeleteGO(m_ssTitleBgm);
		//null�ɂ��Ă���
		m_ssTitleBgm = nullptr;
	}
	
	//�Đ����ʂ�ݒ�Bnull��������ݒ肵�Ȃ�
	if (m_ssTitleBgm != nullptr) {
		m_ssTitleBgm->SetVolume(m_ssTitleBgmVolume);
	}
}

void Title::Update()
{
	//�X�v���C�g�̍X�V����
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

	//�Q�[���J�n�t���O���I���Ȃ�
	if (m_gameReady == true) {
		//�^�C�g���X�v���C�g�̃t�F�[�h�A�E�g�J�n
		m_titleSprite.FadeOutSprite();
		//bgm�̃t�F�[�h�A�E�g���J�n
		BGMFadeOut(TITLE_BGM_FADEOUT_RATE);
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
