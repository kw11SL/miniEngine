#include "stdafx.h"
#include "BulletBase.h"

namespace{
	const float UPPER_OFFSET = 50.0f;			//�n�`���畂�����鋗��
}

BulletBase::~BulletBase()
{
	//�G�t�F�N�g���~
	m_shotEffect.Stop();
}

void BulletBase::Init(
	const Vector3& initPoint,
	const Vector3& initUp,
	const Vector3& direction
)
{
	/////////////////////////////////////////////////////////
	//���ʏ���
	 
	//�����ʒu������
	m_position = initPoint;

	//�L�����R���̏�����
	m_myCharaCon.Init(m_position);

	//���ʈړ��p�N���X�̏�����
	m_sphericalMove.Init(m_forward, m_right, m_up);

	//�����������
	m_up = initUp;

	//���肵��������̋t�x�N�g�����������ɂȂ�
	m_downVector = m_up * -1.0f;
	m_downVector.Normalize();
	
	//�O��������
	m_direction = direction;
	m_direction.Normalize();
	
	//�E�x�N�g���𔭎˕����Ə�����̊O�ςɂ���
	m_right = Cross(m_up, m_direction);

	/////////////////////////////////////////////////////////

	//�G�t�F�N�g����������
	InitEffect();
	//�p����Œ�`��������
	InitSub();

}

void BulletBase::InitEffect()
{
	//�p����Ō��߂�����(�������A�g�嗦�̐ݒ�Ȃ�)
	InitEffectSub();

	//�e�G�t�F�N�g�̈ʒu�A��]��ݒ�
	m_shotEffect.SetPosition(m_position);
	m_shotEffect.SetRotation(m_rot);
	//���ŃG�t�F�N�g�̈ʒu�A��]��ݒ�
	m_banishEffect.SetPosition(m_position);
	m_banishEffect.SetRotation(m_rot);
}

void BulletBase::InitEffectSub()
{

}

void BulletBase::InitSub()
{

}

void BulletBase::Move()
{
	////////////////////////////////////////////////////////////////////////////////
	//���ʏ���

	//���ʂ𔭎˕����ōX�V(����̂�)
	if (m_isDecideDirection == false) {
		m_forward = m_direction;
		m_forward.Normalize();
		//���˕��������܂����̂Ńt���O��true
		m_isDecideDirection = true;
	}

	//�ȍ~�A�O�����X�V���Ȃ���i�ݑ�����
	m_moveSpeed = m_forward * m_speed;

	//�L�����R���ɂ����W�X�V
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector, UPPER_OFFSET);

	//���̎��_�ł̑O���x�N�g�����L�^
	m_oldForward = m_forward;

	//�x�N�g���̌�����ς��鏈��
	//��������X�V���A�E�ƑO�����X�V
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	////////////////////////////////////////////////////////////////////////////////

	//�p����Ō��߂��ړ�����
	MoveSub();

}

void BulletBase::MoveSub()
{

}

void BulletBase::Rotation()
{
	////////////////////////////////////////////////////////////////////////////////
	//���ʏ���
	
	//�G�t�F�N�g�̌�����ς��鏈��
	//��]�N�H�[�^�j�I�����쐬
	Quaternion rot;
	//�L�^���Ă������X�V�O�̑O������X�V��̑O���ɉ�]����N�H�[�^�j�I�����v�Z
	rot.SetRotation(m_oldForward, m_forward);

	Quaternion mulRot;
	//�N�H�[�^�j�I������Z
	mulRot.Multiply(m_rot, rot);

	//m_rot.Multiply(m_rot, rot);

	//�O���A�E�A��̊e�x�N�g����n���A������ς���
	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
	////////////////////////////////////////////////////////////////////////////////

	//�p����Ō��߂���]����
	RotationSub();
}

void BulletBase::RotationSub()
{

}

void BulletBase::Hit()
{
	//���g�̖��O���G�e�̂Ƃ��A�v���C���[�̒e�Ɣ���������
	if (m_name == BULLET_ENEMY_NAME) {

		QueryGOs<Bullet>(BULLET_PLAYER_NAME, [&](Bullet* bullet) {
			Vector3 diff = bullet->GetPosition() - m_position;
			float length = diff.Length();

			if (length < 60.0f) {
				//���葤�̑ϋv�l������
				bullet->DecLife(m_power);
				//�₢���킹�I��
				return false;
			}

			//�₢���킹���s
			return true;
		});

		QueryGOs<Explosion>(EXPLOSION_PLAYER_NAME, [&](Explosion* explosion) {
			Vector3 diff = explosion->GetPosition() - m_position;
			float length = diff.Length();

			if (length < explosion->GetDamageArea()) {
				//������̑ϋv�l������
				DecLife(explosion->GetPower());
				//�₢���킹�I��
				return false;
			}

			//�₢���킹���s
			return true;
		});

	}
	//���g�̖��O�����@�e�̂Ƃ��A�G�e������
	else if (m_name == BULLET_PLAYER_NAME) {

		QueryGOs<Bullet>(BULLET_ENEMY_NAME, [&](Bullet* bullet) {
			Vector3 diff = bullet->GetPosition() - m_position;
			float length = diff.Length();

			if (length < 60.0f) {
				//���葤�̑ϋv�l������
				bullet->DecLife(m_power);
				//�₢���킹�I��
				return false;
			}

			//�₢���킹���s
			return true;
		});
	}
}

void BulletBase::DecLifeTime()
{
	//���Ԏ���������
	m_lifeTime -= g_gameTime->GetFrameDeltaTime();

	//�e�̑ϋv�l�A���Ԏ����A�����̂����ꂩ��0�ȉ��ɂȂ����Ƃ�
	if (m_life <= 0.0f ||
		m_lifeTime <= 0.0f ||
		m_speed <= 0.0f) {

		//���݃t���O���I�t
		m_isExist = false;
	}
}

void BulletBase::Destroy()
{
	//���݃t���O���I�t�ɂȂ����Ƃ�
	if (m_isExist == false) {
		//�p����Ō��߂�����
		DestroySub();
	}
}

void BulletBase::DestroySub()
{

}

void BulletBase::EffectUpdate()
{
	//�e�̃G�t�F�N�g�̈ʒu�Ɖ�]���X�V
	m_shotEffect.SetPosition(m_position);
	m_shotEffect.SetRotation(m_rot);

	//�Đ����łȂ����
	if (m_shotEffect.IsPlay() != true) {
		//�Đ����ĊJ
		m_shotEffect.Play(false);
	}

	//�e�G�t�F�N�g�̍X�V
	m_shotEffect.Update();
	//���ŃG�t�F�N�g�̍X�V
	m_banishEffect.Update();

	//�p����Ō��߂�����
	EffectUpdateSub();
}

void BulletBase::EffectUpdateSub()
{

}


void BulletBase::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//�ړ�����
	Move();
	//��]����
	Rotation();
	//���Ԏ�������&���݃t���O�I�t����
	DecLifeTime();
	//�����蔻�菈��
	Hit();
	//�j�����̏����Ə��ŃG�t�F�N�g�Đ�
	Destroy();
	//�p����Ō��߂�����
	UpdateSub();
	//�G�t�F�N�g�̍X�V����
	EffectUpdate();
}

void BulletBase::UpdateSub()
{

}


