#include "stdafx.h"
#include "MyCharacterController.h"

namespace {

	//�Փˎ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
	struct SweepResultGround : public btCollisionWorld::ConvexResultCallback 
	{
		bool isHit = false;									//�Փ˃t���O
		Vector3 hitPos = Vector3(0.0f, 0.0f, 0.0f);		//���C�Փ˓_�̍��W
		Vector3 hitNormal;								//���C�Փ˓_�̖@��

		//btCollisionObject* me = nullptr;					//�������g�����O���邽�߂̃����o

		virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) 
		{
			////�������g���Փ˂��珜�O���鏈��
			//if (convexResult.m_hitCollisionObject == me) {
			//	return 0.0f;
			//}


			//�Փ˓_�����������Ă��鏈��
			
			//�q�b�g�t���O���I��
			isHit = true;

			//���C�̏Փ˓_�̍��W���擾
			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
			//�擾�����Փ˓_���i�[
			hitPos = hitPosTmp;

			////���C�̏Փ˓_�̖@�����擾
			//Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
			////�擾�����@�����i�[
			//hitNormal = hitNormalTmp;

			return 0.0f;
		}
	};
}


void MyCharacterController::Init(float radius, float height, Vector3& pos)
{
	//���W��m_position�ɐݒ肷��
	//���W�̓��f���̌��_
	SetPosition(pos);

	////////////////////////////////////////////////////////////////////////////////////////
	//�J�v�Z���R���C�_�[�̏�����
	m_radius = radius;
	m_height = height;
	//���a�ƍ�����n���A�J�v�Z���R���C�_�[��������
	m_cupsuleCollider.Init(m_radius, m_height);
	////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////
	//���̂̏������f�[�^���쐬
	RigidBodyInitData rbInitData;
	//�J�v�Z���R���C�_�[�����̂ɓo�^
	rbInitData.collider = &m_cupsuleCollider;
	//���ʂ�ݒ�
	rbInitData.mass = 0.0f;
	
	//���̂����������ŏ�����
	m_rigidBody.Init(rbInitData);
	////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////
	//���̂̊e��ݒ�
	//���̂̃��[���h���W���擾
	btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
	//���̂̊�_���X�V
	//�J�v�Z���R���C�_�[�̍����̔�������_�ɂ��Ă���
	trans.setOrigin(btVector3(pos.x, pos.y + m_height * 0.5, pos.z));
	//���̂ɃR���W����������t�^����(=�����𕪂���ۂ̎��ʂɎg�p)
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
	m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	////////////////////////////////////////////////////////////////////////////////////////


	//�������I��
	m_isInit = true;
}

void MyCharacterController::RemoveRigidBody()
{
	PhysicsWorld::GetInstance()->RemoveRigidBody(m_rigidBody);
}

const Vector3& MyCharacterController::Execute(Vector3& moveSpeed,Vector3& downVector)
{
	//���W��ۑ�
	Vector3 posTmp = m_position;

	// �ړ����x���g���āA�L�����R���̍��W�𓮂���
	// 1�t���[���̈ړ��ʂ��v�Z����
	Vector3 addPos = moveSpeed;
	//m_position += addPos;
	
	// step-1 �������Ƀ��C���΂��āA�n�ʂƂ̓����蔻����s���B
	// �q���g�@�����蔻���CharaCon��179�s��
	
	//���C�̍쐬
	//�n�_�ƏI�_���쐬
	btTransform start;
	btTransform end;

	start.setIdentity();
	end.setIdentity();

	//���C�̎n�_���R���C�_�̒�ӂɂ���
	start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));

	////���C�̎n�_����I�_�ւ̃x�N�g�������
	//btVector3 endTmp = start.getOrigin() + (btVector3(downVector.x, downVector.y, downVector.z));
	
	////���C�̏I�_���R���C�_���牺�����x�N�g���̏I�_�ɂ���
	//end.setOrigin(endTmp);

	end.setOrigin(btVector3(downVector.x, downVector.y, downVector.z));
	//end.setOrigin(btVector3(posTmp.x, posTmp.y - 10.0f, posTmp.z));


	//�֐��I�u�W�F�N�g���`
	SweepResultGround callBack;
	//callBack.me = m_rigidBody.GetBody();

	// step-2 �Ԃ����Ă�����Am_position�Ɍ�_�̍��W��������
	// �q���g�@�Ԃ����Ă���Ƃ��ɌĂ΂��R�[���o�b�N�֐��̓L�����R����27�s�ځB

	//���C�̏Փ˂����o���A�֐��I�u�W�F�N�g�Ɍ��ʂ��i�[����
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_cupsuleCollider.GetBody(),
		start,
		end,
		callBack
	);

	//�Փ˂��Ă�����
	if (callBack.isHit) {
		//���W���Փ˓_�ɂ���
		m_position = callBack.hitPos;
	}

	//���W���ړ����x�ōX�V
	m_position += addPos;


	//������̍��W��Ԃ�
	return m_position;
}