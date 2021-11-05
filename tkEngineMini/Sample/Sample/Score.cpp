#include "stdafx.h"
#include "Score.h"

namespace {
	const Vector2 SCORE_TEXT_POS = { -500.0f,300.0f };
	const Vector2 SCORE_TEXT_TO_NUMBER = { 150.0f,0.0f };

	const wchar_t* SCORE_TEXT = L"SCORE : ";

	const int DIGITS = 4;
}

Score::~Score()
{
	DeleteGO(m_scoreText);
	DeleteGO(m_scoreNumber);
}

void Score::Init()
{
	m_scoreText = NewGO<FontRender>(0);
	m_scoreNumber = NewGO<FontRender>(0);

	m_scoreTextPos = SCORE_TEXT_POS;

	//スコアテキストを決定
	m_scoreText->Init(SCORE_TEXT);
	m_scoreText->SetText(SCORE_TEXT);
	m_scoreText->SetPosition(m_scoreTextPos);
	
	//ゲームディレクターからスコアを取得
	float point = GameDirector::GetInstance()->GetScore();

	//入出力ストリームを作成
	std::wstringstream wss;
	//表示桁数を指定
	wss.precision(DIGITS);
	//入出力ストリームに数値を出力
	wss << point;
	m_scoreNumWs = wss.str();
	const wchar_t* score = m_scoreNumWs.c_str();

	//座標の設定
	Vector2 posFromText = m_scoreTextPos;
	//テキストからのベクトルを加算
	posFromText.x += SCORE_TEXT_TO_NUMBER.x;
	posFromText.y += SCORE_TEXT_TO_NUMBER.y;
	
	//テキストの設定
	m_scoreNumber->Init(score);
	m_scoreNumber->SetPosition(posFromText);
}

void Score::UpdateScore()
{
	//ゲームディレクターからスコアを取得
	float plScore = GameDirector::GetInstance()->GetScore();

	//入出力ストリームを作成
	std::wstringstream wss;
	//表示桁数を指定
	wss.precision(DIGITS);
	//入出力ストリームに数値を出力
	wss << plScore;

	m_scoreNumWs = wss.str();
	const wchar_t* score = m_scoreNumWs.c_str();

	//テキストを設定
	if (m_scoreNumber != nullptr) {
		m_scoreNumber->SetText(score);
	}

}