#pragma once
#include "ExplosionBase.h"


class SpreadBombExplosion : public ExplosionBase
{
public:
	~SpreadBombExplosion();

	/// @brief �������ɕt�����鏈��
	void InitSub() override;

	/// @brief �X�V�ɕt�����鏈��
	void UpdateSub() override;

};

