#pragma once
#include "ExplosionManager.h"
#include "BulletBase.h"
#include "BulletPlayerNormal.h"
#include "BulletPlayerSpread.h"
#include "BulletEnemyNormal.h"

class ExplosionManager;

/// @brief �e�̎��
enum EnBulletType
{
	enPlayerNormal,
	enPlayerSpreadBomb,
	enEnemyNormal,
	enBulletTypeNum
};

/// @brief �e�N���X�B�e�̊��N���X������Ɏ����Ďg���B
class Bullet : public IGameObject
{
public:
	Bullet() {}
	~Bullet();

	/// @brief ����������
	/// @param initPoint �����ʒu
	/// @param initUp �����ʒu�̏�x�N�g��
	/// @param direction ����
	/// @param bulletType �e�̎��
	void Init(
		const Vector3& initPoint,
		const Vector3& initUp,
		const Vector3& direction,
		const EnBulletType& bulletType);

	//�Q�b�^�[
	/// @brief	���W���擾 
	/// @return ���W
	Vector3 GetPosition() 
	{ 
		return m_bulletBase->GetPosition();
	}

	/// @brief �g�嗦���擾
	/// @return �g�嗦
	Vector3 GetScale() 
	{ 
		return m_bulletBase->GetScale();
	}

	/// @brief ��]���擾
	/// @return ��]
	Quaternion GetRotation() 
	{
		return m_bulletBase->GetRotation();
	}

	/// @brief ��]�p�x���擾
	/// @return ��]�p�x
	float GetAngle() 
	{ 
		return m_bulletBase->GetAngle();
	}

	/// @brief ���݃t���O���擾
	/// @return 
	bool GetIsExist()
	{
		return m_bulletBase->GetIsExist();
	}

	/// @brief �e�̃_���[�W���擾
	/// @return 
	float GetPower()
	{
		return m_bulletBase->GetPower();
	}

	/// @brief �e�̎��Ԏ������擾
	/// @return 
	float GetLifeTime()
	{
		return m_bulletBase->GetLifeTime();
	}

	/// @brief �_���[�W�Ԋu���擾
	/// @return 
	float GetDamageInterval()
	{
		return m_bulletBase->GetDamageInterval();
	}
		
	//�Z�b�^�[
	/// @brief	���W��ݒ� 
	/// @param pos ���W
	void SetPostion(const Vector3& pos)
	{
		m_bulletBase->SetPostion(pos);
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale �g�嗦
	void SetScale(const Vector3& scale)
	{
		m_bulletBase->SetScale(scale);
	}

	/// @brief ��]��ݒ�
	/// @param rot	��]
	void SetRotation(const Quaternion rot)
	{
		m_bulletBase->SetRotation(rot);
	}

	/// @brief �p�x��ݒ�
	/// @param angle �p�x
	void SetAngle(const float& angle)
	{
		m_bulletBase->SetAngle(angle);
	}

	/// @brief ���݃t���O���I��(true)�ɂ���
	void SetIsExist()
	{
		m_bulletBase->SetIsExist();
	}

	/// @brief �ϋv�l������
	/// @param decVal ����������l
	void DecLife(float decVal)
	{
		m_bulletBase->DecLife(decVal);
	}

private:
	//�����Ŏg���֐�
	
	/// @brief ��������1�x�����Ă΂�鏈���Btrue�Ŋ��� 
	/// @return 
	bool Start() override;

	/// @brief �X�V����
	void Update() override;

private:	
	
	//�e�̊��N���X
	BulletBase* m_bulletBase = nullptr;			
	
	//�e�̔h���N���X
	BulletPlayerNormal m_bulletPlayerNormal;		//�v���C���[�ʏ�e
	BulletPlayerSpread m_bulletPlayerSpread;		//�v���C���[�X�v���b�h�{��
	BulletEnemyNormal m_bulletEnemyNormal;			//�G�l�~�[�ʏ�e
};

