#include "stdafx.h"
#include "Timer.h"

namespace {

	const char* TIMER_SPRITE_FILEPATH = "Assets/sprite/ui/time.dds";
	const int TIMER_SPRITE_WIDTH = 256;
	const int TIMER_SPRITE_HEIGHT = 128;
	const Vector3 TIMER_SPRITE_POS = { 0.0f,310.0f,0.0f };
	const Vector2 TIMER_SPRITE_TO_NUMBER = { 0.0f,-15.0f };
	const Vector2 TIMER_SPRITE_PIVOT = { 0.5f,0.5f };
	const Vector4 TIMER_SPRITE_COLOR = { 0.25f * 1.5f,0.9f * 1.5f,0.45f * 1.5f,1.0f };
	const Vector3 TIMER_SPRITE_SCALE = { 0.4f,0.4f,1.0f };
	
	const Vector2 TIMER_NUMBER_PIVOT = { 0.5f,0.5f };
	const Vector4 TIMER_NUMBER_COLOR = { 0.25f * 1.5f,0.9f * 1.5f,0.45f * 1.5f,1.0f };

	const int DIGITS = 4 ;
}

Timer::~Timer()
{
	DeleteGO(m_timeSprite);
	DeleteGO(m_timeNumber);
}

void Timer::Init()
{
	m_timeSprite = NewGO<SpriteRender>(0);
	m_timeNumber = NewGO<FontRender>(0);

	//�X�v���C�g�̏�����
	m_timeSprite->Init(
		TIMER_SPRITE_FILEPATH,
		TIMER_SPRITE_WIDTH,
		TIMER_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//�ʒu��ݒ�
	m_timeSpritePos.x = TIMER_SPRITE_POS.x;
	m_timeSpritePos.y = TIMER_SPRITE_POS.y;
	m_timeSpritePos.z = 0.0f;

	m_timeSprite->SetPosition(m_timeSpritePos);
	m_timeSprite->SetColor(TIMER_SPRITE_COLOR);
	m_timeSprite->SetPivot(TIMER_SPRITE_PIVOT);
	m_timeSprite->SetScale(TIMER_SPRITE_SCALE);

	//�Q�[���f�B���N�^�[���玞�Ԃ��擾
	int point = GameDirector::GetInstance()->GetTime();

	//���ԕ\���̐ݒ�
	//���o�̓X�g���[�����쐬
	std::wstringstream wss;
	//�\���������w��
	wss.precision(DIGITS);
	//���o�̓X�g���[���ɐ��l���o��
	wss << point;
	m_timeNumWs = wss.str();
	const wchar_t* score = m_timeNumWs.c_str();

	//���l�����̍��W�̐ݒ�
	//�X�v���C�g��������̑��΍��W�ɂ���
	m_timeNumPos.x = m_timeSpritePos.x;
	m_timeNumPos.y = m_timeSpritePos.y;

	//�e�L�X�g����̃x�N�g�������Z
	m_timeNumPos.x += TIMER_SPRITE_TO_NUMBER.x;
	m_timeNumPos.y += TIMER_SPRITE_TO_NUMBER.y;

	//���ԕ\���e�L�X�g�̐ݒ�
	m_timeNumber->Init(score);
	m_timeNumber->SetPosition(m_timeNumPos);
	m_timeNumber->SetColor(TIMER_NUMBER_COLOR);
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