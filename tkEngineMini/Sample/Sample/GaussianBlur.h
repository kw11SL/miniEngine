#pragma once
class GaussianBlur
{
public:
	GaussianBlur() {}
	~GaussianBlur() {}

	/// @brief テクスチャの初期化
	/// @param baseTexture ブラーを掛ける元となるテクスチャ
	void Init(Texture* baseTexture);
	
	/// @brief ガウシアンブラーをGPU上で実行
	/// @param rc レンダリングターゲット
	/// @param blurPower ブラーの強度。値が大きいほどボケる
	void ExecuteOnGPU(RenderContext& rc, float blurPower);

	/// @brief 縦、横ブラーを適用したテクスチャを取得
	/// @return ブラーを適用したテクスチャ
	Texture& GetExecutedTexture()
	{
		return m_yBlurRenderTarget.GetRenderTargetTexture();
	}

private:
	//内部で使用する関数
	
	/// @brief レンダリングターゲットの初期化
	void InitRenderTargets();
	
	/// @brief スプライトの初期化
	void InitSprites();
	
	/// @brief 重みテーブルを更新 
	/// @param blurPower ブラーの強度。値が大きいほどボケる
	void UpdateWeightsTable(float blurPower);

private:
	enum {NUM_WEIGHTS = 8};						//重みの数
	float m_weights[NUM_WEIGHTS] = {0.0f};		//重みテーブル
	Texture* m_baseTexture = nullptr;			//ブラーを適用するベースのテクスチャ
	RenderTarget m_xBlurRenderTarget;			//横ブラー画像を描画するレンダリングターゲット
	RenderTarget m_yBlurRenderTarget;			//縦ブラー画像を描画するレンダリングターゲット
	Sprite m_xBlurSprite;						//横ブラー画像を描画するためのスプライト
	Sprite m_yBlurSprite;						//縦ブラー画像を描画するためのスプライト
};

