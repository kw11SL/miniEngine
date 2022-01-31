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
	void AddModel(Model* model) 
	{ 
		m_modelsArray.push_back(model); 
	}

	/// @brief シャドウマップを取得
	/// @return シャドウマップ
	RenderTarget& GetShadowMap()
	{
		return m_shadowMap;
	}

	/// @brief モデルを削除
	/// @param model モデル
	void DeleteModel(Model* model);

	/*/// @brief カスケードシャドウ用のシャドウマップに描画するモデルを登録
	/// @param model1 //近景用モデル
	/// @param model2 //中景用モデル
	/// @param model3 //遠景用モデル
	void MultiAddModel(Model& model1,Model& model2,Model& model3) 
	{ 
		m_models[0].push_back(&model1);
		m_models[1].push_back(&model2);
		m_models[2].push_back(&model3);
	}*/

	/// @brief シャドウマップへの描画処理
	/// @param rc レンダリングコンテキスト
	/// @param lightCamera ライトカメラ
	void Render(RenderContext& rc, Camera& lightCamera);
	
	/*/// @brief シャドウマップへの描画処理(カスケードシャドウ用) 
	/// @param rc レンダリングコンテキスト
	/// @param lightCamera ライトカメラ
	void MultiRender(RenderContext& rc, Camera& lightCamera);*/

private:
	/// @brief シャドウマップを初期化
	void InitShadowMap();

	/// @brief カスケードシャドウ用に複数のシャドウマップを初期化
	void InitMultiShadowMap();

private:
	
	//1つだけ影を生成する場合
	std::vector<Model*> m_modelsArray;				//影描画用モデルの可変長配列
	RenderTarget m_shadowMap;						//シャドウマップ
	
	////カスケードシャドウ用
	////距離に合わせたモデルとシャドウマップが必要
	//std::vector<Model*> m_models[NUM_SHADOW_MAP];	//モデルの配列
	//RenderTarget m_shadowMaps[NUM_SHADOW_MAP];		//シャドウマップの配列

};

