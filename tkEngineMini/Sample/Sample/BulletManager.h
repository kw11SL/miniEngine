#pragma once
class Bullet;

class BulletManager
{
public:
	/// @brief オブジェクトの作成
	static void CreateInstance()
	{
		if (m_bulletManager == nullptr) {
			m_bulletManager = new BulletManager;
		}
	}

	/// @brief オブジェクトを取得
	/// @return 
	static BulletManager* GetInstance()
	{
		return m_bulletManager;
	}

	/// @brief オブジェクトの削除
	static void DeleteInstance()
	{
		delete m_bulletManager;
		m_bulletManager = nullptr;
	}

	/// @brief 弾の初期化
	/// @param initPoint 初期位置
	/// @param initUp 上方向
	/// @param direction 向き
	/// @param bulletType 弾の種類
	void InitBullets(
		const Vector3& initPoint,
		const Vector3& initUp,
		const Vector3& direction,
		const EnBulletType bulletType
	);

	/// @brief 更新処理 
	void ExecuteUpdate();

	/// @brief 配列内の弾を全て削除する
	void DeleteBullets();

private:
	BulletManager() {}

private:

	static BulletManager* m_bulletManager;
	std::vector<Bullet*> m_bullets;
};

