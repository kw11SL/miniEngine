#include "stdafx.h"
#include "PressStart.h"

namespace {
	const char* PRESS_SPRITE_FILEPATH = "Assets/sprite/title/pressStart.dds";			//スプライトのファイルパス
	const int PRESS_SPRITE_WIDTH = 1280;												//横幅
	const int PRESS_SPRITE_HEIGHT = 720;												//縦幅
	const Vector4 PRESS_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };					//初期カラー
	const Vector2 PRESS_SPRITE_PIVOT = { 0.5f,0.5f };
	const Vector3 PRESS_SPRITE_INIT_POS = { 0.0f,-300.0f,0.0f };
	const Vector3 PRESS_SPRITE_INIT_SCALE = { 0.2f,0.2f,1.0f };

	const float FADEIN_RATE = 0.02f;
	const float FADEOUT_RATE = 0.05f;
}

PressStart::~PressStart()
{
	DeleteGO(m_pressStartSprite);
}

void PressStart::Init()
{
	m_pressStartSprite = NewGO<SpriteRender>(0);
	
	//スプライトの初期化処理
	m_pressStartSprite->Init(
		PRESS_SPRITE_FILEPATH,
		PRESS_SPRITE_WIDTH,
		PRESS_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//各種パラメータを設定
	m_pressStartSprite->SetColor(PRESS_SPRITE_INIT_COLOR);
	m_pressStartSprite->SetPivot(PRESS_SPRITE_PIVOT);
	m_pressStartSprite->SetPosition(PRESS_SPRITE_INIT_POS);
	m_pressStartSprite->SetScale(PRESS_SPRITE_INIT_SCALE);

}

void PressStart::Update()
{
	//フラグ切り替え処理
	if (m_pressStartSprite->GetColor().w == 1.0f) {
		//スプライトの不等明度が1になったときにフェードイン完了
		m_isFinishFadeIn = true;
		m_isFinishFadeOut = false;
	}
	else if (m_pressStartSprite->GetColor().w == 0.0f) {
		//スプライトの不等明度が0になったときにフェードアウト完了
		m_isFinishFadeIn = false;
		m_isFinishFadeOut = true;
	}

	//スタートボタンが押されたらフェードイン、アウトフラグをオフ、ボタンが押されたフラグをオン
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		m_isFinishFadeIn = false;
		m_isFinishFadeOut = false;
		m_isPressStart = true;
	}

	//明滅処理
	if (m_isFinishFadeIn == false && m_isFinishFadeOut == true) {
		//フェードイン未完了でフェードアウト完了ならフェードイン
		m_pressStartSprite->FadeIn(FADEIN_RATE);
	}
	else if (m_isFinishFadeIn == true && m_isFinishFadeOut == false) {
		//逆ならフェードアウト
		m_pressStartSprite->FadeOut(FADEOUT_RATE);
	}

	//スタートボタンが押されたときのフェードアウト処理
	if (m_isPressStart == true) {
		m_pressStartSprite->FadeOut(FADEOUT_RATE);
	}
	

}