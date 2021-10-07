#include "stdafx.h"
#include "system/system.h"
#include "Bloom.h"
#include "RenderingEngine.h"

namespace
{
	const int WINDOW_WIDTH = 1280;	//幅
	const int WINDOW_HEIGHT = 720;	//高さ

	const char* POSTEFFECT_FILEPATH = "Assets/shader/PostEffect.fx";
	const char* GAUSSIAN_BLUR_EFFECT_FILEPATH = "Assets/shader/GaussianBlur.fx";
}

//重みテーブルの設定
const int NUM_WEIGHTS = 8;
struct SBlurParam
{
	float weights[NUM_WEIGHTS];
};

//関数宣言
void InitRootSignature(RootSignature& rs);

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	RootSignature rs;
	InitRootSignature(rs);

	//カメラの設定
	g_camera3D->SetPosition({ 0.0f,50.0f,200.0f });
	g_camera3D->SetTarget({ 0.0f,50.0f,0.0f });
	float cameraH = 50.0f;

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//step-1 エフェクトエンジンのインスタンスを作成する。
	EffectEngine::CreateInstance();

	//テスト：レンダリングエンジン
	RenderingEngine::CreateInstance();
	//RenderingEngine renderingEngine;
	//レンダリングエンジンの初期化
	RenderingEngine::GetInstance()->Init();
	//renderingEngine.Init();

	//テクスチャを貼りつけるためのスプライトを初期化
	SpriteInitData spriteInitData;
	//テクスチャはメインレンダリングターゲットのカラーバッファ
	spriteInitData.m_textures[0] = &RenderingEngine::GetInstance()->GetRenderTarget().GetRenderTargetTexture();
	spriteInitData.m_width = WINDOW_WIDTH;
	spriteInitData.m_height = WINDOW_HEIGHT;
	//通常のシェーダを指定
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//貼り付けるスプライトの初期化
	Sprite frameBufferSprite;
	frameBufferSprite.Init(spriteInitData);

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//ゲームシーンを作成
	Game* game = nullptr;
	game = NewGO<Game>(0, "game");
	game->Init(*RenderingEngine::GetInstance());

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{

		//レンダリング開始。
		g_engine->BeginFrame();

		//テスト：カメラ上下
		if (g_pad[0]->IsPress(enButtonSelect)) {
			cameraH -= 2.0f;
		}
		else if (g_pad[0]->IsPress(enButtonStart)) {
			cameraH += 2.0f;
		}

		g_camera3D->SetPosition({ 0.0f,cameraH,200.0f });
		
		//登録されているゲームオブジェクトの更新関数を呼び出す。
		GameObjectManager::GetInstance()->ExecuteUpdate();

		//物理ワールドの更新。
		PhysicsWorld::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

		//step-5 エフェクトエンジンの更新。
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());


		//ブルームの処理テスト
		
		//レンダリングターゲットをメインレンダリングターゲットに変更(=オフスクリーンレンダリングにする)
		//レンダリングターゲットとして利用できるまで待つ
		renderContext.WaitUntilToPossibleSetRenderTarget(RenderingEngine::GetInstance()->GetRenderTarget());
		//レンダリングターゲットを設定
		renderContext.SetRenderTargetAndViewport(RenderingEngine::GetInstance()->GetRenderTarget());
		//レンダリングターゲットのクリア
		renderContext.ClearRenderTargetView(RenderingEngine::GetInstance()->GetRenderTarget());
		
		////////モデルのドロー////////
		//登録されているゲームオブジェクトの描画関数を呼び出す。
		//GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		//////////////////////////////
		
		//テスト：レンダリングエンジンによる通常描画
		RenderingEngine::GetInstance()->CommonRender(renderContext);
		
		//テスト：レンダリングエンジンの処理
		RenderingEngine::GetInstance()->Execute(renderContext);

		//メインレンダリングターゲットに描画したものをフレームバッファにコピー
		//レンダリングターゲットをオンスクリーンに戻す
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()		
		);
		frameBufferSprite.Draw(renderContext);

		//step-6 エフェクトのドロー。
		EffectEngine::GetInstance()->Draw();

		g_engine->EndFrame();
	}

	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	//ゲームを削除
	DeleteGO(game);

	return 0;
}

//ルートシグネチャの初期化
void InitRootSignature(RootSignature& rs)
{
	rs.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);
}

