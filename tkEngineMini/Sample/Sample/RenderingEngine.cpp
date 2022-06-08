#include "stdafx.h"
#include "RenderingEngine.h"

namespace {
	const int WINDOW_WIDTH = 1280;	//幅
	const int WINDOW_HEIGHT = 720;	//高さ
}

RenderingEngine* RenderingEngine::m_renderingEngine = nullptr;

void RenderingEngine::Init()
{
	//ライトカメラの初期化
	InitLightCamera();
	//メインレンダリングターゲットの初期化
	InitMainRenderTarget();
	//フレームバッファに貼り付けるスプライトの初期化
	InitFrameBufferSprite();
	//シャドウマップの初期化
	InitShadowMap();
	//ポストエフェクトの初期化
	InitPostEffect();
}

void RenderingEngine::InitPostEffect()
{
	//ブルームの初期化
	InitBloom(m_mainRenderTarget);
}

void RenderingEngine::Execute(RenderContext& rc)
{
	///////////////////////////////////////////////////////////////
	//レンダリングターゲットとして利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//レンダリングターゲットを設定
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	//レンダリングターゲットのクリア
	rc.ClearRenderTargetView(m_mainRenderTarget);
	///////////////////////////////////////////////////////////////

	//ライトカメラの更新
	UpdateLightCamera();

	//シャドウマップへの描画
	RenderToShadowMap(rc, m_lightCamera);

	//通常描画
	CommonRendering(rc);

	//エフェクトを描画
	EffectEngine::GetInstance()->Draw();

	//ブルーム処理
	BloomRendering(rc, m_mainRenderTarget);

	//スプライトを描画
	SpriteRendering(rc);

	//フォントを描画
	FontRendering(rc);

	///////////////////////////////////////////////////////////////
	//メインレンダリングターゲットに描画したものをフレームバッファにコピー
	//レンダリングターゲットをオンスクリーンに戻す
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//フレームバッファ用スプライトを描画
	m_frameBufferSprite.Draw(rc);
	///////////////////////////////////////////////////////////////
}

void RenderingEngine::CommonRendering(RenderContext& rc)
{
	//メインレンダリングターゲットが利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//描き込み先をメインレンダリングターゲットにする。
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	////レンダリングターゲットをクリア
	//rc.ClearRenderTargetView(m_mainRenderTarget);

	//モデルをドロー
	for (auto& model : m_commonModels) {
		model->Draw(rc);
	}

	//レンダリングターゲットの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}

void RenderingEngine::RenderToShadowMap(RenderContext& rc , Camera camera)
{
	//シャドウマップに描き込み
	m_shadowMap.Render(rc, camera);
}

void RenderingEngine::BloomRendering(RenderContext& rc, RenderTarget& mainRT)
{
	//ブルーム処理
	m_bloom.Render(rc, mainRT);
}

void RenderingEngine::SpriteRendering(RenderContext& rc)
{
	//メインレンダリングターゲットが利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//描き込み先をメインレンダリングターゲットにする。
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	//レンダリングターゲットをクリア
	//rc.ClearRenderTargetView(m_mainRenderTarget);

	for (auto& sprite : m_sprites) {
		//スプライトを描画
		sprite->Draw(rc);
	}

	//レンダリングターゲットの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}

void RenderingEngine::FontRendering(RenderContext& rc)
{
	//メインレンダリングターゲットが利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//描き込み先をメインレンダリングターゲットにする。
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	//レンダリングターゲットをクリア
	//rc.ClearRenderTargetView(m_mainRenderTarget);

	for (auto& font : m_fontDataVector) {

		//描画開始
		font->font.Begin(rc);

		//フォントを描画
		font->font.Draw(
			font->text,
			font->position,
			font->color,
			font->rotation,
			font->scale,
			font->pivot
		);

		//描画終了
		font->font.End(rc);
	}

	//レンダリングターゲットの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}


void RenderingEngine::InitLightCamera()
{
	m_lightCamera.SetPosition(0.0f, 2000.0f, 0.0f);
	m_lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	m_lightCamera.SetNear(1.0f);
	m_lightCamera.SetFar(20000.0f);
	m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	m_lightCamera.SetViewAngle(Math::DegToRad(120.0f));
	m_lightCamera.Update();
}

void RenderingEngine::UpdateLightCamera()
{
	m_lightCamera.SetPosition(g_camera3D->GetPosition());
	//m_lightCamera.SetTarget(g_camera3D->GetTarget());
	m_lightCamera.SetTarget({ 0.0f,0.0f,0.0f });
	m_lightCamera.SetUp(g_camera3D->GetUp());
	m_lightCamera.Update();
}

void RenderingEngine::InitMainRenderTarget()
{
	m_mainRenderTarget.Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
}

void RenderingEngine::InitFrameBufferSprite()
{
	//初期化データを作成
	SpriteInitData spriteInitData;
	//テクスチャはメインレンダリングターゲット
	spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = WINDOW_WIDTH;
	spriteInitData.m_height = WINDOW_HEIGHT;
	//シェーダーを指定
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//貼り付けるスプライトの初期化
	m_frameBufferSprite.Init(spriteInitData);
}

void RenderingEngine::InitShadowMap()
{
	m_shadowMap.Init();
}

void RenderingEngine::InitBloom(RenderTarget& mainRT)
{
	m_bloom.Init(mainRT);
}

void RenderingEngine::DeleteCommonModel(Model* model)
{
	//イテレータを作成
	std::vector<Model*>::iterator itr;
	
	//モデルを検索
	itr = std::find(
		m_commonModels.begin(),
		m_commonModels.end(),
		model
	);

	//モデルが見つかったら削除
	if (itr != m_commonModels.end()) {
		m_commonModels.erase(itr);
	}
}

void RenderingEngine::DeleteSprite(Sprite* sprite)
{
	//イテレータを作成
	std::vector<Sprite*>::iterator itr;

	//スプライトを検索
	itr = std::find(
		m_sprites.begin(),
		m_sprites.end(),
		sprite
	);

	//スプライトが見つかったら削除
	if (itr != m_sprites.end()) {
		m_sprites.erase(itr);
	}
}

void RenderingEngine::DeleteFonts(SFontData& fontData)
{
	//イテレータを作成
	std::vector<SFontData*>::iterator itr;

	//フォントデータを検索
	itr = std::find(
		m_fontDataVector.begin(),
		m_fontDataVector.end(),
		&fontData
	);

	//フォントデータが見つかったら削除
	if (itr != m_fontDataVector.end()) {
		m_fontDataVector.erase(itr);
	}
}

