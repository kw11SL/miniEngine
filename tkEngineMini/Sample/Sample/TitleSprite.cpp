#include "stdafx.h"
#include "TitleSprite.h"

namespace {
	const char* TITLE_SPRITE_FILEPATH = "Assets/sprite/title/title_Sprite.dds";			//スプライトのファイルパス
	const int TITLE_SPRITE_WIDTH = 1280;												//横幅
	const int TITLE_SPRITE_HEIGHT = 720;												//縦幅
	const Vector4 TITLE_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };					//初期カラー
	const Vector2 TITLE_SPRITE_PIVOT = { 0.5f,0.5f };
	const Vector3 TITLE_SPRITE_INIT_POS = { 0.0f,0.0f,0.0f };
	const Vector3 TITLE_SPRITE_INIT_SCALE = { 0.7f,0.7f,1.0f };

	const float FADEIN_RATE = 0.02f;
	const float FADEOUT_RATE = 0.05f;
}

TitleSprite::~TitleSprite()
{
	DeleteGO(m_titleSprite);
}

void TitleSprite::Init()
{
	m_titleSprite = NewGO<SpriteRender>(0);
	
	//スプライトの初期化
	m_titleSprite->Init(
		TITLE_SPRITE_FILEPATH,
		TITLE_SPRITE_WIDTH,
		TITLE_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//各種パラメータを設定
	m_titleSprite->SetPivot(TITLE_SPRITE_PIVOT);
	m_titleSprite->SetColor(TITLE_SPRITE_INIT_COLOR);
	m_titleSprite->SetPosition(TITLE_SPRITE_INIT_POS);
	m_titleSprite->SetScale(TITLE_SPRITE_INIT_SCALE);
}

void TitleSprite::FadeOutSprite()
{
	m_titleSprite->FadeOut(FADEOUT_RATE);
	
	//フェードイン完了フラグがオンで不等明度が0になったらフェードアウト完了
	if (m_isFinishFadeIn == true && m_titleSprite->GetColor().w == 0.0f) {
		m_isFinishFadeOut = true;
	}
}

void TitleSprite::Update()
{
	if (m_isFinishFadeIn == false) {
		m_titleSprite->FadeIn(FADEIN_RATE);
	}

	//タイトルがフェードインしきったらフェードアウト完了
	if (m_titleSprite->GetColor().w == 1.0f) {
		m_isFinishFadeIn = true;
	}

}