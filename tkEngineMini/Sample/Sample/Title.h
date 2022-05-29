#pragma once
#include "TitleSprite.h"
#include "PressStart.h"
//#include "SoundSource.h"
class CSoundSource;

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override;
	void Update() override;

	/// @brief 初期化処理
	void Init();

	/// @brief BGMのフェードアウトと削除処理
	/// @param fadeOutRate フェードアウト量
	void BGMFadeOut(const float fadeOutRate);

private:
	TitleSprite m_titleSprite;
	PressStart m_pressStart;

	bool m_gameReady = false;

	CSoundSource* m_ssTitleBgm = nullptr;
	float m_ssTitleBgmVolume = 0.0f;
};

