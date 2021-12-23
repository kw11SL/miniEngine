#pragma once
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
	SpriteRender* m_normalShotSprite = nullptr;			//�ʏ�V���b�g�̃A�C�R��
	Vector3 m_normalShotSpritePos = Vector3::Zero;		//�ʏ�V���b�g�̃A�C�R���̈ʒu
	float m_normalShotSpriteAlpha = 1.0f;				//�ʏ�V���b�g�̃A�C�R���̕s�����x

	SpriteRender* m_spreadBombSprite = nullptr;			//�X�v���b�h�{���̃A�C�R��
	Vector3 m_spreadBombSpritePos = Vector3::Zero;		//�X�v���b�h�{���̃A�C�R���̈ʒu
	float m_spreadBombSpriteAlpha = 1.0f;				//�X�v���b�h�{���̃A�C�R���̕s�����x

	SpriteRender* m_iconFarameSprite = nullptr;			//�A�C�R���̊O�g


};

