#include "stdafx.h"
#include "Score.h"

namespace {
	const char* SCORE_SPRITE_FILEPATH = "Assets/sprite/ui/score.dds";
	const Vector3 SCORE_SPRITE_POS = { 330.0f,270.0f,0.0f };
	const Vector3 SCORE_SPRITE_SHADOW_OFFSET = { 7.0f,-7.0f,0.0f };
	const Vector2 SCORE_SPRITE_PIVOT = { 0.5f,0.5f };
	const Vector3 SCORE_SPRITE_SCALE = { 0.5f,0.5f,1.0f };
	
	const Vector4 SCORE_SPRITE_COLOR = { 0.3f,0.3f,0.9f,1.0f };
	const Vector4 SCORE_SPRITE_SHADOW_COLOR = { 0.3f * 0.3f,0.3f * 0.3f,0.9f * 0.3f,1.0f * 0.7f };
	const Vector4 SCORE_NUM_COLOR = { 0.3f,0.3f,0.9f,1.0f };
	const Vector4 SCORE_NUM_SHADOW_COLOR = { 0.3f * 0.3f,0.3f * 0.3f,0.9f * 0.3f,1.0f * 0.7f };

	const float SCORE_NUM_SHADOW_SCALE_RATE = 1.1f;

	const int SCORE_SPRITE_WIDTH = 256;
	const int SCORE_SPRITE_HEIGHT = 128;

	const Vector2 SCORE_SPRITE_TO_NUMBER = { 80.0f,15.0f };


	const int DIGITS = 16;
}

Score::~Score()
{
	DeleteGO(m_scoreSprite);
	DeleteGO(m_scoreShadowSprite);
	DeleteGO(m_scoreNumber);
	DeleteGO(m_scoreNumberShadow);
}

void Score::Init()
{
	m_scoreShadowSprite = NewGO<SpriteRender>(0);
	m_scoreSprite = NewGO<SpriteRender>(0);
	m_scoreNumberShadow = NewGO<FontRender>(0);
	m_scoreNumber = NewGO<FontRender>(0);

	//�e�X�v���C�g�̏�����
	m_scoreShadowSprite->Init(
		SCORE_SPRITE_FILEPATH,
		SCORE_SPRITE_WIDTH,
		SCORE_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//�X�v���C�g�̏�����
	m_scoreSprite->Init(
		SCORE_SPRITE_FILEPATH,
		SCORE_SPRITE_WIDTH,
		SCORE_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_scoreSpritePos = SCORE_SPRITE_POS;
	//�X�v���C�g�̃p�����[�^��ݒ�
	m_scoreSprite->SetPosition(m_scoreSpritePos);
	m_scoreSprite->SetColor(SCORE_SPRITE_COLOR);
	m_scoreSprite->SetPivot(SCORE_SPRITE_PIVOT);
	m_scoreSprite->SetScale(SCORE_SPRITE_SCALE);

	//�e�X�v���C�g�̃p�����[�^��ݒ�
	//�������炵�Ă���
	m_scoreShadowSprite->SetPosition(m_scoreSpritePos + SCORE_SPRITE_SHADOW_OFFSET);
	m_scoreShadowSprite->SetColor(SCORE_SPRITE_SHADOW_COLOR);
	m_scoreShadowSprite->SetPivot(SCORE_SPRITE_PIVOT);
	m_scoreShadowSprite->SetScale(SCORE_SPRITE_SCALE);
	
	
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
	m_scoreNumber->SetColor(SCORE_NUM_COLOR);

	//�e�L�X�g�̉e�̐ݒ�
	m_scoreNumberShadow->Init(score);
	m_scoreNumberShadow->SetPosition(m_scoreTextPos);
	m_scoreNumberShadow->SetScale(SCORE_NUM_SHADOW_SCALE_RATE);
	m_scoreNumberShadow->SetColor(SCORE_NUM_SHADOW_COLOR);
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
		m_scoreNumberShadow->SetText(score);
	}

}