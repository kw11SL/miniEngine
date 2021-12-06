#pragma once
class Timer
{
public:
	Timer(){}
	~Timer();

	/// @brief ���������� 
	void Init();

	/// @brief �Q�[���f�B���N�^�[����擾�����X�R�A���X�V����
	void UpdateTime();

	/// @brief ���W��ݒ�
	/// @param pos 
	void SetPosition(const Vector2& pos)
	{
		m_timeText->SetPosition(pos);
		m_timeNumber->SetPosition(pos);
	}

	/// @brief �J���[��ݒ�
	/// @param color 
	void SetColor(const Vector4& color)
	{
		m_timeText->SetColor(color);
		m_timeNumber->SetColor(color);
	}

	/// @brief �g�嗦��ݒ�
	/// @param scaleRate 
	void SetScale(const float scaleRate)
	{
		m_timeText->SetScale(scaleRate);
		m_timeNumber->SetScale(scaleRate);
	}

	/// @brief ��]��ݒ�
	/// @param rotate 
	void SetRotate(const float rotate)
	{
		m_timeText->SetAngle(rotate);
		m_timeNumber->SetAngle(rotate);
	}

	/// @brief �s�{�b�g��ݒ�
	/// @param pivot 
	void SetPivot(const Vector2& pivot)
	{
		m_timeText->SetPivot(pivot);
		m_timeNumber->SetPivot(pivot);
	}

private:
	FontRender* m_timeText = nullptr;		//�X�R�A�̕����\������
	FontRender* m_timeNumber = nullptr;	//�X�R�A�̐��l�\������

	Vector2 m_timeTextPos = Vector2::Zero;	//�X�R�A�̃e�L�X�g�����̍��W

	std::wstring m_timeNumWs = L"hoge";		//�|�C���^�ێ��p�̃����o


};

