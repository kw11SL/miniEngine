#pragma once
#include "EnemyBase.h"

class EnemyShot : public EnemyBase
{
public:
	~EnemyShot();

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
	/// @brief ���˃J�E���^�[�̑���
	void AddShotCounter()
	{
		m_shotCounter += g_gameTime->GetFrameDeltaTime();
	}

	/// @brief ���˒��O�ɔ�������\���G�t�F�N�g�̃X�P�[�����O����
	/// @param activateTime ����������^�C�~���O
	void ShotNoticeScaling(const float activateTime);

private:
	Effect m_shotNoticeEffect;				//�V���b�g�̗\���G�t�F�N�g
};
