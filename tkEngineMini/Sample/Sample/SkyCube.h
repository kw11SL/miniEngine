#pragma once
class SkyCube:public IGameObject
{
public:
	SkyCube();
	~SkyCube();
	bool Start()override;
	void Update()override;

	/// @brief 初期化処理
	/// @param filePath 
	void Init(const char* filePath);

	/// @brief 座標を設定
	/// @param pos 座標
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		m_isDirty = true;
	}

	/// @brief 拡大率を設定
	/// @param scale 拡大率
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_isDirty = true;
	}

	/// @brief 拡大率を設定
	/// @param scale 拡大率
	void SetScale(const float scale)
	{
		m_scale = g_vec3One;
		m_scale.Scale(scale);
		m_isDirty = true;
	}

	/// <summary>
	/// 明るさを設定。
	/// </summary>
	/// <param name="lum"></param>
	void SetLuminance(float lum)
	{
		m_luminance = lum;
	}

	/// @brief 明るさを徐々に上げる処理
	/// @param addRate 上げる割合
	void FadeIn(const float addRate);

private:
	SkinModelRender* m_skinModelRender = nullptr;	//モデルレンダー
	Texture m_texture;								//テクスチャ
	const wchar_t* m_textureFilePaths;				//テクスチャのファイルパス格納用
	Vector3 m_position = g_vec3Zero;				//座標
	Vector3 m_scale = g_vec3One * 10000.0f;			//拡大率
	Quaternion m_rot = Quaternion::Identity;		//回転
	float m_luminance = 0.0f;						//明るさ
	bool m_isDirty = false;

};

