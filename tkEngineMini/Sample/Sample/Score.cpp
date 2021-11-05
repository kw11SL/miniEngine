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

	//�X�R�A�e�L�X�g������
	m_scoreText->Init(SCORE_TEXT);
	m_scoreText->SetText(SCORE_TEXT);
	m_scoreText->SetPosition(m_scoreTextPos);
	
	//�Q�[���f�B���N�^�[����X�R�A���擾
	float point = GameDirector::GetInstance()->GetScore();

	//���o�̓X�g���[�����쐬
	std::wstringstream wss;
	//�\���������w��
	wss.precision(DIGITS);
	//���o�̓X�g���[���ɐ��l���o��
	wss << point;
	m_scoreNumWs = wss.str();
	const wchar_t* score = m_scoreNumWs.c_str();

	//���W�̐ݒ�
	Vector2 posFromText = m_scoreTextPos;
	//�e�L�X�g����̃x�N�g�������Z
	posFromText.x += SCORE_TEXT_TO_NUMBER.x;
	posFromText.y += SCORE_TEXT_TO_NUMBER.y;
	
	//�e�L�X�g�̐ݒ�
	m_scoreNumber->Init(score);
	m_scoreNumber->SetPosition(posFromText);
}

void Score::UpdateScore()
{
	//�Q�[���f�B���N�^�[����X�R�A���擾
	float plScore = GameDirector::GetInstance()->GetScore();

	//���o�̓X�g���[�����쐬
	std::wstringstream wss;
	//�\���������w��
	wss.precision(DIGITS);
	//���o�̓X�g���[���ɐ��l���o��
	wss << plScore;

	m_scoreNumWs = wss.str();
	const wchar_t* score = m_scoreNumWs.c_str();

	//�e�L�X�g��ݒ�
	if (m_scoreNumber != nullptr) {
		m_scoreNumber->SetText(score);
	}

}