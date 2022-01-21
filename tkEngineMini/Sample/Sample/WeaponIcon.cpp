#include "stdafx.h"
#include "WeaponIcon.h"

namespace {
	const char* TEXT_SPRITE_FILEPATH = "Assets/sprite/ui/weapon.dds";						//weapon文字のファイルパス
	const char* ICON_FRAME_SPRITE_FILEPATH = "Assets/sprite/ui/weaponIcon_frame.dds";		//アイコンの外枠のファイルパス
	const char* SHOT_NORMAL_SPRITE_FILEPATH = "Assets/sprite/ui/weaponIcon_normal.dds";		//通常ショットアイコンのファイルパス
	const char* SHOT_SPREAD_SPRITE_FILEPATH = "Assets/sprite/ui/weaponIcon_spread.dds";		//スプレッドボムアイコンのファイルパス

	const Vector3 TEXT_INIT_POS = { -420.0f,280.0f,0.0f };			//文字スプライトの初期位置
	const Vector3 SPRITE_INIT_POS = { -400.0f,220.0f,0.0f };		//初期位置
	const Vector3 SPRITE_POS_LIGHT = { -280.0f,120.0f,0.0f };		//右からフェードインするときの位置

	const Vector3 SPRITE_SCALE = Vector3::One;
	const float TEXT_SPRITE_SCALE_RATE = 0.35f;
	const float FRAME_SPRITE_SCALE_RATE = 0.3f;
	const float ICON_SPRITE_SCALE_RATE = 0.25f;
	
	const Vector4 TEXT_INIT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 ICON_FRAME_INIT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 ICON_INIT_COLOR = { 1.0f,1.0f,1.0f,1.0f };

	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };

	//スプライトの幅と高さ
	//文字スプライト
	const int TEXT_SPRITE_WIDHT = 256;
	const int TEXT_SPRITE_HEIGHT = 128;
	
	//アイコンスプライト各種共通
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

	//文字スプライトの初期化
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


	//枠スプライトを初期化
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

	//武器スプライトを初期化
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
	//スプライトの切り替え
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