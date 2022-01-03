#pragma once


class TitleSprite
{
public:
	TitleSprite(){}
	~TitleSprite();

	/// @brief 初期化処理
	void Init();

	/// @brief フェードアウト処理
	void FadeOutSprite();

	/// @brief スプライトを取得
	/// @return タイトルスプライト
	const SpriteRender* GetTitleSprite() const
	{
		return m_titleSprite;
	}

	/// @brief フェードインが完了したかどうか
	/// @return フェードイン完了フラグ
	const bool GetIsFinishFadeIn() const
	{
		return m_isFinishFadeIn;
	}

	/// @brief フェードインイン→フェードアウトが完了したかどうか
	/// @return フェードアウト完了フラグ
	const bool GetIsFinishFadeOut() const
	{
		return m_isFinishFadeOut;
	}

	/// @brief 更新処理
	void Update();

private:
	SpriteRender* m_titleSprite = nullptr;
	Vector4 m_titleSpriteColor = { 1.0f,1.0f,1.0f,0.0f };
	Vector3 m_titleSpriteScale = Vector3::One;
	Vector3 m_titlePos = Vector3::Zero;

	bool m_isFinishFadeIn = false;
	bool m_isFinishFadeOut = false;

};

