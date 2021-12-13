#pragma once


/// @brief �X�R�A�\���N���X�BUI�N���X�Ŏg�p
class Score
{
public:
	Score() {};
	~Score();
	
	/// @brief ���������� 
	void Init();

	/// @brief �Q�[���f�B���N�^�[����擾�����X�R�A���X�V����
	void UpdateScore();

	/// @brief ���W��ݒ�
	/// @param pos 
	void SetPosition(const Vector2& pos)
	{
		m_scoreNumber->SetPosition(pos);
	}

	/// @brief �J���[��ݒ�
	/// @param color 
	void SetColor(const Vector4& color)
	{
		m_scoreNumber->SetColor(color);
	}

	/// @brief �g�嗦��ݒ�
	/// @param scaleRate 
	void SetScale(const float scaleRate)
	{
		m_scoreNumber->SetScale(scaleRate);
	}

	/// @brief ��]��ݒ�
	/// @param rotate 
	void SetRotate(const float rotate)
	{
		m_scoreNumber->SetAngle(rotate);
	}

	/// @brief �s�{�b�g��ݒ�
	/// @param pivot 
	void SetPivot(const Vector2& pivot)
	{
		m_scoreNumber->SetPivot(pivot);
	}

private:
	SpriteRender* m_scoreSprite = nullptr;
	FontRender* m_scoreNumber = nullptr;	//�X�R�A�̐��l�\������

	Vector3 m_scoreSpritePos = Vector3::Zero;
	Vector2 m_scoreTextPos = Vector2::Zero;	//�X�R�A�̃e�L�X�g�����̍��W

	std::wstring m_scoreNumWs = L"hoge";				//�|�C���^�ێ��p�̃����o
};

