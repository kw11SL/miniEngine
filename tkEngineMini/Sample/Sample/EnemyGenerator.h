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

	/// @brief 座標を設定
	/// @param pos 
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// @brief クォータニオンを設定
	/// @param rot 
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// @brief 座標を取得
	/// @return 
	Vector3& GetPosition()
	{
		return m_position;
	}

	/// @brief クォータニオンを取得
	/// @return 
	Quaternion& GetRotation()
	{
		return m_rotation;
	}

private:
	Enemy* m_enemy = nullptr;

	MyCharacterController m_myCharaCon;
	SphericalMove m_sphericalMove;
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_forward = Vector3::Zero;
	Vector3 m_right = Vector3::Zero;
	Vector3 m_up = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };
	EnEnemyType m_spawnEnemyType = enCommon;
	float m_spawnCounter = 0.0f;
};

