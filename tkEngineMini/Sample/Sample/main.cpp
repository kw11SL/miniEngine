#include "stdafx.h"
#include "system/system.h"
#include "Bloom.h"
#include "RenderingEngine.h"

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
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("SurfaceSweeper"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	RootSignature rs;
	InitRootSignature(rs);
	
	//カメラの遠平面を設定
	g_camera3D->SetFar(500000.0f);

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	//物理ワールドのインスタンスを作成
	PhysicsWorld::CreateInstance();
	//サウンドエンジンを作成,初期化
	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();
	//ゲームディレクターを作成
	GameDirector::CreateInstance();
	//バレットマネージャを作成
	BulletManager::CreateInstance();
	//爆発マネージャを作成
	ExplosionManager::CreateInstance();
	//エネミーマネージャを作成
	EnemyManager::CreateInstance();

	//step-1 エフェクトエンジンのインスタンスを作成する。
	EffectEngine::CreateInstance();
	//テスト：レンダリングエンジン
	RenderingEngine::CreateInstance();
	//レンダリングエンジンの初期化
	RenderingEngine::GetInstance()->Init();

	//タイトルの作成
	NewGO<Title>(0, TITLE_NAME);

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{	

#ifdef DEBUG_ON //stdafx.h内のデバッグフラグを参照
		
		//テスト：ゲームの削除
		if(g_pad[0]->IsTrigger(enButtonStart)){
			QueryGOs<Game>(GAME_SCENE_NAME, [&](Game* gameScene) {
				
				//ゲームを削除
				DeleteGO(gameScene);
				//ゲームを初期状態に戻す
				GameDirector::GetInstance()->ResetGame();
				//バンク内ファイルを消去
				//g_engine->ClearFileBank();

				//バレットマネージャ内からすべての弾を消去
				BulletManager::GetInstance()->DeleteBullets();
				//爆発マネージャ内からすべての爆発を消去
				ExplosionManager::GetInstance()->DeleteExplosions();
				//エネミーマネージャ内からすべてのエネミーを消去
				EnemyManager::GetInstance()->DeleteEnemies();

				//タイトルをNewGO
				NewGO<Title>(0, TITLE_NAME);

				//問い合わせ終了
				return false;
			});

			
		}

		// テスト：ポーズ状態の切り替え
		if (g_pad[0]->IsTrigger(enButtonSelect)) {
			if (GameDirector::GetInstance()->GetGameState() == enGame) {
				GameDirector::GetInstance()->SetGameState(enPause);
			}
			else if(GameDirector::GetInstance()->GetGameState() == enPause) {
				GameDirector::GetInstance()->SetGameState(enGame);
			}
		}
#endif // DEBUG_ON

		//レンダリング開始。
		g_engine->BeginFrame();

		/////////////////////////////////////////////////////////////////////////////////
		//各種更新処理
		
		//登録されているゲームオブジェクトの更新関数を呼び出す。
		GameObjectManager::GetInstance()->ExecuteUpdate();
		//ゲームディレクターの更新処理
		GameDirector::GetInstance()->ExecuteUpdate();
		//バレットマネージャの更新処理
		BulletManager::GetInstance()->ExecuteUpdate();
		//爆発マネージャの更新処理
		ExplosionManager::GetInstance()->ExecuteUpdate();
		//エネミーマネージャの更新処理
		EnemyManager::GetInstance()->ExecuteUpdate();
		//物理ワールドの更新。
		PhysicsWorld::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());
		//step-5 エフェクトエンジンの更新。
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());
		/////////////////////////////////////////////////////////////////////////////////
		
		//レンダリングエンジンの処理
		RenderingEngine::GetInstance()->Execute(renderContext);
		
		//フレーム終了
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
	//爆発マネージャを削除
	ExplosionManager::DeleteInstance();
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

