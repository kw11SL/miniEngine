#pragma once
#include "ExplosionBase.h"
#include "SpreadBombExplosion.h"
#include "EnemyExplosion.h"

/// @brief �����N���X�̊��N���X���g���N���X

/// @brief �����̃^�C�v
enum EnExplosionType {
	enPlayer_Spread_Bomb,
	enEnemy_Explosion,
	enExplosion_Type_Num
};

class Explosion : public IGameObject
{
public:
	Explosion() {}
	~Explosion() {}

	bool Start();
	void Update();

	/// @brief ����������
	/// @param pos �������W
	/// @param scaleRate �g�嗦
	void Init(
		const Vector3& pos ,
		const float scaleRate,
		const EnExplosionType& explosionType
	);

	/// @brief ���W���擾
	/// @return 
	const Vector3& GetPosition() const
	{
		return m_explosionBase->GetPosition();
	}

	/// @brief �g�嗦���擾
	/// @return 
	const Vector3& GetScale() const
	{
		return m_explosionBase->GetScale();
	}

	/// @brief �g�嗦(float��)���擾
	/// @return 
	const float GetScaleRate() const
	{
		return m_explosionBase->GetScaleRate();
	}

	/// @brief ��]�N�H�[�^�j�I�����擾
	/// @return 
	const Quaternion& GetRotation() const
	{
		return m_explosionBase->GetRotation();
	}

	/// @brief �_���[�W�l���擾
	/// @return 
	const float GetPower() const
	{
		return m_explosionBase->GetPower();
	}

	/// @brief �_���[�W���a���擾
	/// @return 
	const float GetDamageArea() const
	{
		return m_explosionBase->GetDamageArea();
	}

	/// @brief �_���[�W�Ԋu���擾
	/// @return 
	const float GetDamageInterval() const
	{
		return m_explosionBase->GetDamageInterval();
	}

	/// @brief ���݃t���O���擾
	/// @return 
	const bool GetIsExist() const
	{
		return m_explosionBase->GetIsExist();
	}

private:
	ExplosionBase* m_explosionBase = nullptr;		//���N���X�̃|�C���^�ϐ�

	//���N���X�ɓn���h���N���X�̃I�u�W�F�N�g
	SpreadBombExplosion m_sBomb;
	EnemyExplosion m_enemyExplosion;

};

