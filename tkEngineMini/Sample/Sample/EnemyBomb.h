#pragma once
#include "EnemyBase.h"
#include "Explosion.h"

class EnemyBomb : public EnemyBase
{
public:
	~EnemyBomb();

	/// @brief �����������ɕt�����鏈��
	void InitSub() override;

	/// @brief �G�t�F�N�g�̏���������
	void InitEffect() override;

	/// @brief �G�t�F�N�g�̍X�V����
	void UpdateEffect() override;

	/// @brief �X�V�����ɕt�����鏈��
	void UpdateSub() override;

	/// @brief �ړ������ɕt�����鏈��
	void MoveSub() override;

	/// @brief ��]�����ɕt�����鏈��
	void RotationSub() override;

	/// @brief ���Ŏ��ɕt�����鏈��
	void SelfDestroySub() override;

private:
	BulletManager* m_bulletManager = nullptr;
	ExplosionManager* m_explosionManager = nullptr;
};

