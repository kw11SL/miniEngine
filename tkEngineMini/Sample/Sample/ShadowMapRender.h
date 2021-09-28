#pragma once

//class RenderTarget;
class Model;

class ShadowMapRender
{
public:
	/// @brief シャドウマップの初期化
	void Init();

	/// @brief シャドウマップに描画するモデルを登録
	/// @param model 
	void AddModel(Model& model) { m_models[0].push_back(&model); }

	/// @brief 描画処理
	/// @param rc レンダリングコンテキスト
	/// @param lightCamera ライトカメラ
	void Render(RenderContext& rc, Camera& lightCamera);


private:
	//Model* m_model = nullptr;
	std::vector<Model*> m_models[4];
	RenderTarget m_shadowMaps[4];

};

