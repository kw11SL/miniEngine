#include "stdafx.h"
#include "UIOutline.h"

namespace {
	const Vector3 INIT_POINT = { 0.0f,0.0f,0.0f };
	const Vector3 INIT_SCALE = { 1.05f,1.05f,1.0f };
	const int SPRITE_WIDTH = 1280;
	const int SPRITE_HEIGHT = 720;
	const Vector4 SPRITE_COLOR = { 0.3f,0.7f,0.5f, 0.0f };
	const char* SPRITE_FILEPATH = "Assets/sprite/ui/ui_outline_mesh.dds";
	//const char* SPRITE_FILEPATH = "Assets/sprite/ui/ui_outline_a.dds";
	const float FADEIN_ADDRATE = 0.04f;
}
 
UIOutline::~UIOutline()
{
	//スプライトを削除
	DeleteGO(m_outlineSprite);
}

void UIOutline::Init()
{
	m_outlineSprite = NewGO<SpriteRender>(0);

	//スプライトを初期化
	m_outlineSprite->Init(
		SPRITE_FILEPATH,
		SPRITE_WIDTH,
		SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_outlineSprite->SetPosition(INIT_POINT);
	m_outlineSprite->SetPivot({ 0.5f, 0.5f });
	m_outlineSprite->SetScale(INIT_SCALE);
	//フェードインさせるために不等明度を0にしておく
	m_outlineSprite->SetColor(SPRITE_COLOR * 2.0f);
}

void UIOutline::Update()
{
	//フェードインする処理
	m_outlineSprite->FadeIn(FADEIN_ADDRATE);

}