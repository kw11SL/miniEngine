#pragma once


class TitleSprite
{
public:
	TitleSprite(){}
	~TitleSprite();

	/// @brief ����������
	void Init();

	/// @brief �t�F�[�h�A�E�g����
	void FadeOutSprite();

	/// @brief �X�v���C�g���擾
	/// @return �^�C�g���X�v���C�g
	const SpriteRender* GetTitleSprite() const
	{
		return m_titleSprite;
	}

	/// @brief �t�F�[�h�C���������������ǂ���
	/// @return �t�F�[�h�C�������t���O
	const bool GetIsFinishFadeIn() const
	{
		return m_isFinishFadeIn;
	}

	/// @brief �t�F�[�h�C���C�����t�F�[�h�A�E�g�������������ǂ���
	/// @return �t�F�[�h�A�E�g�����t���O
	const bool GetIsFinishFadeOut() const
	{
		return m_isFinishFadeOut;
	}

	/// @brief �X�V����
	void Update();

private:
	SpriteRender* m_titleSprite = nullptr;
	Vector4 m_titleSpriteColor = { 1.0f,1.0f,1.0f,0.0f };
	Vector3 m_titleSpriteScale = Vector3::One;
	Vector3 m_titlePos = Vector3::Zero;

	bool m_isFinishFadeIn = false;
	bool m_isFinishFadeOut = false;

};

