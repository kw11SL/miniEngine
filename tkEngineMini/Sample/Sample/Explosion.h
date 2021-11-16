#pragma once
#include "ExplosionBase.h"
#include "SpreadBombExplosion.h"

/// @brief 爆発クラスの基底クラスを使うクラス

/// @brief 爆発のタイプ
enum EnExplosionType {
	enPlayer_Spread_Bomb,
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

private:
	ExplosionBase* m_explosionBase = nullptr;		//基底クラスのポインタ変数

	//基底クラスに渡す派生クラスのオブジェクト
	SpreadBombExplosion m_sBomb;

};

