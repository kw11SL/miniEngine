#pragma once

//#include "CapsuleCollider.h"
//#include "RigidBody.h"

class MyCharacterController
{
public:
	
	/// @brief ����������
	/// @param radius �J�v�Z���R���C�_�[�̔��a
	/// @param height �J�v�Z���R���C�_�[�̍���
	/// @param pos �J�v�Z���R���C�_�[�̈ʒu
	void Init(Vector3& pos);

	/// @brief ���W�̏������s��
	/// @param moveSpeed �ړ����x
	/// @param downVector �������x�N�g��
	/// @param upperOffset �n�`���畂�������
	/// @return ������̍��W
	const Vector3& Execute(Vector3& moveSpeed,Vector3& downVector ,float upperOffset);

private:
	//�����Ŏg���֐�

	/// @brief ���W��ݒ�
	/// @param pos ���W
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}


private:
	bool m_isInit = false;							//�������t���O
	Vector3 m_position = Vector3::Zero;				//���W

	//float m_radius = 0.0f;							//�J�v�Z���R���C�_�[�̔��a
	//float m_height = 0.0f;							//�J�v�Z���R���C�_�[�̍���

};

