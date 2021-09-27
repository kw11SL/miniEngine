#pragma once
class Bloom
{
public:
	Bloom() {};
	~Bloom() {};

	void Init(RenderTarget& mainRenderTatget);
	void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

private:
	RenderTarget m_luminanceRenderTarget;	//輝度抽出用レンダリングターゲット
	Sprite m_luminanceSprite;				//輝度抽出テクスチャ
	GaussianBlur m_gaussBlur[4];			//ガウシアンブラー
	Sprite m_finalSprite;					//最終的に合成するスプライト
};

