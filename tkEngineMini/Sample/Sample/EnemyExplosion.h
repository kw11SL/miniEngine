#pragma once
#include "ExplosionBase.h"

class EnemyExplosion : public ExplosionBase
{
public:
	~EnemyExplosion();

	/// @brief �������ɕt�����鏈��
	void InitSub() override;

	/// @brief �X�V�ɕt�����鏈��
	void UpdateSub() override;
};

