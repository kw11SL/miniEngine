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
	const float LIFETIME_PLAYER_NORMAL = 1.0f;
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


Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
}

bool Bullet::Start()
{
	return true;
}

void Bullet::Init(RenderingEngine& renderingEngine, const Vector3& initPoint,const Vector3& direction)
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

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

	m_skinModelRender->Init(
		modelPath,
		enModelUpAxisZ,
		renderingEngine,
		true,
		false
	);

	//�����ʒu������
	m_position = initPoint;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale * 3.0f);

	//���C�g���������Ď󂯎��
	m_directionLight = FindGO<DirectionLight>("directionlight");
	m_pointLight = FindGO<PointLight>("pointlight");
	m_spotLight = FindGO<SpotLight>("spotlight");
	
	RecieveDirectionLight(m_directionLight);
	RecievePointLight(m_pointLight);
	RecieveSpotLight(m_spotLight);

	//���f�����X�V
	InitModelFromInitData();

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

	//�i�s�������w��
	m_direction = direction;
	m_direction.Normalize();
}

void Bullet::Move()
{
	//�E�x�N�g���͔��˕����Ə�����̊O��
	m_right = Cross(m_up, m_direction);
	
	//���ʂ𔭎˕����ōX�V(����̂�)
	if (m_isDecideDirection == false) {
		m_forward += m_direction;
		//���˕��������܂����̂Ńt���O��true
		m_isDecideDirection = true;
	}

	//�ȍ~�A�O�����X�V���Ȃ���i�ݑ�����
	m_moveSpeed = m_forward * m_speed;

	//�L�����R���ɂ����W�X�V
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector);
	
	//�X�V�O�̑O���x�N�g�����L�^
	m_oldForward = m_forward;

	//����������ʂ̖@���ōX�V���A�E�ƑO�����X�V
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//���f���̍��W���X�V
	m_skinModelRender->SetPosition(m_position);
}

void Bullet::Rotation()
{
	//���f���̌�����ς��鏈��
	//��]�N�H�[�^�j�I�����쐬
	Quaternion rot;
	//�L�^���Ă������X�V�O�̑O������X�V��̑O���ɉ�]����N�H�[�^�j�I�����v�Z
	rot.SetRotation(m_oldForward, m_forward);
	
	Quaternion mulRot;
	//�N�H�[�^�j�I������Z
	mulRot.Multiply(m_rot, rot);
	//��Z�����N�H�[�^�j�I���Ń��f������]
	m_skinModelRender->SetRotation(mulRot);


	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
}

void Bullet::DecLifeTime()
{
	m_life -= g_gameTime->GetFrameDeltaTime();
	
	if (m_life < 0.0f) {
		DeleteGO(this);
	}
}

void Bullet::Update()
{
	Move();
	Rotation();
	DecLifeTime();

	m_skinModelRender->SetRotation(m_rot);

}