#pragma once

/// @brief �^�C���A�b�v���̃��U���g�\���N���X
class Result
{
public:
	Result();
	~Result();

	/// @brief ����������
	void Init();

	/// @brief �X�v���C�g�����Ƀt�F�[�h�C�������鏈��
	void FadeInSprite();

	/// @brief ���������Ƀt�F�[�h�C�������鏈��
	void FadeInText();

	/// @brief �������t�F�[�h�A�E�g�����鏈��
	void FadeOutText(const float fadeOutRate);

	/// @brief �X�V����
	void Update();

private:
	SpriteRender* m_screenSprite = nullptr;					//�w�i���Â�����X�v���C�g
	SpriteRender* m_screenTopSprite = nullptr;				//�t�F�[�h�A�E�g�p���X�v���C�g
	SpriteRender* m_resultSprite = nullptr;					//���U���g�̕����X�v���C�g
	SpriteRender* m_pressButtonSprite = nullptr;			//�^�C�g���ɖ߂�{�^���̃X�v���C�g
	
	SpriteRender* m_remainPlayerTextSprite = nullptr;		//�c�@���\���e�L�X�g�X�v���C�g
	FontRender* m_remainPlayerNum = nullptr;				//�c�@��
	std::wstring m_remainPlayerNumWs = L"";
	FontRender* m_remainPlayerBonusText = nullptr;			//�c�@�{�[�i�X�̃e�L�X�g
	FontRender* m_remainPlayerBonusNum = nullptr;			//�c�@���{�[�i�X���l
	std::wstring m_remainPlayerBonusNumWs = L"";

	SpriteRender* m_destructionRateTextSprite = nullptr;	//�G�����ɑ΂��錂�j��
	FontRender* m_destructionRateNum = nullptr;				//���j�����l
	std::wstring m_destructionRateWs = L"";
	FontRender* m_destructionBonusText = nullptr;			//�{�[�i�X�̃e�L�X�g
	FontRender* m_destructionBonusNum = nullptr;			//���j���{�[�i�X���l
	std::wstring m_destructionBonusWs = L"";

	SpriteRender* m_finalScoreTextSprite = nullptr;			//�ŏI�X�R�A�e�L�X�g�X�v���C�g
	FontRender* m_finalScoreNum = nullptr;					//�ŏI�X�R�A���l
	std::wstring m_finalScoreNumWs = L"";

	bool m_isFinishFadeInSprite = false;
	bool m_isFinishFadeInText = false;
	bool m_exitFlag = false;
};

