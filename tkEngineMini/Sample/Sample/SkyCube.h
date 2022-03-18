#pragma once
class SkyCube:public IGameObject
{
public:
	SkyCube();
	~SkyCube();
	bool Start()override;
	void Update()override;

	/// @brief ����������
	/// @param filePath 
	void Init(const char* filePath);

	/// @brief ���W��ݒ�
	/// @param pos ���W
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		m_isDirty = true;
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale �g�嗦
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_isDirty = true;
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale �g�嗦
	void SetScale(const float scale)
	{
		m_scale = g_vec3One;
		m_scale.Scale(scale);
		m_isDirty = true;
	}

	/// <summary>
	/// ���邳��ݒ�B
	/// </summary>
	/// <param name="lum"></param>
	void SetLuminance(float lum)
	{
		m_luminance = lum;
	}

	/// @brief ���邳�����X�ɏグ�鏈��
	/// @param addRate �グ�銄��
	void FadeIn(const float addRate);

private:
	SkinModelRender* m_skinModelRender = nullptr;	//���f�������_�[
	Texture m_texture;								//�e�N�X�`��
	const wchar_t* m_textureFilePaths;				//�e�N�X�`���̃t�@�C���p�X�i�[�p
	Vector3 m_position = g_vec3Zero;				//���W
	Vector3 m_scale = g_vec3One * 10000.0f;			//�g�嗦
	Quaternion m_rot = Quaternion::Identity;		//��]
	float m_luminance = 0.0f;						//���邳
	bool m_isDirty = false;

};

