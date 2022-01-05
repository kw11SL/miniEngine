#pragma once
class Timer
{
public:
	Timer(){}
	~Timer();

	/// @brief 初期化処理 
	void Init();

	/// @brief ゲームディレクターから取得したスコアを更新する
	void UpdateTime();

	/// @brief 座標を設定(スプライト)
	/// @param pos 座標
	void SetPositionSprite(const Vector3& pos)
	{
		m_timeSprite->SetPosition(pos);
	}

	/// @brief 座標を設定
	/// @param pos 座標
	void SetPosition(const Vector2& pos)
	{
		m_timeNumber->SetPosition(pos);
	}

	/// @brief カラーを設定
	/// @param color 
	void SetColor(const Vector4& color)
	{

		m_timeNumber->SetColor(color);
	}

	/// @brief 拡大率を設定
	/// @param scaleRate 
	void SetScale(const float scaleRate)
	{
		m_timeNumber->SetScale(scaleRate);
	}

	/// @brief 回転を設定
	/// @param rotate 
	void SetRotate(const float rotate)
	{
		m_timeNumber->SetAngle(rotate);
	}

	/// @brief ピボットを設定
	/// @param pivot 
	void SetPivot(const Vector2& pivot)
	{
		m_timeNumber->SetPivot(pivot);
	}

private:
	SpriteRender* m_timeSprite = nullptr;
	SpriteRender* m_timeSpriteShadow = nullptr;

	FontRender* m_timeNumber = nullptr;		//スコアの数値表示部分
	FontRender* m_timeNumberOutline = nullptr;

	Vector3 m_timeSpritePos = Vector3::Zero;
	Vector2 m_timeNumPos = Vector2::Zero;	//スコアの数値部分の座標

	std::wstring m_timeNumWs = L"hoge";		//ポインタ保持用のメンバ


};

