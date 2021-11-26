#pragma once
#include "Enemy.h"

class EnemyGenerator : public IGameObject
{
public:
	EnemyGenerator() {}
	~EnemyGenerator();

	bool Start()override { return true; }
	void Update()override;


	void Init(const Vector3& pos,const Quaternion& rot , const EnEnemyType& enemyType);

	void GenerateEnemy(const EnEnemyType& enemyType);

	void Move();

	void Rotation();

	void AddCounter();

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

	/// @brief ���W���擾
	/// @return 
	Vector3& GetPosition()
	{
		return m_position;
	}

	/// @brief �N�H�[�^�j�I�����擾
	/// @return 
	Quaternion& GetRotation()
	{
		return m_rotation;
	}

private:
	Enemy* m_enemy = nullptr;							//�G�l�~�[

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
	float m_spawnCounter = 0.0f;						//�X�|�[���Ԋu�p�J�E���^�[
};

