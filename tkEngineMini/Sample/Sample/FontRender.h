#pragma once

class RenderingEngine;

struct SFontData {
	Font font;								//フォント
	const wchar_t* text;					//テキスト
	Vector2 position = Vector2::Zero;		//座標
	Vector2 pivot = { 0.5f,0.5f };			//ピボット
	Vector4 color = Vector4::White;			//カラー
	float scale = 1.0f;						//拡大率
	float rotation = 0.0f;					//回転角度
};

class FontRender : public IGameObject
{

public:
	FontRender(){}
	~FontRender();

	/// @brief 初期化処理
	/// @param text 表示するテキスト
	void Init(const wchar_t* text);

	/// @brief 表示するテキストを設定
	/// @param text 
	void SetText(const wchar_t* text)
	{
		m_text = text;
		m_fontData.text = m_text;
	}

	/// @brief テキストを取得
	/// @return 
	const wchar_t* GetText()
	{
		return m_text;
	}

	/// @brief 座標を設定
	/// @param pos 
	void SetPosition(const Vector2& pos)
	{
		m_position = pos;
		//m_fontData.position = m_position;
	}

	/// @brief 座標を取得
	/// @return 
	Vector2& GetPosition()
	{
		return m_position;
	}

	/// @brief ピボットを設定
	/// @param pivot 
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
		//m_fontData.pivot = m_pivot;
	}

	/// @brief ピボットを取得
	/// @return 
	Vector2& GetPivot()
	{
		return m_pivot;
	}

	/// @brief カラーを設定
	/// @param color 
	void SetColor(const Vector4& color)
	{
		m_color = color;
		//m_fontData.color = m_color;
	}

	/// @brief カラーを取得
	/// @return 
	Vector4& GetColor()
	{
		return m_color;
	}

	/// @brief 拡大率を設定
	/// @param scale 
	void SetScale(float scale)
	{
		m_scale = scale;
		//m_fontData.scale = m_scale;
	}

	/// @brief 拡大率を取得
	/// @return 
	float& GetScale()
	{
		return m_scale;
	}

	/// @brief 角度を設定
	/// @param angle 
	void SetAngle(const float angle)
	{
		m_rotation = angle;
		//m_fontData.rotation = m_rotation;
	}

	/// @brief 角度を取得
	/// @return 
	float& GetAngle()
	{
		return m_rotation;
	}

private:
	bool Start()override;
	void Update()override;

	/// @brief フォントデータのメンバを更新
	void UpdateFontData();

private:
	RenderingEngine* m_renderingEngine = nullptr;
	SFontData m_fontData;						//フォントデータの構造体
	Font m_font;								//フォント
	const wchar_t* m_text = L"";				//テキスト
	Vector2 m_position = Vector2::Zero;			//座標
	Vector2 m_pivot = { 0.5f,0.5f };			//ピボット
	Vector4 m_color = Vector4::White;			//カラー
	float m_scale = 1.0f;						//拡大率
	float m_rotation = 0.0f;					//回転角度
};

