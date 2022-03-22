#pragma once
#include "Enemy.h"

class EnemyGenerator : public IGameObject
{
public:
	EnemyGenerator() {}
	~EnemyGenerator();

	bool Start()override { return true; }
	void Update()override;

	/// @brief ����������
	/// @param pos ���W
	/// @param rot ��]
	/// @param isActive �ŏ�����A�N�e�B�u�ɂ��邩�ǂ���
	/// @param enemyType �X�|�[��������G�l�~�[�̎��
	void Init(const Vector3& pos,const Quaternion& rot ,const bool isActive, const EnEnemyType& enemyType);

	/// @brief �G�l�~�[����
	void Generate();

	/// @brief �G�t�F�N�g�̍Đ��ƃG�l�~�[�̐�������
	void SpawnEnemyWithEffect();

	/// @brief �ړ�����
	void Move();

	/// @brief ��]����
	void Rotation();

	/// @brief �X�|�[���������A�e��J�E���^�[�𑝉�
	void AddCounter();

	/// @brief �G�l�~�[�̐��������J�E���^�[�𑝉�
	void AddGenerateCounter()
	{
		m_generateCounter += g_gameTime->GetFrameDeltaTime();
	}

	/// @brief �X�|�[�������p�̃J�E���^�[�𑝉�
	void AddSpawnCounter()
	{
		m_spawnCounter += g_gameTime->GetFrameDeltaTime();
	}

	/// @brief �G�t�F�N�g�����p�̃J�E���^�[�𑝉�
	void AddSpawnEffectPlayCounter()
	{
		m_spawnEffectCounter += g_gameTime->GetFrameDeltaTime();
	}

	/// @brief ���W��ݒ�
	/// @param pos 
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// @brief �N�H�[�^�j�I����ݒ�
	/// @param rot 
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// @brief �A�N�e�B�u�t���O��ݒ�
	/// @param isActive 
	void SetActive(const bool isActive)
	{
		m_isActive = isActive;
	}

	/// @brief ���W���擾
	/// @return 
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// @brief �N�H�[�^�j�I�����擾
	/// @return 
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/// @brief ������̃A�N�e�B�u��Ԃ��擾
	/// @return 
	const bool GetIsActive() const
	{
		return m_isActive;
	}

	//������̃A�N�e�B�x�[�g����
	void Activate();

	/// @brief �G�l�~�[�̔����A������
	void SpawnEnemy();

	/// @brief �X�|�[�����̃G�t�F�N�g�Đ�����
	void PlaySpawnEffect();

	/// @brief �G�t�F�N�g�̍X�V
	void UpdateEffect();

	/// @brief ���������G�l�~�[�̍폜����єz�񂩂�̍폜����
	void DeleteEnemy();

	/// @brief �������x�N�g����������̔��΂ŏ�����
	/// @param up �����
	void SetDownVector(const Vector3& up)
	{
		m_downVector = up * -1.0f;
	}

	/// @brief �N�H�[�^�j�I���ŉE�A��A�O����]
	/// @param rot �K�p����N�H�[�^�j�I��
	void SetVectorFromQuaternion(const Quaternion& rot)
	{
		rot.Apply(m_right);
		rot.Apply(m_up);
		rot.Apply(m_forward);

		m_right.Normalize();
		m_up.Normalize();
		m_forward.Normalize();

		//�ύX�������������ɉ�������ݒ肷��
		SetDownVector(m_up);
	}

private:
	std::vector<Enemy*> m_enemies;						//�G�l�~�[�̉ϒ��z��

	MyCharacterController m_myCharaCon;					//���ʈړ��p�L�����R��
	SphericalMove m_sphericalMove;						//���ʈړ��p�N���X

	Vector3 m_position = Vector3::Zero;					//���W
	Quaternion m_rotation = Quaternion::Identity;		//��]�N�H�[�^�j�I��
	Vector3 m_forward = Vector3::Zero;					//�O��
	Vector3 m_right = Vector3::Zero;					//�E
	Vector3 m_up = Vector3::Zero;						//��
	Vector3 m_moveSpeed = Vector3::Zero;				//���x�x�N�g��
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//���C���΂��������x�N�g��
	EnEnemyType m_spawnEnemyType = enCommon;			//�X�|�[��������G�l�~�[�^�C�v
	float m_generateCounter = 0.0f;						//�G�l�~�[�̐��������J�E���^�[
	float m_spawnCounter = 0.0f;						//�X�|�[���Ԋu�p�J�E���^�[
	float m_spawnEffectCounter = 0.0f;					//�X�|�[���G�t�F�N�g�̍Đ��p�J�E���^�[
	bool m_isActive = false;							//�����킪�A�N�e�B�u���ǂ���
	bool m_isSpawning = false;							//�X�|�[�����������ǂ���
	bool m_isValidPlayEffect = false;					//�X�|�[���������A�G�t�F�N�g�����\���ǂ���
	bool m_isValidSpawnEnemy = false;					//�X�|�[���������A�G�l�~�[�𐶐��\���ǂ���

	EnemyManager* m_enemyManager = nullptr;				//�G�l�~�[�Ǘ��N���X�̃I�u�W�F�N�g
	Effect m_spawnEffect;								//�G�l�~�[���X�|�[��������Ƃ��̃G�t�F�N�g
};

