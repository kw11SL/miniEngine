#pragma once
#include "TitleSprite.h"
//class TitleSprite;

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

	bool m_gameReady = false;
};

