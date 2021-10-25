#include "stdafx.h"
#include "Bullet.h"

namespace{
	//���f�����̃t�@�C���p�X
	const char* MODELPATH_PLAYER_NORMAL = "Assets/modelData/bullet/bullet.tkm";
	const char* MODELPATH_ENEMY_NORMAL = "Assets/modelData/bullet/bullet.tkm";

	//�e���Ƃ̒e��
	const float MOVE_SPEED_PLAYER_NORMAL = 20.0f;
	const float MOVE_SPEED_ENEMY_NORMAL = 3.0f;

	//�G�l�~�[�̃^�C�v���̑ϋv�l
	const float LIFE_PLAYER_NORMAL = 1.0f;
	const float LIFE_ENEMY_NORMAL = 1.0f;

	//�e�̎c������
	const float LIFETIME_PLAYER_NORMAL = 2.0f;
	const float LIFETIME_ENEMY_NORMAL = 6.0f;

	//�V�F�[�_�[�̃t�@�C���p�X
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	//�V�F�[�_�[�̃G���g���[�|�C���g��
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	//�X�L�����f���̃G���g���[�|�C���g��
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	
	//�������W
	const Vector3 INIT_POINT = { 0.0f,700.0f,0.0f };

	const float CHARACON_RADIUS = 50.0f;
	const float CHARACON_HEIGHT = 120.0f;
}

Bullet::Bullet()
{

}

Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
}

bool Bullet::Start()
{
	return true;
}

void Bullet::Init(RenderingEngine& renderingEngine, const Vector3& initPoint)
{
	m_skinModelRender = NewGO<SkinModelRender>(0, "bullet");

	const char* modelPath = "hoge";

	switch (m_enBulletType) {
	case enPlayerNormal:
		modelPath = MODELPATH_PLAYER_NORMAL;
		m_life = LIFE_PLAYER_NORMAL;
		m_speed = MOVE_SPEED_PLAYER_NORMAL;
		break;
	case enEnemyNormal:
		modelPath = MODELPATH_PLAYER_NORMAL;
		m_life = LIFETIME_ENEMY_NORMAL;
		m_speed = MOVE_SPEED_ENEMY_NORMAL;
		break;
	
	default:
		break;
	}

	m_skinModelRender->Init(modelPath, enModelUpAxisZ, renderingEngine, true, false);

	m_position = initPoint;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	//����L�����R���̏�����
	m_myCharaCon.Init(
		CHARACON_RADIUS,
		CHARACON_HEIGHT,
		m_position
	);

	//�������x�N�g���𐳋K��
	m_downVector.Normalize();

	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	m_sphericalMove.Init(m_forward, m_right, m_up);

}

void Bullet::Move()
{
	//�L�����R���ɂ����W�X�V
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector);
	
	//����������ʂ̖@���ōX�V���A�E�ƑO�����X�V
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//���f���̍��W���X�V
	m_skinModelRender->SetPosition(m_position);
}

void Bullet::Rotation()
{

	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
}

void Bullet::Update()
{
	Move();
	Rotation();

	m_skinModelRender->SetRotation(m_rot);

}