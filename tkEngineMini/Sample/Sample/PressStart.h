#pragma once
class PressStart
{
public:
	PressStart() {}
	~PressStart();

	/// @brief 初期化処理
	void Init();

	/// @brief フェードインが完了したかどうか
	/// @return フェードイン完了フラグ
	const bool& GetIsFinishFadeIn() const
	{
		return m_isFinishFadeIn;
	}
	
	/// @brief フェードアウトが完了したかどうか
	/// @return フェードアウト完了フラグ
	const bool& GetIsFinishFadeOut() const
	{
		return m_isFinishFadeOut;
	}

	const bool& GetIsPressStart() const
	{
		return m_isPressStart;
	}

	/// @brief 更新処理
	void Update();

private:
	SpriteRender* m_pressStartSprite = nullptr;		//文字のスプライト
	bool m_isFinishFadeIn = false;					//フェードイン完了フラグ
	bool m_isFinishFadeOut = true;					//フェードアウト完了フラグ
	bool m_isPressStart = false;					//ボタンが押されたフラグ
};

