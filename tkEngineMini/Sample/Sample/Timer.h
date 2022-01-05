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

	/// @brief ���W��ݒ�(�X�v���C�g)
	/// @param pos ���W
	void SetPositionSprite(const Vector3& pos)
	{
		m_timeSprite->SetPosition(pos);
	}

	/// @brief ���W��ݒ�
	/// @param pos ���W
	void SetPosition(const Vector2& pos)
	{
		m_timeNumber->SetPosition(pos);
	}

	/// @brief �J���[��ݒ�
	/// @param color 
	void SetColor(const Vector4& color)
	{

		m_timeNumber->SetColor(color);
	}

	/// @brief �g�嗦��ݒ�
	/// @param scaleRate 
	void SetScale(const float scaleRate)
	{
		m_timeNumber->SetScale(scaleRate);
	}

	/// @brief ��]��ݒ�
	/// @param rotate 
	void SetRotate(const float rotate)
	{
		m_timeNumber->SetAngle(rotate);
	}

	/// @brief �s�{�b�g��ݒ�
	/// @param pivot 
	void SetPivot(const Vector2& pivot)
	{
		m_timeNumber->SetPivot(pivot);
	}

private:
	SpriteRender* m_timeSprite = nullptr;
	SpriteRender* m_timeSpriteShadow = nullptr;

	FontRender* m_timeNumber = nullptr;		//�X�R�A�̐��l�\������
	FontRender* m_timeNumberOutline = nullptr;

	Vector3 m_timeSpritePos = Vector3::Zero;
	Vector2 m_timeNumPos = Vector2::Zero;	//�X�R�A�̐��l�����̍��W

	std::wstring m_timeNumWs = L"hoge";		//�|�C���^�ێ��p�̃����o


};

