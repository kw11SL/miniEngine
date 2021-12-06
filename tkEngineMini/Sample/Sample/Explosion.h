#pragma once
#include "ExplosionBase.h"
#include "SpreadBombExplosion.h"
#include "EnemyExplosion.h"

/// @brief 爆発クラスの基底クラスを使うクラス

/// @brief 爆発のタイプ
enum EnExplosionType {
	enPlayer_Spread_Bomb,
	enEnemy_Explosion,
	enExplosion_Type_Num
};

class Explosion : public IGameObject
{
public:
	Explosion() {}
	~Explosion() {}

	bool Start();
	void Update();

	/// @brief 初期化処理
	/// @param pos 初期座標
	/// @param scaleRate 拡大率
	void Init(
		const Vector3& pos ,
		const float scaleRate,
		const EnExplosionType& explosionType
	);

	/// @brief 座標を取得
	/// @return 
	const Vector3& GetPosition() const
	{
		return m_explosionBase->GetPosition();
	}

	/// @brief 拡大率を取得
	/// @return 
	const Vector3& GetScale() const
	{
		return m_explosionBase->GetScale();
	}

	/// @brief 拡大率(float版)を取得
	/// @return 
	const float GetScaleRate() const
	{
		return m_explosionBase->GetScaleRate();
	}

	/// @brief 回転クォータニオンを取得
	/// @return 
	const Quaternion& GetRotation() const
	{
		return m_explosionBase->GetRotation();
	}

	/// @brief ダメージ値を取得
	/// @return 
	const float GetPower() const
	{
		return m_explosionBase->GetPower();
	}

	/// @brief ダメージ半径を取得
	/// @return 
	const float GetDamageArea() const
	{
		return m_explosionBase->GetDamageArea();
	}

	/// @brief ダメージ間隔を取得
	/// @return 
	const float GetDamageInterval() const
	{
		return m_explosionBase->GetDamageInterval();
	}

	/// @brief 存在フラグを取得
	/// @return 
	const bool GetIsExist() const
	{
		return m_explosionBase->GetIsExist();
	}

private:
	ExplosionBase* m_explosionBase = nullptr;		//基底クラスのポインタ変数

	//基底クラスに渡す派生クラスのオブジェクト
	SpreadBombExplosion m_sBomb;
	EnemyExplosion m_enemyExplosion;

};

