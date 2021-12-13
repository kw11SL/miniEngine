#pragma once
#include "Score.h"
#include "Timer.h"
#include "UIOutline.h"
#include "Life.h"

class UI : public IGameObject
{
public:
	UI() {}
	~UI();

	/// @brief ����������
	void Init();

private:
	bool Start() { return true; }
	void Update();


private:
	Score m_score;				//�X�R�A�\���p
	Timer m_timer;				//���ԕ\���p
	UIOutline m_uiOutline;		//�O�g
	Life m_life;				//���C�t�\��
};

