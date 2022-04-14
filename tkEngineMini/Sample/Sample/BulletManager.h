#pragma once
class Bullet;

class BulletManager
{
public:
	/// @brief �I�u�W�F�N�g�̍쐬
	static void CreateInstance()
	{
		if (m_bulletManager == nullptr) {
			m_bulletManager = new BulletManager;
		}
	}

	/// @brief �I�u�W�F�N�g���擾
	/// @return 
	static BulletManager* GetInstance()
	{
		return m_bulletManager;
	}

	/// @brief �I�u�W�F�N�g�̍폜
	static void DeleteInstance()
	{
		delete m_bulletManager;
		m_bulletManager = nullptr;
	}

	/// @brief �e�̏�����
	/// @param initPoint �����ʒu
	/// @param initUp �����
	/// @param direction ����
	/// @param bulletType �e�̎��
	void InitBullets(
		const Vector3& initPoint,
		const Vector3& initUp,
		const Vector3& direction,
		const EnBulletType bulletType
	);

	/// @brief �X�V���� 
	void ExecuteUpdate();

	/// @brief �z����̒e��S�č폜����
	void DeleteBullets();

private:
	BulletManager() {}

private:

	static BulletManager* m_bulletManager;
	std::vector<Bullet*> m_bullets;
};

