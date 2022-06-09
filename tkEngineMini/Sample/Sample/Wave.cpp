#include "stdafx.h"
#include "Wave.h"

namespace {

	//wave数の最大値。スプライトの初期化に使用。
	const int WAVE_NUM_MAX = 9;

	//ファイルパス名
	const char* TEXT_SPRITE_FILEPATH = "Assets/sprite/ui/wave.dds";							//waveの文字スプライトのファイルパス
	const char* NUM_SPRITE_FILEPATH = "Assets/sprite/ui/1.dds";								//wave数のスプライトのファイルパス
	const char* NUM_SPRITE_FRAME_FILEPATH = "Assets/sprite/ui/weaponIcon_frame.dds";		//wave数の枠のスプライト

	//wave数スプライト初期化用ファイルパスの前半と拡張子部分
	const std::string FILEPATH_BASE_1 = "Assets/sprite/ui/";
	const std::string FILEPATH_BASE_2 = ".dds";
	
	//初期位置
	const Vector3 TEXT_SPRITE_POS = { -580.0f,320.0f,0.0f };
	const Vector3 TEXT_SPRITE_SHADOW_OFFSET = { 7.0f,-7.0f,0.0f };
	const Vector3 NUM_SPRITE_INIT_POS = { -520.0f,280.0f,0.0f };
	const Vector3 NUM_SPRITE_SHADOW_OFFSET = { 7.0f,-7.0f,0.0f };
	
	//拡大率と拡大率係数
	const Vector3 TEXT_SPRITE_SCALE = Vector3::One;
	const float TEXT_SCALE_RATE = 0.3f;
	const Vector3 NUM_SPRITE_SCALE = Vector3::One;
	const float NUM_SCALE_RATE = 0.5f;
	const float NUM_FRAME_SCALE_RATE = 0.35f;
	
	//ピボット
	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };
	
	//カラー
	const Vector4 TEXT_SPRITE_INIT_COLOR = { 0.0f,1.0f,0.85f,1.0f };
	const Vector4 TEXT_SPRITE_SHADOW_INIT_COLOR = { 0.0f*0.3f,1.0f*0.3f,0.85f*0.3f,1.0f*0.7f };
	const Vector4 NUM_SPRITE_INIT_COLOR = { 0.0f,1.0f,0.85f,0.0f };
	const Vector4 NUM_SPRITE_INIT_SHADOW_COLOR = { 0.0f*0.3f,1.0f*0.3f,0.85f*0.3f,0.0f };
	
	//スプライトの幅と高さ
	const int TEXT_SPRITE_WIDTH = 256;
	const int TEXT_SPRITE_HEIGHT = 128;
	const int NUM_SPRITE_WIDTH = 256;
	const int NUM_SPRITE_HEIGHT = 256;
}

Wave::~Wave()
{
	//数字スプライトの削除
	//可変長配列のスプライトを削除し、構造体自体も削除する
	for (auto& sNumSprites : m_numSprites) {
		DeleteGO(sNumSprites->numSprite);
		DeleteGO(sNumSprites->numShadowSprite);

		delete sNumSprites;
		sNumSprites = nullptr;
	}
	
	//文字スプライトを削除
	DeleteGO(m_waveTextSprite);
	DeleteGO(m_waveTextShadowSprite);
}

void Wave::Init()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//文字スプライトの初期化
	
	//waveの文字の影スプライトの初期化
	m_waveTextShadowSprite = NewGO<SpriteRender>(0);
	m_waveTextShadowSprite->Init(
		TEXT_SPRITE_FILEPATH,
		TEXT_SPRITE_WIDTH,
		TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//ピボット、拡大率、位置、色を設定
	m_waveTextShadowSprite->SetPivot(SPRITE_PIVOT);
	m_waveTextShadowSprite->SetScale(TEXT_SPRITE_SCALE * TEXT_SCALE_RATE);
	//ちょっとずらす
	m_waveTextShadowSprite->SetPosition(TEXT_SPRITE_POS + TEXT_SPRITE_SHADOW_OFFSET);
	m_waveTextShadowSprite->SetColor(TEXT_SPRITE_SHADOW_INIT_COLOR);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//waveの文字スプライトの初期化
	m_waveTextSprite = NewGO<SpriteRender>(0);
	m_waveTextSprite->Init(
		TEXT_SPRITE_FILEPATH,
		TEXT_SPRITE_WIDTH,
		TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//ピボット、拡大率、位置、色を設定
	m_waveTextSprite->SetPivot(SPRITE_PIVOT);
	m_waveTextSprite->SetScale(TEXT_SPRITE_SCALE * TEXT_SCALE_RATE);
	m_waveTextSprite->SetPosition(TEXT_SPRITE_POS);
	m_waveTextSprite->SetColor(TEXT_SPRITE_INIT_COLOR);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//数字スプライトの用意
	//可変長配列内に数字スプライトの構造体を作成していく
	for (int i = 0; i < WAVE_NUM_MAX; i++) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ファイルパスを作成
		std::string numStr = std::to_string(i);
		//string型でファイルパスを構成するため、パス + (wave数) + 拡張子部分で文字を結合する
		std::string convFilePath = FILEPATH_BASE_1 + numStr + FILEPATH_BASE_2;
		//できたstring型のファイルパスをconst char*型に変換
		const char* newFilePath = convFilePath.c_str();

		//可変長配列内に構造体を作成
		m_numSprites.push_back(new SNumSprite);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//数字の影スプライトの初期化
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite = NewGO<SpriteRender>(0);
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->Init(
			newFilePath,
			NUM_SPRITE_WIDTH,
			NUM_SPRITE_HEIGHT,
			AlphaBlendMode_Trans
		);

		//拡大率変数をリセット
		m_numSprites[m_numSprites.size() - 1]->spriteScaleRate = NUM_SCALE_RATE;
		
		//ピボット、拡大率、位置、色を設定
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->SetPivot(SPRITE_PIVOT);
		//ちょっとずらす
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->SetPosition(NUM_SPRITE_INIT_POS + NUM_SPRITE_SHADOW_OFFSET);
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->SetScale(NUM_SPRITE_SCALE * m_numSprites[m_numSprites.size() - 1]->spriteScaleRate);
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->SetColor(NUM_SPRITE_INIT_SHADOW_COLOR);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//数字スプライトの初期化
		m_numSprites[m_numSprites.size() - 1]->numSprite = NewGO<SpriteRender>(0);
		m_numSprites[m_numSprites.size() - 1]->numSprite->Init(
			newFilePath,
			NUM_SPRITE_WIDTH,
			NUM_SPRITE_HEIGHT,
			AlphaBlendMode_Trans
		);

		//拡大率変数をリセット
		m_numSprites[m_numSprites.size() - 1]->spriteScaleRate = NUM_SCALE_RATE;
		
		//ピボット、拡大率、位置、色を設定
		m_numSprites[m_numSprites.size() - 1]->numSprite->SetPivot(SPRITE_PIVOT);
		m_numSprites[m_numSprites.size() - 1]->numSprite->SetPosition(NUM_SPRITE_INIT_POS);
		m_numSprites[m_numSprites.size() - 1]->numSprite->SetScale(NUM_SPRITE_SCALE * m_numSprites[m_numSprites.size() - 1]->spriteScaleRate);
		m_numSprites[m_numSprites.size() - 1]->numSprite->SetColor(NUM_SPRITE_INIT_COLOR);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Wave::FadeinWithScalingWaveSprite(SNumSprite* sNumSprite)
{
	//引数の構造体がnullなら処理しない
	if (sNumSprite == nullptr) {
		return;
	}

	//透明度が1未満ならフェードイン
	if (sNumSprite->numSprite->GetColor().w < 1.0f) {
		sNumSprite->numSprite->FadeIn(0.025f);
		sNumSprite->numShadowSprite->FadeIn(0.025f);
	}

	//スプライトの拡大率係数を下げていき、一定値でストップ
	sNumSprite->spriteScaleRate -= 0.03f;
	if (sNumSprite->spriteScaleRate <= 0.25) {
		sNumSprite->spriteScaleRate = 0.25;
	}

	//縮小
	//拡大率係数を掛けてスケーリング
	sNumSprite->numSprite->SetScale(Vector3::One * sNumSprite->spriteScaleRate);
	sNumSprite->numShadowSprite->SetScale(Vector3::One * sNumSprite->spriteScaleRate);
}

void Wave::NumSpriteClear(SNumSprite* sNumSprite)
{
	//引数の構造体がnullなら処理しない
	if (sNumSprite == nullptr) {
		return;
	}

	//数字スプライト、影スプライトともに透明にする
	sNumSprite->numSprite->SetColor({ 0.0f,0.0f,0.0f,0.0f });
	sNumSprite->numShadowSprite->SetColor({ 0.0f,0.0f,0.0f,0.0f });
}

void Wave::Update()
{
	//wave数を取得
	int waveNum = GameDirector::GetInstance()->GetWaveNumber();
	//このフレームで更新する可変長配列の配列番号を決定
	int updateTargetNum = waveNum;
	
	//フェードインと縮小
	FadeinWithScalingWaveSprite(m_numSprites[updateTargetNum]);

	//wave数が前フレームと変わっていたら
	if (waveNum != m_waveNumPrevFrame) {
		//変わる前のスプライトの色を透明にする
		//例：wave1 → wave2 だったら wave1のスプライトを透明にする
		NumSpriteClear(m_numSprites[updateTargetNum - 1]);
	}

	//現フレームのwave数を記録
	m_waveNumPrevFrame = waveNum;
}