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

	//�X�R�A�e�L�X�g������
	m_timeText->Init(TIMER_TEXT);
	m_timeText->SetText(TIMER_TEXT);
	m_timeText->SetPosition(m_timeTextPos);
	m_timeText->SetPivot(TIMER_TEXT_PIVOT);

	//�Q�[���f�B���N�^�[���玞�Ԃ��擾
	int point = GameDirector::GetInstance()->GetTime();

	//���o�̓X�g���[�����쐬
	std::wstringstream wss;
	//�\���������w��
	wss.precision(DIGITS);
	//���o�̓X�g���[���ɐ��l���o��
	wss << point;
	m_timeNumWs = wss.str();
	const wchar_t* score = m_timeNumWs.c_str();

	//���W�̐ݒ�
	Vector2 posFromText = m_timeTextPos;
	//�e�L�X�g����̃x�N�g�������Z
	posFromText.x += TIMER_TEXT_TO_NUMBER.x;
	posFromText.y += TIMER_TEXT_TO_NUMBER.y;

	//�e�L�X�g�̐ݒ�
	m_timeNumber->Init(score);
	m_timeNumber->SetPosition(posFromText);
	m_timeNumber->SetPivot(TIMER_NUMBER_PIVOT);
}

void Timer::UpdateTime()
{
	//�Q�[���f�B���N�^�[����X�R�A���擾
	float remainTime = GameDirector::GetInstance()->GetTime();

	//���o�̓X�g���[�����쐬
	std::wstringstream wss;
	//�\���������w��
	wss.precision(DIGITS);
	//���o�̓X�g���[���ɐ��l���o��
	wss << remainTime;

	m_timeNumWs = wss.str();
	const wchar_t* time = m_timeNumWs.c_str();

	//�e�L�X�g��ݒ�
	if (m_timeNumber != nullptr) {
		m_timeNumber->SetText(time);
	}

}