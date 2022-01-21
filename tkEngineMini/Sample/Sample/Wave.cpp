#include "stdafx.h"
#include "Wave.h"

namespace {
	const char* TEXT_SPRITE_FILEPATH = "Assets/sprite/ui/wave.dds";
	const char* NUM_SPRITE_FILEPATH = "Assets/sprite/ui/1.dds";

	//wave数スプライト初期化用ファイルパスの前半と拡張子部分
	const std::string FILEPATH_BASE_1 = "Assets/sprite/ui/";
	const std::string FILEPATH_BASE_2 = ".dds";

	const Vector3 TEXT_SPRITE_POS = { -580.0f,320.0f,0.0f };
	const Vector3 NUM_SPRITE_INIT_POS = { -520.0f,280.0f,0.0f };

	const Vector3 TEXT_SPRITE_SCALE = Vector3::One;
	const float TEXT_SCALE_RATE = 0.3f;
	const Vector3 NUM_SPRITE_SCALE = Vector3::One;
	const float NUM_SCALE_RATE = 0.5f;

	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };

	const Vector4 TEXT_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 NUM_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };

	//スプライトの幅と高さ
	const int TEXT_SPRITE_WIDTH = 256;
	const int TEXT_SPRITE_HEIGHT = 128;

	const int NUM_SPRITE_WIDTH = 256;
	const int NUM_SPRITE_HEIGHT = 256;


	
}

Wave::~Wave()
{
	DeleteGO(m_waveTextSprite);
	DeleteGO(m_waveNumSprite);
}

void Wave::Init()
{
	m_waveTextSprite = NewGO<SpriteRender>(0);
	m_waveNumSprite = NewGO<SpriteRender>(0);

	//waveの文字スプライトの初期化
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

	//wave数スプライトの初期化
	//スプライトの拡大率係数を記録
	m_numSpriteScaleRate = NUM_SCALE_RATE;

	m_waveNumSprite->Init(
		NUM_SPRITE_FILEPATH,
		NUM_SPRITE_WIDTH,
		NUM_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveNumSprite->SetPivot(SPRITE_PIVOT);
	m_waveNumSprite->SetScale(NUM_SPRITE_SCALE * m_numSpriteScaleRate);
	m_waveNumSprite->SetPosition(NUM_SPRITE_INIT_POS);
	m_waveNumSprite->SetColor(NUM_SPRITE_INIT_COLOR);
}

void Wave::Update()
{
	//テストwave数スプライトの変更
	if (g_pad[0]->IsTrigger(enButtonRight)) {
		
		m_numSpriteScaleRate = 0.5f;

		m_waveNum++;
		if (m_waveNum > 9) {
			m_waveNum = 9;
		}

		//int waveNum = GameDirector::GetInstance()->GetWaveNumber();
		////wave数を一度string型に変換
		//std::string numStr = std::to_string(waveNum);
		
		std::string numStr = std::to_string(m_waveNum);
		//string型でファイルパスを構成するため、パス + (wave数) + 拡張子部分で文字を結合する
		std::string convFilePath = FILEPATH_BASE_1 + numStr + FILEPATH_BASE_2;
		//できたファイルパスをconst char*型に変換
		const char* newFilePath = convFilePath.c_str();

		//できたファイルパスで初期化
		m_waveNumSprite->Init(
			newFilePath,
			NUM_SPRITE_WIDTH,
			NUM_SPRITE_HEIGHT,
			AlphaBlendMode_Trans
		);

		m_waveNumSprite->SetPivot(SPRITE_PIVOT);
		m_waveNumSprite->SetScale(NUM_SPRITE_SCALE * NUM_SCALE_RATE);
		m_waveNumSprite->SetPosition(NUM_SPRITE_INIT_POS);
		m_waveNumSprite->SetColor(NUM_SPRITE_INIT_COLOR);
		
	}
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		
		m_numSpriteScaleRate = 0.5f;
		
		m_waveNum--;
		if (m_waveNum < 0) {
			m_waveNum = 0;
		}

		std::string filePathBase1 = "Assets/sprite/ui/";
		std::string filePathBase2 = ".dds";

		std::string numStr = std::to_string(m_waveNum);
		std::string convFilePath = filePathBase1 + numStr + filePathBase2;

		const char* newFilePath = convFilePath.c_str();

		m_waveNumSprite->Init(
			newFilePath,
			NUM_SPRITE_WIDTH,
			NUM_SPRITE_HEIGHT,
			AlphaBlendMode_Trans
		);

		m_waveNumSprite->SetPivot(SPRITE_PIVOT);
		m_waveNumSprite->SetScale(NUM_SPRITE_SCALE * NUM_SCALE_RATE);
		m_waveNumSprite->SetPosition(NUM_SPRITE_INIT_POS);
		m_waveNumSprite->SetColor(NUM_SPRITE_INIT_COLOR);

	}

	//フェードイン
	if (m_waveNumSprite->GetColor().w < 1.0f) {
		m_waveNumSprite->FadeIn(0.02f);
	}

	//縮小
	//スプライトの拡大率係数を下げていき、一定値でストップ
	m_numSpriteScaleRate -= 0.03f;
	if (m_numSpriteScaleRate <= 0.25) {
		m_numSpriteScaleRate = 0.25;
	}
	//拡大率係数を掛けてスケーリング
	m_waveNumSprite->SetScale(Vector3::One * m_numSpriteScaleRate);

}