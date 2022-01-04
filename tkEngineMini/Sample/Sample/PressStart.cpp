#include "stdafx.h"
#include "PressStart.h"

namespace {
	const char* PRESS_SPRITE_FILEPATH = "Assets/sprite/title/pressStart.dds";			//�X�v���C�g�̃t�@�C���p�X
	const int PRESS_SPRITE_WIDTH = 1280;												//����
	const int PRESS_SPRITE_HEIGHT = 720;												//�c��
	const Vector4 PRESS_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };					//�����J���[
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
	
	//�X�v���C�g�̏���������
	m_pressStartSprite->Init(
		PRESS_SPRITE_FILEPATH,
		PRESS_SPRITE_WIDTH,
		PRESS_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//�e��p�����[�^��ݒ�
	m_pressStartSprite->SetColor(PRESS_SPRITE_INIT_COLOR);
	m_pressStartSprite->SetPivot(PRESS_SPRITE_PIVOT);
	m_pressStartSprite->SetPosition(PRESS_SPRITE_INIT_POS);
	m_pressStartSprite->SetScale(PRESS_SPRITE_INIT_SCALE);

}

void PressStart::Update()
{
	//�t���O�؂�ւ�����
	if (m_pressStartSprite->GetColor().w == 1.0f) {
		//�X�v���C�g�̕s�����x��1�ɂȂ����Ƃ��Ƀt�F�[�h�C������
		m_isFinishFadeIn = true;
		m_isFinishFadeOut = false;
	}
	else if (m_pressStartSprite->GetColor().w == 0.0f) {
		//�X�v���C�g�̕s�����x��0�ɂȂ����Ƃ��Ƀt�F�[�h�A�E�g����
		m_isFinishFadeIn = false;
		m_isFinishFadeOut = true;
	}

	//�X�^�[�g�{�^���������ꂽ��t�F�[�h�C���A�A�E�g�t���O���I�t�A�{�^���������ꂽ�t���O���I��
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		m_isFinishFadeIn = false;
		m_isFinishFadeOut = false;
		m_isPressStart = true;
	}

	//���ŏ���
	if (m_isFinishFadeIn == false && m_isFinishFadeOut == true) {
		//�t�F�[�h�C���������Ńt�F�[�h�A�E�g�����Ȃ�t�F�[�h�C��
		m_pressStartSprite->FadeIn(FADEIN_RATE);
	}
	else if (m_isFinishFadeIn == true && m_isFinishFadeOut == false) {
		//�t�Ȃ�t�F�[�h�A�E�g
		m_pressStartSprite->FadeOut(FADEOUT_RATE);
	}

	//�X�^�[�g�{�^���������ꂽ�Ƃ��̃t�F�[�h�A�E�g����
	if (m_isPressStart == true) {
		m_pressStartSprite->FadeOut(FADEOUT_RATE);
	}
	

}