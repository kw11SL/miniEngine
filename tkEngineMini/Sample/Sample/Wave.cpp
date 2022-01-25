#include "stdafx.h"
#include "Wave.h"

namespace {
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
	DeleteGO(m_waveTextSprite);
	DeleteGO(m_waveTextShadowSprite);
	DeleteGO(m_waveNumSprite);
	DeleteGO(m_waveNumShadowSprite);
}

void Wave::Init()
{
	/////////////////////////////////////////////////////////////
	//文字スプライトの初期化
	//waveの文字の影スプライトの初期化
	m_waveTextShadowSprite = NewGO<SpriteRender>(0);
	m_waveTextShadowSprite->Init(
		TEXT_SPRITE_FILEPATH,
		TEXT_SPRITE_WIDTH,
		TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveTextShadowSprite->SetPivot(SPRITE_PIVOT);
	m_waveTextShadowSprite->SetScale(TEXT_SPRITE_SCALE * TEXT_SCALE_RATE);
	//ちょっとずらす
	m_waveTextShadowSprite->SetPosition(TEXT_SPRITE_POS + TEXT_SPRITE_SHADOW_OFFSET);
	m_waveTextShadowSprite->SetColor(TEXT_SPRITE_SHADOW_INIT_COLOR);

	//waveの文字スプライトの初期化
	m_waveTextSprite = NewGO<SpriteRender>(0);
	m_waveTextSprite->Init(
		TEXT_SPRITE_FILEPATH,
		TEXT_SPRITE_WIDTH,
		TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveTextSprite->SetPivot(SPRITE_PIVOT);
	m_waveTextSprite->SetScale(TEXT_SPRITE_SCALE * TEXT_SCALE_RATE);
	m_waveTextSprite->SetPosition(TEXT_SPRITE_POS);
	m_waveTextSprite->SetColor(TEXT_SPRITE_INIT_COLOR);
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	//wave数スプライトの初期化
	//スプライトの拡大率係数を記録
	m_numSpriteScaleRate = NUM_SCALE_RATE;

	//wave数の影スプライトの初期化
	m_waveNumShadowSprite = NewGO<SpriteRender>(0);
	//wave数スプライトの初期化
	m_waveNumSprite = NewGO<SpriteRender>(0);

	//wave数からスプライトを決定して初期化する
	SwitchingWaveSprite();

	/////////////////////////////////////////////////////////////
}

void Wave::SwitchingWaveSprite()
{
	//現在のwave数を取得
	int waveNum = GameDirector::GetInstance()->GetWaveNumber();
	//wave数を一度string型に変換
	std::string numStr = std::to_string(waveNum);

	//string型でファイルパスを構成するため、パス + (wave数) + 拡張子部分で文字を結合する
	std::string convFilePath = FILEPATH_BASE_1 + numStr + FILEPATH_BASE_2;
	//できたstring型のファイルパスをconst char*型に変換
	const char* newFilePath = convFilePath.c_str();

	//拡大率変数をリセット
	m_numSpriteScaleRate = NUM_SCALE_RATE;

	//変換したファイルパスで初期化
	//影スプライトの初期化
	m_waveNumShadowSprite->Init(
		newFilePath,
		NUM_SPRITE_WIDTH,
		NUM_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveNumShadowSprite->SetPivot(SPRITE_PIVOT);
	m_waveNumShadowSprite->SetScale(NUM_SPRITE_SCALE * m_numSpriteScaleRate);
	m_waveNumShadowSprite->SetPosition(NUM_SPRITE_INIT_POS + NUM_SPRITE_SHADOW_OFFSET);
	m_waveNumShadowSprite->SetColor(NUM_SPRITE_INIT_SHADOW_COLOR);

	//本体スプライトの初期化
	m_waveNumSprite->Init(
		newFilePath,
		NUM_SPRITE_WIDTH,
		NUM_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveNumSprite->SetPivot(SPRITE_PIVOT);
	m_waveNumSprite->SetScale(NUM_SPRITE_SCALE * m_numSpriteScaleRate);
	m_waveNumSprite->SetPosition(NUM_SPRITE_INIT_POS);
	m_waveNumSprite->SetColor(NUM_SPRITE_INIT_COLOR);
}

void Wave::FadeinWithScalingWaveSprite()
{
	//フェードイン
	if (m_waveNumSprite->GetColor().w < 1.0f) {
		m_waveNumSprite->FadeIn(0.025f);
		m_waveNumShadowSprite->FadeIn(0.025f);
	}
	
	//スプライトの拡大率係数を下げていき、一定値でストップ
	m_numSpriteScaleRate -= 0.03f;
	if (m_numSpriteScaleRate <= 0.25) {
		m_numSpriteScaleRate = 0.25;
	}
	
	//縮小
	//拡大率係数を掛けてスケーリング
	m_waveNumSprite->SetScale(Vector3::One * m_numSpriteScaleRate);
	m_waveNumShadowSprite->SetScale(Vector3::One * m_numSpriteScaleRate);
}

void Wave::Update()
{
	//このフレームでディレクターがwave数を切り替えていたらwave数スプライトを切り替え
	if (GameDirector::GetInstance()->GetIsSwitchedWave() == true) {
		SwitchingWaveSprite();
	}
	
	//wave数をフェードインしながら縮小しつつ表示する
	FadeinWithScalingWaveSprite();

}