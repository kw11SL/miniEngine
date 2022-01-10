#include "stdafx.h"
#include "Timer.h"

namespace {

	const char* TIMER_SPRITE_FILEPATH = "Assets/sprite/ui/time.dds";
	
	const int TIMER_SPRITE_WIDTH = 256;
	const int TIMER_SPRITE_HEIGHT = 128;
	
	const Vector3 TIMER_SPRITE_POS = { 0.0f,310.0f,0.0f };
	const Vector3 TIMER_SPRITE_SHADOW_OFFSET = { 7.0f,-7.0f,0.0f };
	const Vector2 TIMER_SPRITE_TO_NUMBER = { 0.0f,-15.0f };
	const Vector2 TIMER_SPRITE_PIVOT = { 0.5f,0.5f };
	
	const Vector4 TIMER_SPRITE_COLOR = { 0.25f * 1.5f,0.9f * 1.5f,0.45f * 1.5f,1.0f };
	const Vector4 TIMER_SPRITE_SHADOW_COLOR = { 0.25f * 0.3f,0.9f * 0.3f,0.45f * 0.3f,1.0f * 0.7f };
	const Vector4 TIMER_NUMBER_COLOR = { 0.25f * 1.5f,0.9f * 1.5f,0.45f * 1.5f,1.0f };
	const Vector4 TIMER_NUMBER_OUTLINE_COLOR = { 0.25f * 0.2f,0.9f * 0.2f,0.45f * 0.2f,1.0 * 0.7f };

	const Vector3 TIMER_SPRITE_SCALE = { 0.4f,0.4f,1.0f };
	const Vector3 TIMER_SPRITE_OUTLINE_SCALE = { 0.5f,0.5f,1.0f };

	const Vector2 TIMER_NUMBER_PIVOT = { 0.5f,0.5f };

	const float TIMER_NUMBER_OUTLINE_SCALE_RATE = 1.1f;

	const int DIGITS = 4 ;
}

Timer::~Timer()
{
	DeleteGO(m_timeSprite);
	DeleteGO(m_timeSpriteShadow);
	DeleteGO(m_timeNumber);
	DeleteGO(m_timeNumberOutline);
}

void Timer::Init()
{
	m_timeSprite = NewGO<SpriteRender>(0);
	m_timeSpriteShadow = NewGO<SpriteRender>(0);
	m_timeNumber = NewGO<FontRender>(0);
	m_timeNumberOutline = NewGO<FontRender>(0);

	//影スプライトの初期化。先にしておく
	m_timeSpriteShadow->Init(
		TIMER_SPRITE_FILEPATH,
		TIMER_SPRITE_WIDTH,
		TIMER_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//スプライトの初期化
	m_timeSprite->Init(
		TIMER_SPRITE_FILEPATH,
		TIMER_SPRITE_WIDTH,
		TIMER_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);
	
	//位置を設定
	m_timeSpritePos.x = TIMER_SPRITE_POS.x;
	m_timeSpritePos.y = TIMER_SPRITE_POS.y;
	m_timeSpritePos.z = 0.0f;

	m_timeSprite->SetPosition(m_timeSpritePos);
	m_timeSprite->SetColor(TIMER_SPRITE_COLOR);
	m_timeSprite->SetPivot(TIMER_SPRITE_PIVOT);
	m_timeSprite->SetScale(TIMER_SPRITE_SCALE);

	//影スプライトの位置を設定
	//文字の位置からちょっとずれた位置にする
	m_timeSpriteShadow->SetPosition(m_timeSpritePos + TIMER_SPRITE_SHADOW_OFFSET);
	m_timeSpriteShadow->SetColor(TIMER_SPRITE_SHADOW_COLOR);
	m_timeSpriteShadow->SetPivot(TIMER_SPRITE_PIVOT);
	m_timeSpriteShadow->SetScale(TIMER_SPRITE_SCALE);

	//ゲームディレクターから時間を取得
	int point = GameDirector::GetInstance()->GetTime();

	//時間表示の設定
	//入出力ストリームを作成
	std::wstringstream wss;
	//表示桁数を指定
	wss.precision(DIGITS);
	//入出力ストリームに数値を出力
	wss << point;
	m_timeNumWs = wss.str();
	const wchar_t* score = m_timeNumWs.c_str();

	//数値部分の座標の設定
	//スプライト部分からの相対座標にする
	m_timeNumPos.x = m_timeSpritePos.x;
	m_timeNumPos.y = m_timeSpritePos.y;

	//テキストからのベクトルを加算
	m_timeNumPos.x += TIMER_SPRITE_TO_NUMBER.x;
	m_timeNumPos.y += TIMER_SPRITE_TO_NUMBER.y;

	//時間表示テキスト外枠の設定
	m_timeNumberOutline->Init(score);
	m_timeNumberOutline->SetPosition(m_timeNumPos);
	m_timeNumberOutline->SetColor(TIMER_NUMBER_OUTLINE_COLOR);
	m_timeNumberOutline->SetPivot(TIMER_NUMBER_PIVOT);
	m_timeNumberOutline->SetScale(TIMER_NUMBER_OUTLINE_SCALE_RATE);

	//時間表示テキストの設定
	m_timeNumber->Init(score);
	m_timeNumber->SetPosition(m_timeNumPos);
	m_timeNumber->SetColor(TIMER_NUMBER_COLOR);
	m_timeNumber->SetPivot(TIMER_NUMBER_PIVOT);

	
}

void Timer::UpdateTime()
{
	//ゲームディレクターからスコアを取得
	float remainTime = GameDirector::GetInstance()->GetTime();

	//入出力ストリームを作成
	std::wstringstream wss;
	//表示桁数を指定
	wss.precision(DIGITS);
	//入出力ストリームに数値を出力
	wss << remainTime;

	m_timeNumWs = wss.str();
	const wchar_t* time = m_timeNumWs.c_str();

	//テキストを設定
	if (m_timeNumber != nullptr) {
		m_timeNumber->SetText(time);
		m_timeNumberOutline->SetText(time);
	}

	if (GameDirector::GetInstance()->GetGameState() == enGameOver
		|| GameDirector::GetInstance()->GetGameState() == enResult) {

		m_timeNumber->SetColor({ 0.0f,0.0f,0.0f,0.0f });
		m_timeNumberOutline->SetColor({ 0.0f,0.0f,0.0f,0.0f });
	}

}