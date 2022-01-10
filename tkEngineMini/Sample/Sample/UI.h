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

	/// @brief 初期化処理
	void Init();

private:
	bool Start();
	void Update();


private:
	Score m_score;				//スコア表示用
	Timer m_timer;				//時間表示用
	UIOutline m_uiOutline;		//外枠
	Life m_life;				//ライフ表示
	GameOver m_gameOver;		//ゲームオーバー時のスプライト表示
};

