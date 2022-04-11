#pragma once
#include"Enemy.h"

class Enemy;

class EnemyManager
{
public:
	/// @brief オブジェクトの作成
	static void CreateInstance()
	{
		if (m_enemyManager == nullptr) {
			m_enemyManager = new EnemyManager;
		}

	}

	/// @brief オブジェクトを取得
	/// @return 
	static EnemyManager* GetInstance()
	{
		return m_enemyManager;
	}

	static void DeleteInstance()
	{
		delete m_enemyManager;
		m_enemyManager = nullptr;
	}

	/// @brief エネミーの初期化
	/// @param initPoint 初期位置
	/// @param initUp 上方向
	/// @param enemyType エネミーの種類
	void InitEnemies(
		const Vector3& initPoint,
		const Vector3& initUp,
		const EnEnemyType enemyType
	);

	/// @brief 配列内のエネミーを削除
	void DeleteEnemies();

	/// @brief 配列への追加
	/// @param enemy エネミー
	void AddEnemies(Enemy* enemy)
	{
		m_enemies.push_back(enemy);
	}

	/// @brief 更新処理
	void ExecuteUpdate();

private:
	EnemyManager(){}
private:
	static EnemyManager* m_enemyManager;
	std::vector<Enemy*> m_enemies;
};

