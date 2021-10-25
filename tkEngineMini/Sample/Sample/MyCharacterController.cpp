#include "stdafx.h"
#include "MyCharacterController.h"

namespace {

	//���C�̎n�_�̈ʒu(�L�����̉������𐳂Ƃ��ċt�����ɐݒ肷��̂ŕ��̒l)
	const float RAY_START_OFFSET = -10.0f;
	//���C�̏I�_�̈ʒu(�L�����̉������𐳂Ƃ��ď������ɐݒ肷��̂Ő��̒l)
	const float RAY_END_OFFSET = 1000.0f;


	//�Փˎ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
	struct SweepResultGround : public btCollisionWorld::RayResultCallback
	{
		bool isHit = false;								//�Փ˃t���O
		Vector3 hitPos = Vector3(0.0f, 0.0f, 0.0f);		//���C�Փ˓_�̍��W
		Vector3 hitNormal;								//���C�Փ˓_�̖@��
		float distToHitPos = FLT_MAX;					//��_�܂ł̋����B
		Vector3 startPos;								//���C�̎n�_
		Vector3 endPos;									//���C�̏I�_

		/// @brief ���C�ƃR���C�_�[���Փ˂������ɌĂ΂��R�[���o�b�N
		/// @param rayResult 
		/// @param normalInWorldSpace 
		/// @return 
		btScalar addSingleResult(
			btCollisionWorld::LocalRayResult& rayResult, 
			bool normalInWorldSpace ) override
		{

			//�Փ˓_�����������Ă��鏈��		
			//�q�b�g�t���O���I��
			isHit = true;
			//���C�̏Փ˓_�̍��W���擾
			Vector3 hitPosTmp;
			//m_hitFraction�F�Փ˓_�̕�ԗ��B�n�_�ƏI�_�̊Ԃ̂ǂ��Ɉʒu���邩�̏��
			//�܂���`�⊮���邱�ƂŏՓ˓_�̍��W�����߂���
			hitPosTmp.Lerp(rayResult.m_hitFraction, startPos, endPos );
			
			hitPos = hitPosTmp;

			if (rayResult.m_hitFraction < distToHitPos) {
				// ������̕����߂��̂ŁA�Փ˓_���X�V
				hitPos = hitPosTmp;
				//�Փ˓_�̖@���������Ă���
				hitNormal.x = rayResult.m_hitNormalLocal.x();
				hitNormal.y = rayResult.m_hitNormalLocal.y();
				hitNormal.z = rayResult.m_hitNormalLocal.z();

				distToHitPos = rayResult.m_hitFraction;
			}
			
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

	//�������I��
	m_isInit = true;
}


const Vector3& MyCharacterController::Execute(Vector3& moveSpeed,Vector3& downVector)
{
	// ���̈ړ���ƂȂ���W���v�Z����B
	Vector3 nextPos = m_position + moveSpeed;
	
	//���C�̍쐬
	//�n�_�ƏI�_���쐬
	Vector3 start;
	Vector3 end;

	//���C�̎n�_�͈ړ���̍��W
	start = nextPos;
	//�������Ƌt�����ɂ�����w��(�Փ˓_���m�肳���邽��)
	start += downVector * RAY_START_OFFSET;
	
	//�I�_�Ɏn�_�̍��W����
	end = start;
	// �I�_�͉�������300
	end += downVector * RAY_END_OFFSET;


	//�֐��I�u�W�F�N�g���`
	SweepResultGround callBack;
	callBack.startPos = start;
	callBack.endPos = end;

	// �������[���h�ɓo�^����Ă���R���C�_�[�ƃ��C�̌����e�X�g
	PhysicsWorld::GetInstance()->RayTest(
		start,
		end,
		callBack
	);

	//�Փ˂��Ă�����
	if (callBack.isHit) {
		
		//���W���Փ˓_�ɂ���
		nextPos = callBack.hitPos;
		
		//�������x�N�g���̍X�V
		//�����������������Ă����@���Ƌt�����ɂ���
		downVector = callBack.hitNormal * -1.0f;
	}

	//�ړ���̍��W���m�肵���̂�m_position�ɑ��
	m_position = nextPos;

	//������̍��W��Ԃ�
	return m_position;
}