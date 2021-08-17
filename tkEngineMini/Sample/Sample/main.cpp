#include "stdafx.h"
#include "system/system.h"
#include "Player.h"
#include "Background.h"
#include "HelloFont.h"
#include "Hello2D.h"
#include "HelloLevel2D.h"
#include "PhysicsBall.h"
#include "Seesaw.h"
#include "effect/effect.h"
#include "DirectionLight.h"
#include "Game.h"
#include "Player_new.h"
#include "SkinModelRender.h"




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
	
	//カメラの設定
	g_camera3D->SetPosition({ 0.0f,50.0f,200.0f });
	g_camera3D->SetTarget({ 0.0f,50.0f,0.0f });
	float cameraH = 50.0f;

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//step-1 エフェクトエンジンのインスタンスを作成する。
	EffectEngine::CreateInstance();

	
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//ゲームシーンを作成
	NewGO<Game>(0, "game");

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

		//登録されているゲームオブジェクトの描画関数を呼び出す。
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//step-6 エフェクトのドロー。
		EffectEngine::GetInstance()->Draw();

		g_engine->EndFrame();
	}

	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

