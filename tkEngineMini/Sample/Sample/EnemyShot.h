#pragma once
#include "EnemyBase.h"

class EnemyShot : public EnemyBase
{
public:
	~EnemyShot();

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

private:
	/// @brief 発射カウンターの増加
	void AddShotCounter()
	{
		m_shotCounter += g_gameTime->GetFrameDeltaTime();
	}

	/// @brief 発射直前に発生する予兆エフェクトのスケーリング処理
	/// @param activateTime 発生させるタイミング
	void ShotNoticeScaling(const float activateTime);

private:
	Effect m_shotNoticeEffect;				//ショットの予兆エフェクト
};
