#pragma once
class PressStart
{
public:
	PressStart() {}
	~PressStart();

	/// @brief ����������
	void Init();

	/// @brief �t�F�[�h�C���������������ǂ���
	/// @return �t�F�[�h�C�������t���O
	const bool& GetIsFinishFadeIn() const
	{
		return m_isFinishFadeIn;
	}
	
	/// @brief �t�F�[�h�A�E�g�������������ǂ���
	/// @return �t�F�[�h�A�E�g�����t���O
	const bool& GetIsFinishFadeOut() const
	{
		return m_isFinishFadeOut;
	}

	const bool& GetIsPressStart() const
	{
		return m_isPressStart;
	}

	/// @brief �X�V����
	void Update();

private:
	SpriteRender* m_pressStartSprite = nullptr;		//�����̃X�v���C�g
	bool m_isFinishFadeIn = false;					//�t�F�[�h�C�������t���O
	bool m_isFinishFadeOut = true;					//�t�F�[�h�A�E�g�����t���O
	bool m_isPressStart = false;					//�{�^���������ꂽ�t���O
};

