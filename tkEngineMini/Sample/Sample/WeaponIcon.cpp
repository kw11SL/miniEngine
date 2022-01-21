#include "stdafx.h"
#include "WeaponIcon.h"

namespace {
	const char* TEXT_SPRITE_FILEPATH = "Assets/sprite/ui/weapon.dds";						//weapon�����̃t�@�C���p�X
	const char* ICON_FRAME_SPRITE_FILEPATH = "Assets/sprite/ui/weaponIcon_frame.dds";		//�A�C�R���̊O�g�̃t�@�C���p�X
	const char* SHOT_NORMAL_SPRITE_FILEPATH = "Assets/sprite/ui/weaponIcon_normal.dds";		//�ʏ�V���b�g�A�C�R���̃t�@�C���p�X
	const char* SHOT_SPREAD_SPRITE_FILEPATH = "Assets/sprite/ui/weaponIcon_spread.dds";		//�X�v���b�h�{���A�C�R���̃t�@�C���p�X

	const Vector3 TEXT_INIT_POS = { -420.0f,280.0f,0.0f };			//�����X�v���C�g�̏����ʒu
	const Vector3 SPRITE_INIT_POS = { -400.0f,220.0f,0.0f };		//�����ʒu
	const Vector3 SPRITE_POS_LIGHT = { -280.0f,120.0f,0.0f };		//�E����t�F�[�h�C������Ƃ��̈ʒu

	const Vector3 SPRITE_SCALE = Vector3::One;
	const float TEXT_SPRITE_SCALE_RATE = 0.35f;
	const float FRAME_SPRITE_SCALE_RATE = 0.3f;
	const float ICON_SPRITE_SCALE_RATE = 0.25f;
	
	const Vector4 TEXT_INIT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 ICON_FRAME_INIT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 ICON_INIT_COLOR = { 1.0f,1.0f,1.0f,1.0f };

	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };

	//�X�v���C�g�̕��ƍ���
	//�����X�v���C�g
	const int TEXT_SPRITE_WIDHT = 256;
	const int TEXT_SPRITE_HEIGHT = 128;
	
	//�A�C�R���X�v���C�g�e�틤��
	const int ICON_SPRITE_WIDTH = 256;
	const int ICON_SPRITE_HEIGHT = 256;
}


WeaponIcon::~WeaponIcon()
{
	DeleteGO(m_weaponTextSprite);
	DeleteGO(m_iconFrameSprite);
	DeleteGO(m_weaponSprite);
	/*DeleteGO(m_normalShotSprite);
	DeleteGO(m_spreadBombSprite);*/
}

void WeaponIcon::Init()
{
	m_weaponTextSprite = NewGO<SpriteRender>(0);
	m_iconFrameSprite = NewGO<SpriteRender>(0);
	m_weaponSprite = NewGO<SpriteRender>(0);
	/*m_normalShotSprite = NewGO<SpriteRender>(0);
	m_spreadBombSprite = NewGO<SpriteRender>(0);*/

	//�����X�v���C�g�̏�����
	m_weaponTextSprite->Init(
		TEXT_SPRITE_FILEPATH,
		TEXT_SPRITE_WIDHT,
		TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_weaponTextSprite->SetPivot(SPRITE_PIVOT);
	m_weaponTextSprite->SetScale(SPRITE_SCALE * TEXT_SPRITE_SCALE_RATE);
	m_weaponTextSprite->SetPosition(TEXT_INIT_POS);
	m_weaponTextSprite->SetColor(TEXT_INIT_COLOR);


	//�g�X�v���C�g��������
	m_iconFrameSprite->Init(
		ICON_FRAME_SPRITE_FILEPATH,
		ICON_SPRITE_WIDTH,
		ICON_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_iconFrameSprite->SetPivot(SPRITE_PIVOT);
	m_iconFrameSprite->SetScale(SPRITE_SCALE * FRAME_SPRITE_SCALE_RATE);
	m_iconFrameSprite->SetPosition(SPRITE_INIT_POS);
	m_iconFrameSprite->SetColor(ICON_FRAME_INIT_COLOR);

	//����X�v���C�g��������
	m_weaponSprite->Init(
		SHOT_NORMAL_SPRITE_FILEPATH,
		ICON_SPRITE_WIDTH,
		ICON_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_weaponSprite->SetPivot(SPRITE_PIVOT);
	m_weaponSprite->SetScale(SPRITE_SCALE * ICON_SPRITE_SCALE_RATE);
	m_weaponSprite->SetPosition(SPRITE_INIT_POS);
	m_weaponSprite->SetColor(ICON_INIT_COLOR);


}

void WeaponIcon::Update()
{
	//�X�v���C�g�̐؂�ւ�
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		if (m_weaponState == enNormal) {
			
			m_weaponState = enSpread;

			m_weaponSprite->Init(
				SHOT_SPREAD_SPRITE_FILEPATH,
				ICON_SPRITE_WIDTH,
				ICON_SPRITE_HEIGHT,
				AlphaBlendMode_Trans
			);

			m_weaponSprite->SetPivot(SPRITE_PIVOT);
			m_weaponSprite->SetScale(SPRITE_SCALE * ICON_SPRITE_SCALE_RATE);
			m_weaponSprite->SetPosition(SPRITE_INIT_POS);
			m_weaponSprite->SetColor(ICON_INIT_COLOR);
		}
		else if (m_weaponState == enSpread){

			m_weaponState = enNormal;

			m_weaponSprite->Init(
				SHOT_NORMAL_SPRITE_FILEPATH,
				ICON_SPRITE_WIDTH,
				ICON_SPRITE_HEIGHT,
				AlphaBlendMode_Trans
			);

			m_weaponSprite->SetPivot(SPRITE_PIVOT);
			m_weaponSprite->SetScale(SPRITE_SCALE * ICON_SPRITE_SCALE_RATE);
			m_weaponSprite->SetPosition(SPRITE_INIT_POS);
			m_weaponSprite->SetColor(ICON_INIT_COLOR);
		}

	}

}