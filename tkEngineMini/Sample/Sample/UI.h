#pragma once
#include "Score.h"

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
	Score m_score;		//�X�R�A�\���p

};

