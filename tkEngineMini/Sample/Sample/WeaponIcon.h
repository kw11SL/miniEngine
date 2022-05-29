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

};

