#include "stdafx.h"
#include "BulletBase.h"

BulletBase::~BulletBase()
{
	//�G�t�F�N�g���~
	m_shotEffect.Stop();
	DeleteGO(m_skinModelRender);
}

void BulletBase::Init(
	RenderingEngine& renderingEngine,
	const Vector3& initPoint,
	const Vector3& initUp,
	const Vector3& direction
)
{
	//�p����Œ�`��������
	InitSub();

	//�����ʒu������
	m_position = initPoint;
	//�����������
	m_up = initUp;
	//�O��������
	m_direction = direction;
	m_direction.Normalize();

	//���肵��������̔��΂��������ɂȂ�
	m_downVector = m_up * -1.0f;
	m_downVector.Normalize();

	//�L�����R���̏�����
	m_myCharaCon.Init(m_position);

	//���ʈړ��p�N���X�̏�����
	m_sphericalMove.Init(m_forward, m_right, m_up);


}

void BulletBase::InitSub()
{

}

void BulletBase::UpdateSub()
{

}

void BulletBase::Update()
{

}



