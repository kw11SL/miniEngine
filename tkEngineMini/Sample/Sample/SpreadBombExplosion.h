#pragma once
#include "ExplosionBase.h"


class SpreadBombExplosion : public ExplosionBase
{
public:
	~SpreadBombExplosion();

	/// @brief 初期化に付随する処理
	void InitSub() override;

	/// @brief 更新に付随する処理
	void UpdateSub() override;

};

