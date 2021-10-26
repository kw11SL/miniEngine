#pragma once

#include "Bloom.h"
#include "ShadowMapRender.h"

//class bloom;
//class RenderTarget;

//シングルトンパターン
class RenderingEngine
{
public:

	/// @brief オブジェクトの作成
	static void CreateInstance()
	{
		if (m_renderingEngine == nullptr) {
			m_renderingEngine = new RenderingEngine;
		}
	}
	
	/// @brief オブジェクトの取得
	/// @return 自身のオブジェクト
	static RenderingEngine* GetInstance()
	{
		return m_renderingEngine;
	}
	
	/// @brief オブジェクトの削除
	static void DeleteInstance()
	{
		delete m_renderingEngine;
		m_renderingEngine = nullptr;
	}

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

	/// @brief シャドウマップレンダー内のシャドウマップを取得
	/// @return シャドウマップ
	RenderTarget& GetShadowMap()
	{
		return m_shadowMap.GetShadowMap();
	}

	/// @brief ライトカメラを取得
	/// @return ライトカメラ
	Camera& GetLightCamera()
	{
		return m_lightCamera;
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

	/// @brief 通常描画モデルの削除処理
	/// @param model 
	void DeleteCommonModel(Model& model);

	/// @brief	影描画用モデルの削除処理
	/// @param model 
	void DeleteShadowModel(Model& model)
	{
		m_shadowMap.DeleteModel(model);
	}

private:
	//内部で実行する関数
	RenderingEngine() {}
	~RenderingEngine() {}


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
	static RenderingEngine* m_renderingEngine;	//唯一のオブジェクト

	//std::vector<Model*> m_shadowModels;		//影描画用モデル
	ShadowMapRender m_shadowMap;				//シャドウマップ
	std::vector<Model*> m_commonModels;			//通常描画用モデル


	RenderTarget m_mainRenderTarget;			//メインレンダリングターゲット
	Bloom m_bloom;								//ブルーム

	Camera m_lightCamera;						//ライトカメラ

};