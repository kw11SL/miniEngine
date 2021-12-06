#pragma once
#include "ExplosionBase.h"

class EnemyExplosion : public ExplosionBase
{
public:
	~EnemyExplosion();

	/// @brief 初期化に付随する処理
	void InitSub() override;

	/// @brief 更新に付随する処理
	void UpdateSub() override;
};

