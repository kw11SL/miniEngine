#pragma once
class FontRender : public IGameObject
{
private:
	
	struct SFontData {
		Font m_font;								//�t�H���g
		const wchar_t* m_text;						//�e�L�X�g
		Vector2 m_position = Vector2::Zero;			//���W
		Vector2 m_pivot = { 0.5f,0.5f };			//�s�{�b�g
		Vector4 m_color = Vector4::White;			//�J���[
		float m_scale = 1.0f;						//�g�嗦
		float m_rotation = 0.0f;					//��]�p�x
	};

public:
	FontRender(){}
	~FontRender(){}

	/// @brief ����������
	/// @param text 
	/// @param position 
	/// @param color 
	/// @param rotation 
	/// @param scale 
	/// @param pivot 
	void Init(
		wchar_t const* text,
		const Vector2& position,
		const Vector4& color,
		const float rotation,
		const float scale,
		const Vector2& pivot
	);

	/// @brief �\������e�L�X�g��ݒ�
	/// @param text 
	void SetText(const wchar_t* text)
	{
		m_fontData.m_text = text;
	}

	/// @brief �e�L�X�g���擾
	/// @return 
	const wchar_t* GetText()
	{
		return m_fontData.m_text;
	}

	/// @brief ���W��ݒ�
	/// @param pos 
	void SetPosition(const Vector2& pos)
	{
		m_fontData.m_position = pos;
	}

	/// @brief ���W���擾
	/// @return 
	Vector2& GetPosition()
	{
		return m_fontData.m_position;
	}

	/// @brief �s�{�b�g��ݒ�
	/// @param pivot 
	void SetPivot(const Vector2& pivot)
	{
		m_fontData.m_pivot = pivot;
	}

	/// @brief �s�{�b�g���擾
	/// @return 
	Vector2& GetPivot()
	{
		return m_fontData.m_pivot;
	}

	/// @brief �J���[��ݒ�
	/// @param color 
	void SetColor(const Vector4& color)
	{
		m_fontData.m_color = color;
	}

	/// @brief �J���[���擾
	/// @return 
	Vector4& GetColor()
	{
		return m_fontData.m_color;
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale 
	void SetScale(float scale)
	{
		m_fontData.m_scale = scale;
	}

	/// @brief �g�嗦���擾
	/// @return 
	float& GetScale()
	{
		return m_fontData.m_scale;
	}

	/// @brief �p�x��ݒ�
	/// @param angle 
	void SetAngle(const float angle)
	{
		m_fontData.m_rotation = angle;
	}

	/// @brief �p�x���擾
	/// @return 
	float& GetAngle()
	{
		return m_fontData.m_rotation;
	}

private:
	bool Start()override { return true; }
	void Update()override;

private:
	SFontData m_fontData;
};

