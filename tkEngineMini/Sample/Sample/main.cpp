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
	////ディレクションライトを定義
	//DirectionLight directionLight;

	////ディレクションライトの初期化
	//directionLight.Init({ 1.0f,0.0f,0.0f }, { 0.5f, 0.5f, 0.5f }, { 0.5f,0.5f,0.5f });
	////カメラ位置を取得し
	//Vector3 cameraPos = g_camera3D->GetPosition();
	////メンバに反映
	//directionLight.SetEyePos(cameraPos);

	//float cameraPosH = 0.0f;
	//float cameraPosV = 0.0f;


	//カメラの設定
	g_camera3D->SetPosition({ 0.0f,200.0f,200.0f });
	g_camera3D->SetTarget({ 0.0f,50.0f,0.0f });

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//step-1 エフェクトエンジンのインスタンスを作成する。
	EffectEngine::CreateInstance();
	
	

	////モデルの初期化情報を作成
	//ModelInitData initData;
	////ファイルパスの指定
	//initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	////シェーダーのファイルパスを指定
	//initData.m_fxFilePath = "Assets/shader/model.fx";
	////ディレクションライトの情報を定数バッファとして
	////ディスクリプタヒープに登録するため、初期化情報として渡す
	//initData.m_expandConstantBuffer = directionLight.GetDirLightAddress();
	//initData.m_expandConstantBufferSize = sizeof(directionLight.GetDirLight());
	
	////キャラクターのモデルを↑の初期化情報で初期化
	//Model charaModel;
	//charaModel.Init(initData);
	
	/*Player_new* player = nullptr;
	player = NewGO<Player_new>(0, "pl");*/
	//player->InitDirectionLight(directionLight);
	//player->SetPostion({ 0.0f,0.0f,0.0f });

	/*SkinModelRender* m_skinModelRender = nullptr;
	m_skinModelRender = NewGO<SkinModelRender>(0, "player");
	
	m_skinModelRender->Init("Assets/modelData/unityChan.tkm", enModelUpAxisZ);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain");
	m_skinModelRender->InitDirectionLight(&directionLight);*/

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//step-2 レーザーエフェクトの初期化。
	Effect laserEffect;
	laserEffect.Init(u"Assets/effect/kick.efk");

	Effect laserEffect2;
	laserEffect2.Init(u"Assets/effect/laser2.efk");

	//ゲームシーンを作成
	NewGO<Game>(0, "game");

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		/*float stickLX = g_pad[0]->GetLStickXF();
		float stickLY = g_pad[0]->GetLStickYF();

		cameraPosH += stickLX;
		cameraPosV += stickLY;
		
		Vector3 cameraPos = Vector3::Zero;
		cameraPos.Set(cameraPosH, 0.0f, cameraPosV);

		g_camera3D->SetPosition({cameraPosH,0.0f,cameraPosV});*/


		////ディレクションライトの操作
		////右スティックの水平方向、垂直方向の入力値を受け取る
		//float angleH = g_pad[0]->GetRStickXF();
		//float angleV = g_pad[0]->GetRStickYF();



		//Vector3 dirLigDirection = directionLight.GetDirection();
		//Quaternion rotH;
		//Quaternion rotV;

		//
		//rotH.SetRotationDeg(Vector3::AxisY, angleH);
		//rotH.Apply(dirLigDirection);
		//rotV.SetRotationDeg(Vector3::AxisX, angleV);
		//rotV.Apply(dirLigDirection);

		//////↑で変更したベクトルと回転を反映
		//directionLight.SetDirection(dirLigDirection);

		//エフェクトの発生
		if (g_pad[0]->IsTrigger(enButtonA)) {
			//再生開始。
			laserEffect.Play();
		}
		if (g_pad[0]->IsTrigger(enButtonB)) {
			//再生開始。
			laserEffect2.Play();
		}
		
		//step-3 エフェクトを動かす。	
		auto pos = laserEffect.GetPosition();
		pos.x += g_pad[0]->GetLStickXF();
		pos.z += g_pad[0]->GetLStickYF();

		auto rot = laserEffect.GetRotation();
		rot.AddRotationY(g_pad[0]->GetRStickXF() * 0.1f);

		laserEffect.SetPosition(pos);
		laserEffect.SetRotation(rot);

		pos = laserEffect2.GetPosition();
		pos.x += g_pad[0]->IsPress(enButtonLeft);
		pos.x -= g_pad[0]->IsPress(enButtonRight);
		laserEffect2.SetPosition(pos);


		//step-4 エフェクトのワールド行列を更新する。
		laserEffect.Update();
		laserEffect2.Update();

		//レンダリング開始。
		g_engine->BeginFrame();
		
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

		////キャラモデルのドローコールを実行
		//charaModel.Draw(renderContext);

		g_engine->EndFrame();
	}

	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

