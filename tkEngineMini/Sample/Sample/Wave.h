#pragma once
class Wave
{
public:
	Wave(){}
	~Wave();

	void Init();

	void Update();

private:
	SpriteRender* m_waveTextSprite = nullptr;
	SpriteRender* m_waveNumSprite = nullptr;

	float m_numSpriteScaleRate = 0.0f;				//wave数スプライトの拡大率

	int m_waveNum = 1;
};

