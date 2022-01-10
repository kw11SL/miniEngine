#include "stdafx.h"
#include "GameOver.h"

namespace {
	const char* SPRITE_FILEPATH = "Assets/sprite/ui/gameover_orange.dds";
	const char* SPRITE_FADEOUT_FILEPATH = "Assets/sprite/ui/screen_black.dds";
	const Vector3 INIT_POSITION = { 0.0f,0.0f,0.0f };
	//const Vector3 INIT_SCALE = { 3.0f,3.0f,3.0f };
	const Vector3 INIT_SCALE = {1.0f,1.0f,1.0f};
	const Vector4 INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };
	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };

	const int SPRITE_WIDTH = 512;
	const int SPRITE_HEIGHT = 256;

	const int SPRITE_FADEOUT_WIDTH = 1280;
	const int SPRITE_FADEOUT_HEIGHT = 720;
}

GameOver::GameOver()
{

}

GameOver::~GameOver()
{
	DeleteGO(m_gameOverSprite);
	DeleteGO(m_fadeOutSprite);
}

void GameOver::Init()
{
	m_fadeOutSprite = NewGO<SpriteRender>(0);
	m_fadeOutSprite->Init(
		SPRITE_FADEOUT_FILEPATH,
		SPRITE_FADEOUT_WIDTH,
		SPRITE_FADEOUT_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_gameOverSprite = NewGO<SpriteRender>(0);
	m_gameOverSprite->Init(
		SPRITE_FILEPATH,
		SPRITE_WIDTH,
		SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//メンバ変数を定数で初期化
	m_position = INIT_POSITION;
	m_scale = INIT_SCALE;
	m_color = INIT_COLOR;
	m_pivot = SPRITE_PIVOT;

	//各種パラメータを設定
	m_gameOverSprite->SetPivot(m_pivot);
	m_gameOverSprite->SetScale(m_scale);
	m_gameOverSprite->SetPosition(m_position);
	m_gameOverSprite->SetColor(m_color);
	m_gameOverSprite->SetScale(m_scale);

	m_fadeOutSprite->SetPivot(SPRITE_PIVOT);
	m_fadeOutSprite->SetPosition(INIT_POSITION);
	m_fadeOutSprite->SetColor(INIT_COLOR);
	m_fadeOutSprite->SetScale(INIT_SCALE);
}

void GameOver::Update()
{
	//ゲームオーバー時にフェードイン
	if (GameDirector::GetInstance()->GetGameState() == enGameOver) {
		m_gameOverSprite->FadeIn(0.03f);
		m_fadeOutSprite->FadeIn(0.007f);
	}

}