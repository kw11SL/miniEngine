#pragma once
class Explosion;

class ExplosionManager
{
public:
	/// @brief オブジェクトを生成
	static void CreateInstance()
	{
		if (m_explosionManager == nullptr) {
			m_explosionManager = new ExplosionManager;
		}
	}

	/// @brief オブジェクトを取得
	/// @return オブジェクト
	static ExplosionManager* GetInstance()
	{
		return m_explosionManager;
	}

	/// @brief オブジェクトを削除
	static void DeleteInstance()
	{
		delete m_explosionManager;
		m_explosionManager = nullptr;
	}

	/// @brief 爆発の初期化
	/// @param pos 座標
	/// @param scaleRate 拡大率
	/// @param explosionType 爆発の種類
	void InitExplosion(
		const Vector3& pos,
		const float scaleRate,
		const EnExplosionType& explosionType
	);

	/// @brief 更新処理
	void ExecuteUpdate();

	/// @brief 配列内の爆発を全て削除する
	void DeleteExplosions();

private:
	/// @brief 爆発のタイプごとに返す名前を決定
	/// @param type 爆発のタイプ
	/// @return 名前
	const char* Naming(const EnExplosionType& type);

private:
	ExplosionManager() {}

	static ExplosionManager* m_explosionManager;
	std::vector<Explosion*> m_explosions;
};

