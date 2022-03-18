#pragma once

enum EnWeaponState {
	enNormal,
	enSpread,
	enWeaponStateNum
};

class WeaponIcon
{
public:
	WeaponIcon() {}

	~WeaponIcon();

	/// @brief 初期化処理
	void Init();

	/// @brief 更新処理
	void Update();

private:
	SpriteRender* m_weaponTextSprite = nullptr;			//weaponテキストのスプライト
	SpriteRender* m_weaponTextShadowSprite = nullptr;	//weaponテキストの影スプライト
	SpriteRender* m_iconFrameSprite = nullptr;			//アイコンの外枠
	
	SpriteRender* m_weaponSpriteNormal = nullptr;
	SpriteRender* m_weaponSpriteSpread = nullptr;

	EnWeaponState m_weaponState = enNormal;

	//SpriteRender* m_normalShotSprite = nullptr;			//通常ショットのアイコン
	//Vector3 m_normalShotSpritePos = Vector3::Zero;		//通常ショットのアイコンの位置
	//float m_normalShotSpriteAlpha = 1.0f;				//通常ショットのアイコンの不透明度

	//SpriteRender* m_spreadBombSprite = nullptr;			//スプレッドボムのアイコン
	//Vector3 m_spreadBombSpritePos = Vector3::Zero;		//スプレッドボムのアイコンの位置
	//float m_spreadBombSpriteAlpha = 1.0f;				//スプレッドボムのアイコンの不等明度



};

