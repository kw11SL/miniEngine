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

	/// @brief ����������
	void Init();

	/// @brief �X�V����
	void Update();

private:
	SpriteRender* m_weaponTextSprite = nullptr;			//weapon�e�L�X�g�̃X�v���C�g
	SpriteRender* m_weaponTextShadowSprite = nullptr;	//weapon�e�L�X�g�̉e�X�v���C�g
	SpriteRender* m_iconFrameSprite = nullptr;			//�A�C�R���̊O�g
	
	SpriteRender* m_weaponSpriteNormal = nullptr;
	SpriteRender* m_weaponSpriteSpread = nullptr;

	EnWeaponState m_weaponState = enNormal;

};

