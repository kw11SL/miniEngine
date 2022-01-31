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
	Result m_result;			//リザルト画面
	Wave m_wave;				//wave数表
	WaveCutIn m_waveCutIn;		//wave開始のカットイン
	WeaponIcon m_weaponIcon;	//使用中の武器表示
};

