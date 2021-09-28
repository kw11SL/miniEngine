#pragma once

#include "Bloom.h"

//class bloom;
//class RenderTarget;

class RenderingEngine
{
public:
	RenderingEngine() {}
	~RenderingEngine() {}
	
	/// @brief レンダリングエンジンの初期化
	void Init();
	
	/// @brief 処理を実行 
	/// @param rc レンダリングコンテキスト
	void Execute(RenderContext& rc);

	RenderTarget& GetRenderTarget() { return m_mainRenderTarget; }

private:
	/// @brief メインのレンダリングターゲットの初期化
	void InitMainRenderTarget();
	
	/// @brief ブルームの初期化
	/// @param mainRT レンダリングターゲット
	void InitBloom(RenderTarget& mainRT);

	/// @brief ブルームの実行
	/// @param rc レンダリングコンテキスト
	void BloomRendering(RenderContext& rc, RenderTarget& mainRT);

private:
	RenderTarget m_mainRenderTarget;		//メインレンダリングターゲット
	
	Bloom m_bloom;							//ブルーム

};