#include "stdafx.h"
#include "Score.h"

namespace {
	const char* SCORE_SPRITE_FILEPATH = "Assets/sprite/ui/score.dds";
	const Vector3 SCORE_SPRITE_POS = { 330.0f,270.0f,0.0f };
	const Vector2 SCORE_SPRITE_PIVOT = { 0.5f,0.5f };
	const Vector3 SCORE_SPRITE_SCALE = { 0.5f,0.5f,1.0f };
	const Vector4 SCORE_SPRITE_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	const int SCORE_SPRITE_WIDTH = 256;
	const int SCORE_SPRITE_HEIGHT = 128;

	const Vector2 SCORE_SPRITE_TO_NUMBER = { 80.0f,15.0f };


	const int DIGITS = 16;
}

Score::~Score()
{
	DeleteGO(m_scoreSprite);
	DeleteGO(m_scoreNumber);
}

void Score::Init()
{
	m_scoreSprite = NewGO<SpriteRender>(0);
	m_scoreNumber = NewGO<FontRender>(0);

	//�X�v���C�g�̏�����
	m_scoreSprite->Init(
		SCORE_SPRITE_FILEPATH,
		SCORE_SPRITE_WIDTH,
		SCORE_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_scoreSpritePos = SCORE_SPRITE_POS;

	m_scoreSprite->SetPosition(m_scoreSpritePos);
	m_scoreSprite->SetColor(SCORE_SPRITE_COLOR);
	m_scoreSprite->SetPivot(SCORE_SPRITE_PIVOT);
	m_scoreSprite->SetScale(SCORE_SPRITE_SCALE);
	
	//�Q�[���f�B���N�^�[����X�R�A���擾
	int point = GameDirector::GetInstance()->GetScore();

	//���o�̓X�g���[�����쐬
	std::wstringstream wss;
	//�\���������w��
	wss.precision(DIGITS);
	//���o�̓X�g���[���ɐ��l���o��
	wss << point;
	m_scoreNumWs = wss.str();
	const wchar_t* score = m_scoreNumWs.c_str();

	//���W�̐ݒ�
	m_scoreTextPos.x = m_scoreSpritePos.x;
	m_scoreTextPos.y = m_scoreSpritePos.y;

	//�e�L�X�g����̃x�N�g�������Z
	m_scoreTextPos.x += SCORE_SPRITE_TO_NUMBER.x;
	m_scoreTextPos.y += SCORE_SPRITE_TO_NUMBER.y;
	
	//�e�L�X�g�̐ݒ�
	m_scoreNumber->Init(score);
	m_scoreNumber->SetPosition(m_scoreTextPos);
}

void Score::UpdateScore()
{
	//�Q�[���f�B���N�^�[����X�R�A���擾
	int plScore = GameDirector::GetInstance()->GetScore();

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