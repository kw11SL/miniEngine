#pragma once
#include "Bloom.h"
#include "ShadowMapRender.h"

//フォントデータの構造体
struct SFontData;


/// @brief レンダリングエンジン
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
	void CommonRendering(RenderContext& rc);

	/// @brief シャドウマップに描画
	void RenderToShadowMap(RenderContext& rc,Camera camera);

	/// @brief ブルームの実行
	/// @param rc レンダリングコンテキスト
	void BloomRendering(RenderContext& rc, RenderTarget& mainRT);

	/// @brief スプライトを描画
	/// @param rc 
	void SpriteRendering(RenderContext& rc);

	/// @brief フォントの描画
	/// @param rc レンダリングコンテキスト
	void FontRendering(RenderContext& rc);

	/// @brief ライトカメラの更新
	void UpdateLightCamera();

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

	/// @brief フォントデータの配列にフォントデータを追加
	/// @param fontData 
	void AddFontDataToFonts(SFontData& fontData)
	{
		std::vector<SFontData*>::iterator itr;
		itr = std::find(m_fontDataVector.begin(), m_fontDataVector.end(), &fontData);

		//途中で見つかった場合は配列に追加しない
		if (itr != m_fontDataVector.end()) {
			return;
		}
		//途中で見つからなかった場合(イテレータが終端)まだないので追加
		else if (itr == m_fontDataVector.end()) {
			m_fontDataVector.push_back(&fontData);
		}
	}

	/// @brief スプライトの配列にスプライトを追加
	/// @param sprite 
	void AddSpriteToSprites(Sprite& sprite) 
	{
		std::vector<Sprite*>::iterator itr;
		itr = std::find(m_sprites.begin(),m_sprites.end(),&sprite);

		//途中で見つかった場合は配列に追加しない
		if (itr != m_sprites.end()) {
			return;
		}
		//途中で見つからなかった場合(イテレータが終端)まだないので追加
		else if(itr == m_sprites.end()) {
			m_sprites.push_back(&sprite);
		}

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

	/// @brief スプライトを削除
	/// @param sprite 
	void DeleteSprite(Sprite& sprite);

	/// @brief フォントデータの削除処理
	/// @param fontData 
	void DeleteFonts(SFontData& fontData);

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

	

private:
	static RenderingEngine* m_renderingEngine;	//唯一のオブジェクト

	ShadowMapRender m_shadowMap;				//シャドウマップ
	std::vector<Model*> m_commonModels;			//通常描画用モデル
	std::vector<Sprite*> m_sprites;				//スプライトの配列
	std::vector<SFontData*> m_fontDataVector;	//フォントデータの配列

	RenderTarget m_mainRenderTarget;			//メインレンダリングターゲット
	Bloom m_bloom;								//ブルーム

	Camera m_lightCamera;						//ライトカメラ

};