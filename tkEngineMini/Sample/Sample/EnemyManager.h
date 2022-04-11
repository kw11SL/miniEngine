#pragma once
#include"Enemy.h"

class Enemy;

class EnemyManager
{
public:
	/// @brief �I�u�W�F�N�g�̍쐬
	static void CreateInstance()
	{
		if (m_enemyManager == nullptr) {
			m_enemyManager = new EnemyManager;
		}

	}

	/// @brief �I�u�W�F�N�g���擾
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

	/// @brief �G�l�~�[�̏�����
	/// @param initPoint �����ʒu
	/// @param initUp �����
	/// @param enemyType �G�l�~�[�̎��
	void InitEnemies(
		const Vector3& initPoint,
		const Vector3& initUp,
		const EnEnemyType enemyType
	);

	/// @brief �z����̃G�l�~�[���폜
	void DeleteEnemies();

	/// @brief �z��ւ̒ǉ�
	/// @param enemy �G�l�~�[
	void AddEnemies(Enemy* enemy)
	{
		m_enemies.push_back(enemy);
	}

	/// @brief �X�V����
	void ExecuteUpdate();

private:
	EnemyManager(){}
private:
	static EnemyManager* m_enemyManager;
	std::vector<Enemy*> m_enemies;
};

