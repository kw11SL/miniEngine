#pragma once
#include "EnemyBase.h"

class EnemyNormal : public EnemyBase
{
public:
	~EnemyNormal();

	/// @brief 初期化処理に付随する処理
	void InitSub() override;
	
	/// @brief エフェクトの初期化処理
	void InitEffect() override;

	/// @brief エフェクトの更新処理
	void UpdateEffect() override;

	/// @brief 更新処理に付随する処理
	void UpdateSub() override;

	/// @brief 移動処理に付随する処理
	void MoveSub() override;

	/// @brief 回転処理に付随する処理
	void RotationSub() override;

	/// @brief 自滅時に付随する処理
	void SelfDestroySub() override;

};

