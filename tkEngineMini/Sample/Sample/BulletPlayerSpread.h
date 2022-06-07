#pragma once
#include "BulletBase.h"

/// @brief プレイヤーのスプレッドボムクラス
class BulletPlayerSpread : public BulletBase
{
public:
	~BulletPlayerSpread();

	/// @brief 初期化処理に付随する処理
	void InitSub() override;
	/// @brief 更新処理に付随する処理
	void UpdateSub() override;
	/// @brief 移動処理に付随する処理
	void MoveSub() override;
	/// @brief 回転処理に付随する処理
	void RotationSub() override;
	/// @brief 破棄処理に付随する処理
	void DestroySub() override;
	/// @brief エフェクトの初期化処理に付随する処理
	void InitEffectSub() override;
	/// @brief エフェクトの更新に付随する処理
	void EffectUpdateSub() override;
};

