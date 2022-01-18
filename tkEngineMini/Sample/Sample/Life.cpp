#include "stdafx.h"
#include "Life.h"

namespace{
	//���C�t�A�C�R���̐�(for���񂷗p)
	const int LIFE_ICON_NUM = 3;

	//�t�@�C���p�X
	const char* LIFE_TEXT_SPRITE_FILEPATH = "Assets/sprite/ui/life.dds";						//LIFE�A�C�R���摜�̃t�@�C���p�X
	const char* LIFE_ICON_SPRITE_FILEPATH = "Assets/sprite/ui/lifeIcon_noneFrame.dds";			//LIFE�A�C�R���̘g�摜�̃t�@�C���p�X
	const char* LIFE_ICON_HALO_SPRITE_FILEPATH = "Assets/sprite/ui/lifeIconHalo.dds";			//�����摜�̃t�@�C���p�X
	const char* LIFE_ICON_FRAME_FILEPATH = "Assets/sprite/ui/lifeIcon_frame.dds";				//�g�X�v���C�g�̃t�@�C���p�X

	//�ʒu�A�g�嗦�A�J���[�A�s�{�b�g
	const Vector3 LIFE_TEXT_POS = { -600.0f,120.0f,0.0f };										//LIFE�̎��̃X�v���C�g�ʒu
	const Vector3 LIFE_TEXT_SHADOW_OFFSET = { 7.0f,-7.0f,0.0f };
	
	const Vector3 LIFE_TEXT_SCALE = { 0.6f,0.6f,1.0f };											//LIFE�̎��̃X�v���C�g�̊g�嗦
	const Vector3 LIFE_ICON_SCALE = { 0.2f,0.2f,1.0f };											//�A�C�R���̃X�v���C�g�̊g�嗦
	const Vector3 LIFE_ICON_FRAME_SCALE = { 0.2f,0.2f,1.0f };									//�g�X�v���C�g�̊g�嗦
	const Vector3 LIFE_ICON_HALO_SCALE = { 1.0f,1.0f,1.0f };									//�����X�v���C�g�̊g�嗦

	const Vector4 LIFE_TEXT_COLOR = { 0.9f * 1.5f,0.25f * 1.5f,0.25f * 1.5f,1.0f };				//LIFE�̎��̃X�v���C�g�̐F
	const Vector4 LIFE_TEXT_SHADOW_COLOR = { 0.9f * 0.3f,0.25f * 0.3f,0.25f * 0.3f,1.0f * 0.7f };
	const Vector4 LIFE_ICON_COLOR = { 0.95f,0.95f,0.95f,1.0f };									//�A�C�R���̐F
	const Vector4 LIFE_ICON_FRAME_COLOR = { 1.0f,1.0f,1.0f,1.0f };								//�g�X�v���C�g�̐F
	const Vector4 LIFE_ICON_HALO_COLOR = { 0.9f,0.3f,0.2f,1.0f };								//�����̐F

	const Vector3 LIFE_TEXT_TO_LIFE_ICON_0 = { 80.0f,40.0f,0.0f };								//LIFE�̎����烉�C�t�A�C�R��1�ւ̑��Έʒu
	const Vector3 LIFE_ICON_0_TO_LIFE_ICON_1 = { 5.0f,-40.0f,0.0f };							//���C�t�A�C�R��1���烉�C�t�A�C�R��2�ւ̑��Έʒu
	const Vector3 LIFE_ICON_1_TO_LIFE_ICON_2 = { -5.0f,-40.0f,0.0f };							//���C�t�A�C�R��2���烉�C�t�A�C�R��3�ւ̑��Έʒu

	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };													//�X�v���C�g�̃s�{�b�g

	//���A����
	const int LIFE_TEXT_WIDTH = 128;															//LIFE�̎��̕�
	const int LIFE_TEXT_HEIGHT = 256;															//FIFE�̎��̍���
	const int LIFE_ICON_WIDTH = 256;															//���C�t�A�C�R���̕�
	const int LIFE_ICON_HEIGHT = 256;															//���C�t�A�C�R���̍���
	const int LIFE_ICON_FRAME_WIDTH = 256;														//�g�X�v���C�g�̕�
	const int LIFE_ICON_FRAME_HEIGHT = 256;														//�g�X�v���C�g�̍���
	const int LIFE_ICON_HALO_WIDTH = 128;														//�����̍���
	const int LIFE_ICON_HALO_HEIGHT = 128;														//�����̕�

	//�����֌W
	const float X_SCALE_RATE = 0.1f;															//���ɉ��т��
	const float Y_SCALE_RATE = 0.03f;															//�c�ɏk�ޗ�
}

Life::Life()
{
	m_prevPlayerLife = LIFE_ICON_NUM;

	int iconNum = 0;
	//���C�t�A�C�R���A�g�X�v���C�g�̏�����
	for (; iconNum < LIFE_ICON_NUM; iconNum++) {
		m_lifeIconSpritePos[iconNum] = Vector3::Zero;
		m_lifeIconFramePos[iconNum] = Vector3::Zero;
	}
}

Life::~Life()
{
	int iconNum = 0;
	for (; iconNum < LIFE_ICON_NUM; iconNum++){
		DeleteGO(m_lifeIconSprite[iconNum]);
		DeleteGO(m_lifeIconFrame[iconNum]);
	}

	DeleteGO(m_lifeIconHalo);
	DeleteGO(m_lifeTextSprite);
	DeleteGO(m_lifeTextShadowSprite);
}

void Life::Init()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	//�����̉e�X�v���C�g�̏������B��ɂ��Ă����B
	m_lifeTextShadowSprite = NewGO<SpriteRender>(0);
	m_lifeTextShadowSprite->Init(
		LIFE_TEXT_SPRITE_FILEPATH,
		LIFE_TEXT_WIDTH,
		LIFE_TEXT_HEIGHT,
		AlphaBlendMode_Trans
	);
	
	//�����X�v���C�g�̏�����
	m_lifeTextSprite = NewGO<SpriteRender>(0);
	m_lifeTextSprite->Init(
		LIFE_TEXT_SPRITE_FILEPATH,
		LIFE_TEXT_WIDTH,
		LIFE_TEXT_HEIGHT,
		AlphaBlendMode_Trans
	);

	//�����X�v���C�g�̈ʒu�A�g�嗦�A�F�A�s�{�b�g��ݒ�
	m_lifeTextSpritePos = LIFE_TEXT_POS;
	m_lifeTextSprite->SetPosition(m_lifeTextSpritePos);
	m_lifeTextSprite->SetScale(LIFE_TEXT_SCALE);
	m_lifeTextSprite->SetColor(LIFE_TEXT_COLOR);
	m_lifeTextSprite->SetPivot(SPRITE_PIVOT);

	//�e�X�v���C�g�̃p�����[�^��ݒ�
	//�������炷
	m_lifeTextShadowSprite->SetPosition(m_lifeTextSpritePos + LIFE_TEXT_SHADOW_OFFSET);
	m_lifeTextShadowSprite->SetScale(LIFE_TEXT_SCALE);
	m_lifeTextShadowSprite->SetColor(LIFE_TEXT_SHADOW_COLOR);
	m_lifeTextShadowSprite->SetPivot(SPRITE_PIVOT);

	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	//�A�C�R���X�v���C�g,�A�C�R���g�X�v���C�g�̏�����
	//�ʒu���Ɍ��߂Ă���
	m_lifeIconSpritePos[0] = m_lifeTextSpritePos;
	m_lifeIconSpritePos[0] += LIFE_TEXT_TO_LIFE_ICON_0;

	m_lifeIconSpritePos[1] = m_lifeIconSpritePos[0];
	m_lifeIconSpritePos[1] += LIFE_ICON_0_TO_LIFE_ICON_1;

	m_lifeIconSpritePos[2] = m_lifeIconSpritePos[1];
	m_lifeIconSpritePos[2] += LIFE_ICON_1_TO_LIFE_ICON_2;

	//�g�X�v���C�g�̈ʒu���A�C�R���Ɠ����ʒu�ɂ���
	int iconFrameNum = 0;
	for (; iconFrameNum < LIFE_ICON_NUM; iconFrameNum++) {
		m_lifeIconFramePos[iconFrameNum] = m_lifeIconSpritePos[iconFrameNum];
	}

	//�A�C�R���X�v���C�g,�g�X�v���C�g�����ꂼ��NewGO���A������
	int iconNum = 0;
	for (; iconNum < LIFE_ICON_NUM; iconNum++) {
		m_lifeIconSprite[iconNum] = NewGO<SpriteRender>(0);
		m_lifeIconFrame[iconNum] = NewGO<SpriteRender>(0);

		//�A�C�R���̏�����
		m_lifeIconSprite[iconNum]->Init(
			LIFE_ICON_SPRITE_FILEPATH,
			LIFE_ICON_WIDTH,
			LIFE_ICON_HEIGHT,
			AlphaBlendMode_Trans
		);

		//�g�X�v���C�g�̏�����
		m_lifeIconFrame[iconNum]->Init(
			LIFE_ICON_FRAME_FILEPATH,
			LIFE_ICON_FRAME_WIDTH,
			LIFE_ICON_FRAME_HEIGHT,
			AlphaBlendMode_Trans
		);

		//�A�C�R���A�g�X�v���C�g�����炩���ߌ��߂��ʒu�ɐݒ�
		m_lifeIconSprite[iconNum]->SetPosition(m_lifeIconSpritePos[iconNum]);
		m_lifeIconFrame[iconNum]->SetPosition(m_lifeIconFramePos[iconNum]);

		//�A�C�R���A�g�X�v���C�g�̊g�嗦��ݒ�
		m_lifeIconSprite[iconNum]->SetScale(LIFE_ICON_SCALE);
		m_lifeIconFrame[iconNum]->SetScale(LIFE_ICON_FRAME_SCALE);

		//�A�C�R���A�g�X�v���C�g��2�߂̃A�C�R���͍��E���]���������̂�x�̊g�嗦��-1���|����
		if (iconNum == 1) {
			m_lifeIconSprite[iconNum]->SetScale({ -LIFE_ICON_SCALE.x ,LIFE_ICON_SCALE.y,LIFE_ICON_SCALE.z });
			m_lifeIconFrame[iconNum]->SetScale({ -LIFE_ICON_FRAME_SCALE.x,LIFE_ICON_FRAME_SCALE.y,LIFE_ICON_FRAME_SCALE.z });
		}
		
		//�F��ݒ�
		m_lifeIconSprite[iconNum]->SetColor(LIFE_ICON_COLOR);
		m_lifeIconFrame[iconNum]->SetColor(LIFE_ICON_FRAME_COLOR);

		//�s�{�b�g��ݒ�
		m_lifeIconSprite[iconNum]->SetPivot(SPRITE_PIVOT);
		m_lifeIconFrame[iconNum]->SetPivot(SPRITE_PIVOT);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	//�����X�v���C�g�̏�����
	m_lifeIconHalo = NewGO<SpriteRender>(0);
	m_lifeIconHalo->Init(
		LIFE_ICON_HALO_SPRITE_FILEPATH,
		LIFE_ICON_HALO_WIDTH,
		LIFE_ICON_HALO_HEIGHT,
		AlphaBlendMode_Trans
	);
	//�ʒu��3�ڂ̃��C�t�A�C�R���ʒu�ɂ��Ă���
	m_lifeIconHalo->SetPosition(m_lifeIconSpritePos[2]);
	//�g�嗦��ݒ�
	m_lifeIconHaloScale = LIFE_ICON_HALO_SCALE;
	m_lifeIconHalo->SetScale(m_lifeIconHaloScale);
	//�F��ݒ�
	m_lifeIconHaloColor = LIFE_ICON_HALO_COLOR;
	m_lifeIconHalo->SetColor(m_lifeIconHaloColor * 1.2f);
	//�s�{�b�g��ݒ�
	m_lifeIconHalo->SetPivot(SPRITE_PIVOT);
	//////////////////////////////////////////////////////////////////////////////////////////////

}

void Life::LifeDisp()
{
	//�c�@2�̂Ƃ��A3�ڂ̃A�C�R�����t�F�[�h�A�E�g������
	if (GameDirector::GetInstance()->GetPlayerLife() <= 2) {
		m_lifeIconSprite[2]->FadeOut(0.04f);
	}
	//�c�@1�̂Ƃ��A2�ڂ̃A�C�R�����t�F�[�h�A�E�g������
	if (GameDirector::GetInstance()->GetPlayerLife() <= 1) {
		m_lifeIconSprite[1]->FadeOut(0.04f);
	}
	//�c�@0�̂Ƃ��A1�ڂ̃A�C�R�����t�F�[�h�A�E�g������
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		m_lifeIconSprite[0]->FadeOut(0.04f);
	}

}

void Life::HaloDisp()
{
	//�����X�v���C�g�̈ʒu����
	//�c�@3�̏ꍇ
	if (GameDirector::GetInstance()->GetPlayerLife() == 3) {
		m_lifeIconHaloPos = m_lifeIconSpritePos[2];
	}
	//�c�@2�̏ꍇ�͂��̃A�C�R���ֈړ�
	if (GameDirector::GetInstance()->GetPlayerLife() == 2
		&& m_isHaloFadeOutReady == false) {
		m_lifeIconHaloPos = m_lifeIconSpritePos[1];
		m_lifeIconHaloScale = LIFE_ICON_HALO_SCALE;
	}
	//�c�@1�̏ꍇ�͂��̃A�C�R���ֈړ�
	if (GameDirector::GetInstance()->GetPlayerLife() == 1
		&& m_isHaloFadeOutReady == false) {
		m_lifeIconHaloPos = m_lifeIconSpritePos[0];
		m_lifeIconHaloScale = LIFE_ICON_HALO_SCALE;
	}

	//�ύX��̍��W�𔽉f������
	m_lifeIconHalo->SetPosition(m_lifeIconHaloPos);

}

void Life::VariableHalo()
{
	//�F����`�⊮�����邱�ƂŖ��ł�����
	//�F�̍ŏ��l�A�ő�l�����߂�
	Vector4 minColor, maxColor;
	minColor = LIFE_ICON_HALO_COLOR * 0.7f;
	maxColor = LIFE_ICON_HALO_COLOR * 1.4f;

	//���邳�̐܂�Ԃ��t���O��ݒ�
	//������ԗ���1�ȏ�Ȃ�t���O�I��
	if (m_lifeIconHaloColorRate >= 1.0f) {
		m_lifeIconHaloMaxFlag = true;
	}
	else if (m_lifeIconHaloColorRate <= 0.0f) {
		//0�ȉ��Ȃ�I�t
		m_lifeIconHaloMaxFlag = false;
	}

	//��ԗ���ύX
	//�t���O�I���Ȃ��ԗ����㏸
	if (m_lifeIconHaloMaxFlag == false) {
		m_lifeIconHaloColorRate += 0.03f;
	}
	else {
		//�I�t�Ȃ猸��
		m_lifeIconHaloColorRate -= 0.01f;
	}

	//���邳��ύX
	//��ԗ��Ő��`�⊮
	m_lifeIconHaloColor.Lerp(
		m_lifeIconHaloColorRate,
		minColor,
		maxColor
	);

	//�F��ݒ�
	m_lifeIconHalo->SetColor(m_lifeIconHaloColor);
}

void Life::HaloVanish()
{
	//�O�t���[�����烉�C�t�̒l�ɕω���������������鏀���t���O���I��
	if (m_prevPlayerLife - GameDirector::GetInstance()->GetPlayerLife() >= 1) {
		m_isHaloFadeOutReady = true;
	}

	//�����鏀���t���O���I���̂Ƃ��A�ό`
	if (m_isHaloFadeOutReady == true) {
		//�F���ő�̖��邳�ɂ���
		m_lifeIconHaloColorRate = 1.0f;
		//�������ɍL����
		m_lifeIconHaloScale.x += X_SCALE_RATE;
		//�c�����ɏk�߂ď������悤�Ɍ�����
		m_lifeIconHaloScale.y -= Y_SCALE_RATE;
	}

	if (m_lifeIconHaloScale.y <= 0.0f) {
		m_lifeIconHaloScale.y = 0.0f;
		//�c�̃X�P�[����0�ȉ��ɂȂ�����(=�����ڏ��������)0�ɂ��āA�����鏀���t���O���I�t
		m_isHaloFadeOutReady = false;
	}

	//�����X�v���C�g�Ɋg�嗦�𔽉f������
	m_lifeIconHalo->SetScale(m_lifeIconHaloScale);
}

void Life::Update()
{
	LifeDisp();
	VariableHalo();
	HaloVanish();
	HaloDisp();
	
	//���t���[���ł̃v���C���[�̃��C�t���L�^
	m_prevPlayerLife = GameDirector::GetInstance()->GetPlayerLife();
}