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

	g_camera3D->SetFar(500000.0f);

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	//物理ワールドのインスタンスを作成
	PhysicsWorld::CreateInstance();
	//サウンドエンジンを作成
	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();
	//ゲームディレクターを作成
	GameDirector::CreateInstance();
	//バレットマネージャを作成
	BulletManager::CreateInstance();
	//エネミーマネージャを作成
	EnemyManager::CreateInstance();

	//step-1 エフェクトエンジンのインスタンスを作成する。
	EffectEngine::CreateInstance();
	//テスト：レンダリングエンジン
	RenderingEngine::CreateInstance();
	//レンダリングエンジンの初期化
	RenderingEngine::GetInstance()->Init();

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

	//タイトルの作成
	NewGO<Title>(0, "title");

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{	
		//テスト：ゲームの削除
		if(g_pad[0]->IsTrigger(enButtonStart)){
			QueryGOs<Game>("game", [&](Game* gameScene) {
				
				//ゲームを削除
				DeleteGO(gameScene);
				//ゲームを初期状態に戻す
				GameDirector::GetInstance()->ResetGame();
				//バンク内ファイルを消去
				//g_engine->ClearFileBank();

				//タイトルをNewGO
				NewGO<Title>(0, "title");

				//問い合わせ終了
				return false;
			});

			//バレットマネージャ内からすべての弾を消去
			BulletManager::GetInstance()->DeleteBullets();
		}

		//// テスト：ポーズ状態の切り替え
		//if (g_pad[0]->IsTrigger(enButtonSelect)) {
		//	if (GameDirector::GetInstance()->GetGameState() == enGame) {
		//		GameDirector::GetInstance()->SetGameState(enPause);
		//	}
		//	else if(GameDirector::GetInstance()->GetGameState() == enPause) {
		//		GameDirector::GetInstance()->SetGameState(enGame);
		//	}
		//}

		////テスト：ゲーム開始からゲーム中への変更
		//if (g_pad[0]->IsTrigger(enButtonA)) {
		//	if (GameDirector::GetInstance()->GetGameState() == enStart) {
		//		GameDirector::GetInstance()->SetGameState(enGame);
		//	}
		//}

		//レンダリング開始。
		g_engine->BeginFrame();

		//登録されているゲームオブジェクトの更新関数を呼び出す。
		GameObjectManager::GetInstance()->ExecuteUpdate();

		//ゲームディレクターの更新処理
		GameDirector::GetInstance()->ExecuteUpdate();

		//バレットマネージャの更新処理
		BulletManager::GetInstance()->ExecuteUpdate();

		//物理ワールドの更新。
		PhysicsWorld::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

		//step-5 エフェクトエンジンの更新。
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());
		
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

		//テスト：レンダリングエンジンの処理
		RenderingEngine::GetInstance()->Execute(renderContext);

		////step-6 エフェクトのドロー。
		//EffectEngine::GetInstance()->Draw();

		//メインレンダリングターゲットに描画したものをフレームバッファにコピー
		//レンダリングターゲットをオンスクリーンに戻す
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()		
		);
		frameBufferSprite.Draw(renderContext);

		

		g_engine->EndFrame();
	}

	////////////////////////////////////////////////////
	//マネージャ、エンジンの削除
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	//レンダリングエンジンを削除
	RenderingEngine::DeleteInstance();
	//サウンドエンジンを削除
	CSoundEngine::DeleteInstance();
	//ゲームディレクターを削除
	GameDirector::DeleteInstance();
	//バレットマネージャを削除
	BulletManager::DeleteInstance();
	//エネミーマネージャを削除
	EnemyManager::DeleteInstance();
	////////////////////////////////////////////////////

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

