#include "stdafx.h"
#include "Life.h"

namespace{
	const int LIFE_ICON_NUM = 3;

	const char* LIFE_TEXT_SPRITE_FILEPATH = "Assets/sprite/ui/life.dds";
	const char* LIFE_ICON_SPRITE_FILEPATH = "Assets/sprite/ui/lifeIcon_noneFrame.dds";

	const Vector3 LIFE_TEXT_POS = { -550.0f,110.0f,0.0f };					//LIFE�̎��̈ʒu
	
	const Vector3 LIFE_TEXT_SCALE = { 0.6f,0.6f,1.0f };						//LIFE�̎��̊g�嗦
	const Vector3 LIFE_ICON_SCALE = { 0.3f,0.3f,1.0f };						//�A�C�R���̃X�v���C�g�̊g�嗦

	const Vector4 LIFE_TEXT_COLOR = { 1.0f * 1.5f,1.0f * 1.5f,1.0f * 1.5f,1.0f };				//LIFE�̎��̐F
	const Vector4 LIFE_ICON_COLOR = { 0.95f,0.95f,0.95f,1.0f };				//�A�C�R���̐F

	const Vector3 LIFE_TEXT_TO_LIFE_ICON_0 = { 60.0f,30.0f,0.0f };		//LIFE�̎����烉�C�t�A�C�R��1�ւ̑��Έʒu
	const Vector3 LIFE_ICON_0_TO_LIFE_ICON_1 = { 30.0f,-30.0f,0.0f };		//���C�t�A�C�R��1���烉�C�t�A�C�R��2�ւ̑��Έʒu
	const Vector3 LIFE_ICON_1_TO_LIFE_ICON_2 = { -30.0f,-30.0f,0.0f };		//���C�t�A�C�R��2���烉�C�t�A�C�R��3�ւ̑��Έʒu

	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };								//�X�v���C�g�̃s�{�b�g

	const int LIFE_TEXT_WIDTH = 128;										//LIFE�̎��̕�
	const int LIFE_TEXT_HEIGHT = 256;										//FIFE�̎��̍���

	const int LIFE_ICON_WIDTH = 256;										//���C�t�A�C�R���̕�
	const int LIFE_ICON_HEIGHT = 256;										//���C�t�A�C�R���̍���

}

Life::Life()
{
	int iconNum = 0;
	for (; iconNum < LIFE_ICON_NUM; iconNum++) {
		m_lifeIconSpritePos[iconNum] = Vector3::Zero;
	}
}

Life::~Life()
{
	int iconNum = 0;
	for (; iconNum < LIFE_ICON_NUM; iconNum++){
		DeleteGO(m_lifeIconSprite[iconNum]);
	}

	DeleteGO(m_lifeTextSprite);
}

void Life::Init()
{

	m_lifeTextSprite = NewGO<SpriteRender>(0);

	//�����X�v���C�g�̏�����
	m_lifeTextSprite->Init(
		LIFE_TEXT_SPRITE_FILEPATH,
		LIFE_TEXT_WIDTH,
		LIFE_TEXT_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_lifeTextSpritePos = LIFE_TEXT_POS;

	m_lifeTextSprite->SetPosition(m_lifeTextSpritePos);
	m_lifeTextSpritePos = LIFE_TEXT_POS;
	m_lifeTextSprite->SetScale(LIFE_TEXT_SCALE);
	m_lifeTextSprite->SetColor(LIFE_TEXT_COLOR);
	m_lifeTextSprite->SetPivot(SPRITE_PIVOT);


	//�A�C�R���X�v���C�g�̏�����
	//�ʒu�����ʂŌ��߂�
	m_lifeIconSpritePos[0] = m_lifeTextSpritePos;
	m_lifeIconSpritePos[0] += LIFE_TEXT_TO_LIFE_ICON_0;

	m_lifeIconSpritePos[1] = m_lifeIconSpritePos[0];
	m_lifeIconSpritePos[1] += LIFE_ICON_0_TO_LIFE_ICON_1;

	m_lifeIconSpritePos[2] = m_lifeIconSpritePos[1];
	m_lifeIconSpritePos[2] += LIFE_ICON_1_TO_LIFE_ICON_2;


	int iconNum = 0;
	for (; iconNum < LIFE_ICON_NUM; iconNum++) {
		m_lifeIconSprite[iconNum] = NewGO<SpriteRender>(0);

		//���ꂼ��̃A�C�R���̏�����
		m_lifeIconSprite[iconNum]->Init(
			LIFE_ICON_SPRITE_FILEPATH,
			LIFE_ICON_WIDTH,
			LIFE_ICON_HEIGHT,
			AlphaBlendMode_Trans
		);

		m_lifeIconSprite[iconNum]->SetPosition(m_lifeIconSpritePos[iconNum]);
		m_lifeIconSprite[iconNum]->SetScale(LIFE_ICON_SCALE);
		
		if (iconNum == 1) {
			m_lifeIconSprite[iconNum]->SetScale({ -LIFE_ICON_SCALE.x ,LIFE_ICON_SCALE.y,LIFE_ICON_SCALE.z });
		}

		m_lifeIconSprite[iconNum]->SetColor(LIFE_ICON_COLOR);
		m_lifeIconSprite[iconNum]->SetPivot(SPRITE_PIVOT);
	}

}

void Life::Update()
{

}