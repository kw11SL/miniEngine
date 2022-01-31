#pragma once
#include "Score.h"
#include "Timer.h"
#include "UIOutline.h"
#include "Life.h"
#include "Wave.h"
#include "WeaponIcon.h"
#include "GameOver.h"
#include "Result.h"
#include "WaveCutIn.h"

class UI : public IGameObject
{
public:
	UI();
	~UI();

	/// @brief ����������
	void Init();

private:
	bool Start();
	void Update();


private:
	Score m_score;				//�X�R�A�\���p
	Timer m_timer;				//���ԕ\���p
	UIOutline m_uiOutline;		//�O�g
	Life m_life;				//���C�t�\��
	GameOver m_gameOver;		//�Q�[���I�[�o�[���̃X�v���C�g�\��
	Result m_result;			//���U���g���
	Wave m_wave;				//wave���\
	WaveCutIn m_waveCutIn;		//wave�J�n�̃J�b�g�C��
	WeaponIcon m_weaponIcon;	//�g�p���̕���\��
};

