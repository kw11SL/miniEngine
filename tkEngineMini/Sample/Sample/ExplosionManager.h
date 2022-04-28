#pragma once
class Explosion;

class ExplosionManager
{
public:
	/// @brief �I�u�W�F�N�g�𐶐�
	static void CreateInstance()
	{
		if (m_explosionManager == nullptr) {
			m_explosionManager = new ExplosionManager;
		}
	}

	/// @brief �I�u�W�F�N�g���擾
	/// @return �I�u�W�F�N�g
	static ExplosionManager* GetInstance()
	{
		return m_explosionManager;
	}

	/// @brief �I�u�W�F�N�g���폜
	static void DeleteInstance()
	{
		delete m_explosionManager;
		m_explosionManager = nullptr;
	}

	/// @brief �����̏�����
	/// @param pos ���W
	/// @param scaleRate �g�嗦
	/// @param explosionType �����̎��
	void InitExplosion(
		const Vector3& pos,
		const float scaleRate,
		const EnExplosionType& explosionType
	);

	/// @brief �X�V����
	void ExecuteUpdate();

	/// @brief �z����̔�����S�č폜����
	void DeleteExplosions();

private:
	/// @brief �����̃^�C�v���ƂɕԂ����O������
	/// @param type �����̃^�C�v
	/// @return ���O
	const char* Naming(const EnExplosionType& type);

private:
	ExplosionManager() {}

	static ExplosionManager* m_explosionManager;
	std::vector<Explosion*> m_explosions;
};

