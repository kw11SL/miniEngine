#pragma once
#include "Score.h"
#include "Timer.h"
#include "UIOutline.h"
#include "Life.h"
#include "GameOver.h"

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
};

