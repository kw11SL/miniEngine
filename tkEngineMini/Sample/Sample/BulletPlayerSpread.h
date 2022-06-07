#pragma once
#include "BulletBase.h"

/// @brief �v���C���[�̃X�v���b�h�{���N���X
class BulletPlayerSpread : public BulletBase
{
public:
	~BulletPlayerSpread();

	/// @brief �����������ɕt�����鏈��
	void InitSub() override;
	/// @brief �X�V�����ɕt�����鏈��
	void UpdateSub() override;
	/// @brief �ړ������ɕt�����鏈��
	void MoveSub() override;
	/// @brief ��]�����ɕt�����鏈��
	void RotationSub() override;
	/// @brief �j�������ɕt�����鏈��
	void DestroySub() override;
	/// @brief �G�t�F�N�g�̏����������ɕt�����鏈��
	void InitEffectSub() override;
	/// @brief �G�t�F�N�g�̍X�V�ɕt�����鏈��
	void EffectUpdateSub() override;
};

