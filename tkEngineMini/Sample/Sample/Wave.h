#pragma once
class Wave
{
public:
	Wave(){}
	~Wave();

	/// @brief 初期化処理
	void Init();

	/// @brief 現在のwave数と同じスプライトに切り替える処理
	void SwitchingWaveSprite();

	/// @brief wave数スプライトがフェードインしながら縮小してくる処理
	void FadeinWithScalingWaveSprite();

	/// @brief 更新処理
	void Update();

private:
	SpriteRender* m_waveTextSprite = nullptr;			//waveの文字スプライト
	SpriteRender* m_waveTextShadowSprite = nullptr;		//waveの文字の影スプライト
	
	SpriteRender* m_waveNumSprite = nullptr;			//wave数のスプライト
	SpriteRender* m_waveNumShadowSprite = nullptr;		//wave数の影スプライト

	float m_numSpriteScaleRate = 0.0f;					//wave数スプライトの拡大率
};

