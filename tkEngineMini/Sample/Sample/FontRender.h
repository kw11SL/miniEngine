#pragma once
class FontRender : public IGameObject
{
private:
	
	struct SFontData {
		Font m_font;								//フォント
		const wchar_t* m_text;						//テキスト
		Vector2 m_position = Vector2::Zero;			//座標
		Vector2 m_pivot = { 0.5f,0.5f };			//ピボット
		Vector4 m_color = Vector4::White;			//カラー
		float m_scale = 1.0f;						//拡大率
		float m_rotation = 0.0f;					//回転角度
	};

public:
	FontRender(){}
	~FontRender(){}

	/// @brief 初期化処理
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

	/// @brief 表示するテキストを設定
	/// @param text 
	void SetText(const wchar_t* text)
	{
		m_fontData.m_text = text;
	}

	/// @brief テキストを取得
	/// @return 
	const wchar_t* GetText()
	{
		return m_fontData.m_text;
	}

	/// @brief 座標を設定
	/// @param pos 
	void SetPosition(const Vector2& pos)
	{
		m_fontData.m_position = pos;
	}

	/// @brief 座標を取得
	/// @return 
	Vector2& GetPosition()
	{
		return m_fontData.m_position;
	}

	/// @brief ピボットを設定
	/// @param pivot 
	void SetPivot(const Vector2& pivot)
	{
		m_fontData.m_pivot = pivot;
	}

	/// @brief ピボットを取得
	/// @return 
	Vector2& GetPivot()
	{
		return m_fontData.m_pivot;
	}

	/// @brief カラーを設定
	/// @param color 
	void SetColor(const Vector4& color)
	{
		m_fontData.m_color = color;
	}

	/// @brief カラーを取得
	/// @return 
	Vector4& GetColor()
	{
		return m_fontData.m_color;
	}

	/// @brief 拡大率を設定
	/// @param scale 
	void SetScale(float scale)
	{
		m_fontData.m_scale = scale;
	}

	/// @brief 拡大率を取得
	/// @return 
	float& GetScale()
	{
		return m_fontData.m_scale;
	}

	/// @brief 角度を設定
	/// @param angle 
	void SetAngle(const float angle)
	{
		m_fontData.m_rotation = angle;
	}

	/// @brief 角度を取得
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

