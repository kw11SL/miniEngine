#include "stdafx.h"
#include "system/system.h"

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


	////ブルーム実装テスト

	////メインレンダリングターゲットの設定
	////カラーバッファのフォーマットは32bit浮動小数点
	//RenderTarget mainRenderTarget;
	//mainRenderTarget.Create(
	//	WINDOW_WIDTH,
	//	WINDOW_HEIGHT,
	//	1,
	//	1,
	//	DXGI_FORMAT_R32G32B32A32_FLOAT,
	//	DXGI_FORMAT_D32_FLOAT
	//);

	////輝度抽出用レンダリングターゲットの設定
	//RenderTarget luminanceRenderTarget;
	//luminanceRenderTarget.Create(
	//	WINDOW_WIDTH,
	//	WINDOW_HEIGHT,
	//	1,
	//	1,
	//	DXGI_FORMAT_R32G32B32A32_FLOAT,
	//	DXGI_FORMAT_D32_FLOAT
	//);
	////輝度抽出用スプライトを初期化
	//SpriteInitData luminanceSpriteInitData;

	//luminanceSpriteInitData.m_fxFilePath = POSTEFFECT_FILEPATH;
	//luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	//luminanceSpriteInitData.m_width = WINDOW_WIDTH;
	//luminanceSpriteInitData.m_height = WINDOW_HEIGHT;
	////テクスチャはメインレンダリングターゲット
	//luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	////書き込むレンダリングターゲットのフォーマットを指定
	//luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	////スプライトを初期化
	//Sprite luminanceSprite;
	//luminanceSprite.Init(luminanceSpriteInitData);

	////ガウシアンブラーの初期化
	//GaussianBlur gaussBlur;
	////輝度抽出用レンダリングターゲットのテクスチャで初期化(スプライトの方ではない)
	//gaussBlur.Init(&luminanceRenderTarget.GetRenderTargetTexture());

	////ブラー画像を加算合成するためのスプライトを初期化
	//SpriteInitData addBrendSpriteInitData;
	////テクスチャはブラー処理をした輝度抽出テクスチャ
	//addBrendSpriteInitData.m_textures[0] = &gaussBlur.GetExecutedTexture();
	//addBrendSpriteInitData.m_width = WINDOW_WIDTH;
	//addBrendSpriteInitData.m_height = WINDOW_HEIGHT;
	////通常のシェーダを指定
	//addBrendSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	////アルファブレンドモードを加算合成にする
	//addBrendSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//addBrendSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	////加算合成スプライトの初期化
	//Sprite addBrendSprite;
	//addBrendSprite.Init(addBrendSpriteInitData);

	////テクスチャを貼りつけるためのスプライトを初期化
	//SpriteInitData spriteInitData;
	////テクスチャはメインレンダリングターゲットのカラーバッファ
	//spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//spriteInitData.m_width = WINDOW_WIDTH;
	//spriteInitData.m_height = WINDOW_HEIGHT;
	////通常のシェーダを指定
	//spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	////貼り付けるスプライトの初期化
	//Sprite frameBufferSprite;
	//frameBufferSprite.Init(spriteInitData);

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

		////ブルームの処理テスト
		//
		////レンダリングターゲットをメインレンダリングターゲットに変更(=オフスクリーンレンダリングにする)
		////レンダリングターゲットとして利用できるまで待つ
		//renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		////レンダリングターゲットを設定
		//renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		////レンダリングターゲットのクリア
		//renderContext.ClearRenderTargetView(mainRenderTarget);

		////////モデルのドロー
		//登録されているゲームオブジェクトの描画関数を呼び出す。
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		////メインレンダリングターゲットへの書き込み終了待ち
		//renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		////輝度抽出処理
		////輝度抽出用レンダリングターゲットに変更
		//renderContext.WaitUntilToPossibleSetRenderTarget(luminanceRenderTarget);
		////レンダリングターゲットを設定
		//renderContext.SetRenderTargetAndViewport(luminanceRenderTarget);
		////レンダリングターゲットをクリア
		//renderContext.ClearRenderTargetView(luminanceRenderTarget);
		////輝度抽出を行う
		//luminanceSprite.Draw(renderContext);
		////レンダリングターゲットへの書き込み終了待ち
		//renderContext.WaitUntilFinishDrawingToRenderTarget(luminanceRenderTarget);

		////ガウシアンブラーの実行
		//gaussBlur.ExecuteOnGPU(renderContext, 20);

		////ブラー画像をメインレンダリングターゲットに加算合成
		////レンダリングターゲットとして利用できるまで待つ
		//renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		////レンダリングターゲットを設定
		//renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		////合成(=スプライトの描画)
		//addBrendSprite.Draw(renderContext);
		////レンダリングターゲットへの書き込み終了待ち
		//renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		////メインレンダリングターゲットに描画したものをフレームバッファにコピー
		////レンダリングターゲットをオンスクリーンに戻す
		//renderContext.SetRenderTarget(
		//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()		
		//);
		//frameBufferSprite.Draw(renderContext);

		//step-6 エフェクトのドロー。
		EffectEngine::GetInstance()->Draw();

		g_engine->EndFrame();
	}

	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
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

