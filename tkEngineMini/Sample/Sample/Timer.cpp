#include "stdafx.h"
#include "Timer.h"

namespace {
	const Vector2 TIMER_TEXT_POS = { -0.0f,300.0f };
	const Vector2 TIMER_TEXT_TO_NUMBER = { 0.0f,-50.0f };
	
	const Vector2 TIMER_TEXT_PIVOT = { 0.5f,0.5f };
	const Vector2 TIMER_NUMBER_PIVOT = { 0.5f,0.5f };

	const wchar_t* TIMER_TEXT = L"TIME";

	const int DIGITS = 4;
}

Timer::~Timer()
{
	DeleteGO(m_timeText);
	DeleteGO(m_timeNumber);
}

void Timer::Init()
{
	m_timeText = NewGO<FontRender>(0);
	m_timeNumber = NewGO<FontRender>(0);

	m_timeTextPos = TIMER_TEXT_POS;

	//スコアテキストを決定
	m_timeText->Init(TIMER_TEXT);
	m_timeText->SetText(TIMER_TEXT);
	m_timeText->SetPosition(m_timeTextPos);
	m_timeText->SetPivot(TIMER_TEXT_PIVOT);

	//ゲームディレクターから時間を取得
	int point = GameDirector::GetInstance()->GetTime();

	//入出力ストリームを作成
	std::wstringstream wss;
	//表示桁数を指定
	wss.precision(DIGITS);
	//入出力ストリームに数値を出力
	wss << point;
	m_timeNumWs = wss.str();
	const wchar_t* score = m_timeNumWs.c_str();

	//座標の設定
	Vector2 posFromText = m_timeTextPos;
	//テキストからのベクトルを加算
	posFromText.x += TIMER_TEXT_TO_NUMBER.x;
	posFromText.y += TIMER_TEXT_TO_NUMBER.y;

	//テキストの設定
	m_timeNumber->Init(score);
	m_timeNumber->SetPosition(posFromText);
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
	}

}