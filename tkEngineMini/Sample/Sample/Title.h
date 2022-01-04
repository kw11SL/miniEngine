#pragma once
#include "TitleSprite.h"
#include "PressStart.h"

class Title : public IGameObject
{
public:
	Title();
	~Title(){}
	bool Start() override;
	void Update() override;

	/// @brief 初期化処理
	void Init();

private:
	TitleSprite m_titleSprite;
	PressStart m_pressStart;

	bool m_gameReady = false;
};

