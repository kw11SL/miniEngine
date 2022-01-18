#pragma once

/// @brief タイムアップ時のリザルト表示クラス
class Result
{
public:
	Result();
	~Result();

	/// @brief 初期化処理
	void Init();

	/// @brief スプライトを順にフェードインさせる処理
	void FadeInSprite();

	/// @brief 文字を順にフェードインさせる処理
	void FadeInText();

	/// @brief 文字をフェードアウトさせる処理
	void FadeOutText(const float fadeOutRate);

	/// @brief 更新処理
	void Update();

private:
	SpriteRender* m_screenSprite = nullptr;					//背景を暗くするスプライト
	SpriteRender* m_screenTopSprite = nullptr;				//フェードアウト用黒スプライト
	SpriteRender* m_resultSprite = nullptr;					//リザルトの文字スプライト
	SpriteRender* m_pressButtonSprite = nullptr;			//タイトルに戻るボタンのスプライト
	
	SpriteRender* m_remainPlayerTextSprite = nullptr;		//残機数表示テキストスプライト
	FontRender* m_remainPlayerNum = nullptr;				//残機数
	std::wstring m_remainPlayerNumWs = L"";
	FontRender* m_remainPlayerBonusText = nullptr;			//残機ボーナスのテキスト
	FontRender* m_remainPlayerBonusNum = nullptr;			//残機数ボーナス数値
	std::wstring m_remainPlayerBonusNumWs = L"";

	SpriteRender* m_destructionRateTextSprite = nullptr;	//敵総数に対する撃破率
	FontRender* m_destructionRateNum = nullptr;				//撃破率数値
	std::wstring m_destructionRateWs = L"";
	FontRender* m_destructionBonusText = nullptr;			//ボーナスのテキスト
	FontRender* m_destructionBonusNum = nullptr;			//撃破率ボーナス数値
	std::wstring m_destructionBonusWs = L"";

	SpriteRender* m_finalScoreTextSprite = nullptr;			//最終スコアテキストスプライト
	FontRender* m_finalScoreNum = nullptr;					//最終スコア数値
	std::wstring m_finalScoreNumWs = L"";

	bool m_isFinishFadeInSprite = false;
	bool m_isFinishFadeInText = false;
	bool m_exitFlag = false;
};

