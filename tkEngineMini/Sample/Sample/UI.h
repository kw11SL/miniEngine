#pragma once
#include "Score.h"

class UI : public IGameObject
{
public:
	UI() {}
	~UI();

	/// @brief 初期化処理
	void Init();

private:
	bool Start() { return true; }
	void Update();


private:
	Score m_score;		//スコア表示用

};

