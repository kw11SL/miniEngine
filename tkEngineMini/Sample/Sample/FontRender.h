#pragma once

class RenderingEngine;

struct SFontData {
	Font font;								//�t�H���g
	const wchar_t* text;					//�e�L�X�g
	Vector2 position = Vector2::Zero;		//���W
	Vector2 pivot = { 0.5f,0.5f };			//�s�{�b�g
	Vector4 color = Vector4::White;			//�J���[
	float scale = 1.0f;						//�g�嗦
	float rotation = 0.0f;					//��]�p�x
};

class FontRender : public IGameObject
{

public:
	FontRender(){}
	~FontRender();

	/// @brief ����������
	/// @param text �\������e�L�X�g
	void Init(const wchar_t* text);

	/// @brief �\������e�L�X�g��ݒ�
	/// @param text 
	void SetText(const wchar_t* text)
	{
		m_text = text;
		m_fontData.text = m_text;
	}

	/// @brief �e�L�X�g���擾
	/// @return 
	const wchar_t* GetText()
	{
		return m_text;
	}

	/// @brief ���W��ݒ�
	/// @param pos 
	void SetPosition(const Vector2& pos)
	{
		m_position = pos;
		//m_fontData.position = m_position;
	}

	/// @brief ���W���擾
	/// @return 
	Vector2& GetPosition()
	{
		return m_position;
	}

	/// @brief �s�{�b�g��ݒ�
	/// @param pivot 
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
		//m_fontData.pivot = m_pivot;
	}

	/// @brief �s�{�b�g���擾
	/// @return 
	Vector2& GetPivot()
	{
		return m_pivot;
	}

	/// @brief �J���[��ݒ�
	/// @param color 
	void SetColor(const Vector4& color)
	{
		m_color = color;
		//m_fontData.color = m_color;
	}

	/// @brief �J���[���擾
	/// @return 
	Vector4& GetColor()
	{
		return m_color;
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale 
	void SetScale(float scale)
	{
		m_scale = scale;
		//m_fontData.scale = m_scale;
	}

	/// @brief �g�嗦���擾
	/// @return 
	float& GetScale()
	{
		return m_scale;
	}

	/// @brief �p�x��ݒ�
	/// @param angle 
	void SetAngle(const float angle)
	{
		m_rotation = angle;
		//m_fontData.rotation = m_rotation;
	}

	/// @brief �p�x���擾
	/// @return 
	float& GetAngle()
	{
		return m_rotation;
	}

private:
	bool Start()override;
	void Update()override;

	/// @brief �t�H���g�f�[�^�̃����o���X�V
	void UpdateFontData();

private:
	RenderingEngine* m_renderingEngine = nullptr;
	SFontData m_fontData;						//�t�H���g�f�[�^�̍\����
	Font m_font;								//�t�H���g
	const wchar_t* m_text = L"";				//�e�L�X�g
	Vector2 m_position = Vector2::Zero;			//���W
	Vector2 m_pivot = { 0.5f,0.5f };			//�s�{�b�g
	Vector4 m_color = Vector4::White;			//�J���[
	float m_scale = 1.0f;						//�g�嗦
	float m_rotation = 0.0f;					//��]�p�x
};

