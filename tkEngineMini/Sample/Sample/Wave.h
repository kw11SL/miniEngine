#pragma once
class Wave
{
public:
	/// @brief 数字スプライト用の構造体
	struct SNumSprite {
		SpriteRender* numSprite = nullptr;			//数字スプライト
		SpriteRender* numShadowSprite = nullptr;	//影スプライト
		float spriteScaleRate = 0.5f;				//拡大率
	};


	Wave(){}
	~Wave();

	/// @brief 初期化処理
	void Init();

	/// @brief wave数スプライトがフェードインしながら縮小してくる処理
	/// @param sNumSprite スプライトの構造体
	void FadeinWithScalingWaveSprite(SNumSprite* sNumSprite);

	/// @brief wave数スプライトを透明にする(=見えなくする)処理
	/// @param sNumSprite スプライトの構造体
	void NumSpriteClear(SNumSprite* sNumSprite);

	/// @brief 更新処理
	void Update();

private:
	std::vector<SNumSprite*> m_numSprites;				//数字の構造体を格納する可変長配列

	SpriteRender* m_waveTextSprite = nullptr;			//waveの文字スプライト
	SpriteRender* m_waveTextShadowSprite = nullptr;		//waveの文字の影スプライト
	
	float m_numSpriteScaleRate = 0.0f;					//wave数スプライトの拡大率
	int m_waveNumPrevFrame = 1;							//前フレームのwave数を取得
};

