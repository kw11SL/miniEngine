#include "stdafx.h"
#include "TitleSprite.h"

namespace {
	const char* TITLE_SPRITE_FILEPATH = "Assets/sprite/title/title_Sprite.dds";			//�X�v���C�g�̃t�@�C���p�X
	const int TITLE_SPRITE_WIDTH = 1280;												//����
	const int TITLE_SPRITE_HEIGHT = 720;												//�c��
	const Vector4 TITLE_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };					//�����J���[
	const Vector2 TITLE_SPRITE_PIVOT = { 0.5f,0.5f };
	const Vector3 TITLE_SPRITE_INIT_POS = { 0.0f,0.0f,0.0f };
	const Vector3 TITLE_SPRITE_INIT_SCALE = { 0.7f,0.7f,1.0f };

	const float FADEIN_RATE = 0.02f;
	const float FADEOUT_RATE = 0.05f;
}

TitleSprite::~TitleSprite()
{
	DeleteGO(m_titleSprite);
}

void TitleSprite::Init()
{
	m_titleSprite = NewGO<SpriteRender>(0);
	
	//�X�v���C�g�̏�����
	m_titleSprite->Init(
		TITLE_SPRITE_FILEPATH,
		TITLE_SPRITE_WIDTH,
		TITLE_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//�e��p�����[�^��ݒ�
	m_titleSprite->SetPivot(TITLE_SPRITE_PIVOT);
	m_titleSprite->SetColor(TITLE_SPRITE_INIT_COLOR);
	m_titleSprite->SetPosition(TITLE_SPRITE_INIT_POS);
	m_titleSprite->SetScale(TITLE_SPRITE_INIT_SCALE);
}

void TitleSprite::FadeOutSprite()
{
	m_titleSprite->FadeOut(FADEOUT_RATE);
	
	//�t�F�[�h�C�������t���O���I���ŕs�����x��0�ɂȂ�����t�F�[�h�A�E�g����
	if (m_isFinishFadeIn == true && m_titleSprite->GetColor().w == 0.0f) {
		m_isFinishFadeOut = true;
	}
}

void TitleSprite::Update()
{
	if (m_isFinishFadeIn == false) {
		m_titleSprite->FadeIn(FADEIN_RATE);
	}

	//�^�C�g�����t�F�[�h�C������������t�F�[�h�A�E�g����
	if (m_titleSprite->GetColor().w == 1.0f) {
		m_isFinishFadeIn = true;
	}

}