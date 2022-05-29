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

	/// @brief ����������
	void Init();

	/// @brief BGM�̃t�F�[�h�A�E�g�ƍ폜����
	/// @param fadeOutRate �t�F�[�h�A�E�g��
	void BGMFadeOut(const float fadeOutRate);

private:
	TitleSprite m_titleSprite;
	PressStart m_pressStart;

	bool m_gameReady = false;

	CSoundSource* m_ssTitleBgm = nullptr;
	float m_ssTitleBgmVolume = 0.0f;
};

