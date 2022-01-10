#pragma once


/// @brief スコア表示クラス。UIクラスで使用
class Score
{
public:
	Score() {};
	~Score();
	
	/// @brief 初期化処理 
	void Init();

	/// @brief ゲームディレクターから取得したスコアを更新する
	void UpdateScore();

	/// @brief 座標を設定
	/// @param pos 
	void SetPosition(const Vector2& pos)
	{
		m_scoreNumber->SetPosition(pos);
	}

	/// @brief カラーを設定
	/// @param color 
	void SetColor(const Vector4& color)
	{
		m_scoreNumber->SetColor(color);
	}

	/// @brief 拡大率を設定
	/// @param scaleRate 
	void SetScale(const float scaleRate)
	{
		m_scoreNumber->SetScale(scaleRate);
	}

	/// @brief 回転を設定
	/// @param rotate 
	void SetRotate(const float rotate)
	{
		m_scoreNumber->SetAngle(rotate);
	}

	/// @brief ピボットを設定
	/// @param pivot 
	void SetPivot(const Vector2& pivot)
	{
		m_scoreNumber->SetPivot(pivot);
	}

private:
	SpriteRender* m_scoreSprite = nullptr;			//スコアのテキスト部分のスプライト
	SpriteRender* m_scoreShadowSprite = nullptr;	//影部分のスプライト
	
	FontRender* m_scoreNumber = nullptr;			//スコアの数値表示部分
	FontRender* m_scoreNumberShadow = nullptr;		//スコアの数値表示部分の影スプライト

	Vector3 m_scoreSpritePos = Vector3::Zero;
	Vector2 m_scoreTextPos = Vector2::Zero;			//スコアのテキスト部分の座標

	std::wstring m_scoreNumWs = L"hoge";			//ポインタ保持用のメンバ

	float m_fadeOutFraction = 1.0f;
};

