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

//�����ōs������
private:
	/// @brief ���Ԏ����ɔ�Ⴕ�ăG�t�F�N�g���k�������鏈��
	void LifeRingScaling();

private:
	Effect m_lifeRingEffect;			//���Ԏ����G�t�F�N�g
};

