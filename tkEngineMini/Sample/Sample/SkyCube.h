#pragma once
class SkyCube:public IGameObject
{
public:
	SkyCube(){}
	~SkyCube();
	bool Start()override;
	void Update()override;
	//void Render(RenderContext& rc)override;

	/// @brief ‰Šú‰»ˆ—
	/// @param filePath 
	void Init(const char* filePath);

	/// @brief À•W‚ğİ’è
	/// @param pos À•W
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		m_isDirty = true;
	}

	/// @brief Šg‘å—¦‚ğİ’è
	/// @param scale Šg‘å—¦
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_isDirty = true;
	}

	/// @brief Šg‘å—¦‚ğİ’è
	/// @param scale Šg‘å—¦
	void SetScale(const float scale)
	{
		m_scale = g_vec3One;
		m_scale.Scale(scale);
		m_isDirty = true;
	}

	/// <summary>
	/// –¾‚é‚³‚ğİ’èB
	/// </summary>
	/// <param name="lum"></param>
	void SetLuminance(float lum)
	{
		m_luminance = lum;
	}

private:
	SkinModelRender* m_skinModelRender = nullptr;
	Texture m_texture;
	const wchar_t* m_textureFilePaths;
	Vector3 m_position = g_vec3Zero;
	Vector3 m_scale = g_vec3One * 1000.0f;
	Quaternion m_rot = Quaternion::Identity;
	float m_luminance = 3.8f;
	bool m_isDirty = false;
};

