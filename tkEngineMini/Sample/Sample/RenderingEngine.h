#pragma once

#include "Bloom.h"
#include "ShadowMapRender.h"

//class bloom;
//class RenderTarget;

class RenderingEngine
{
public:
	RenderingEngine() {}
	~RenderingEngine() {}
	
	/// @brief レンダリングエンジンの初期化まとめ
	void Init();
	
	/// @brief 処理を実行 
	/// @param rc レンダリングコンテキスト
	void Execute(RenderContext& rc);

	/// @brief 通常描画処理
	/// @param rc レンダリングコンテキスト
	void CommonRender(RenderContext& rc);

	/// @brief シャドウマップに描画
	void RenderToShadowMap(RenderContext& rc,Camera camera);

	/// @brief レンダリングターゲットを取得
	/// @return レンダリングターゲット
	RenderTarget& GetRenderTarget() 
	{
		return m_mainRenderTarget; 
	}

	/// @brief 影描画用モデルを追加
	/// @param model モデル
	void Add3DModelToShadowModel(Model& model)
	{
		m_shadowMap.AddModel(model);
	}

	/// @brief 通常描画用モデルを追加
	/// @param model 
	void Add3DModelToCommonModel(Model& model)
	{
		m_commonModels.push_back(&model);
	}

private:
	//内部で実行する関数

	/// @brief ライトカメラの初期化
	void InitLightCamera();

	/// @brief メインのレンダリングターゲットの初期化
	void InitMainRenderTarget();

	/// @brief シャドウマップの初期化
	void InitShadowMap();
	
	/// @brief ブルームの初期化
	/// @param mainRT レンダリングターゲット
	void InitBloom(RenderTarget& mainRT);

	/// @brief ブルームの実行
	/// @param rc レンダリングコンテキスト
	void BloomRendering(RenderContext& rc, RenderTarget& mainRT);

private:
	//std::vector<Model*> m_shadowModels;		//影描画用モデル
	ShadowMapRender m_shadowMap;				//シャドウマップ
	std::vector<Model*> m_commonModels;			//通常描画用モデル

	RenderTarget m_mainRenderTarget;			//メインレンダリングターゲット
	Bloom m_bloom;								//ブルーム

	Camera m_lightCamera;						//ライトカメラ

};