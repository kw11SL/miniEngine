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

	//SpriteRender* m_normalShotSprite = nullptr;			//�ʏ�V���b�g�̃A�C�R��
	//Vector3 m_normalShotSpritePos = Vector3::Zero;		//�ʏ�V���b�g�̃A�C�R���̈ʒu
	//float m_normalShotSpriteAlpha = 1.0f;				//�ʏ�V���b�g�̃A�C�R���̕s�����x

	//SpriteRender* m_spreadBombSprite = nullptr;			//�X�v���b�h�{���̃A�C�R��
	//Vector3 m_spreadBombSpritePos = Vector3::Zero;		//�X�v���b�h�{���̃A�C�R���̈ʒu
	//float m_spreadBombSpriteAlpha = 1.0f;				//�X�v���b�h�{���̃A�C�R���̕s�����x



};

