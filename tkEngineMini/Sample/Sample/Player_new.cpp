#include "stdafx.h"
#include "Player_new.h"

namespace{
	const char* MODELPATH_UTC = "Assets/modelData/unityChan.tkm";
	const float UTC_RADIUS = 40.0f;
	const float UTC_HEIHGT = 100.0f;
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	const Vector3 INIT_POINT = {0.0f,700.0f,0.0f};

	const float CHARACON_RADIUS = 50.0f;
	const float CHARACON_HEIGHT = 120.0f;

	const float PL_MOVE_SPEED = -15.0f;

	const float FIRECOUNTER = 20.0f;
}

Player_new::~Player_new()
{
	DeleteGO(m_skinModelRender);
}

void Player_new::Init(RenderingEngine& renderingEngine)
{

	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(MODELPATH_UTC, enModelUpAxisZ,renderingEngine,true,false);

	//m_skinModelRender->InitShader(MODEL_SHADER_PATH, VS_ENTRYPOINT_NAME);

	m_position = INIT_POINT;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	////�L�����R���̏�����
	//m_charaCon.Init(
	//	CHARACON_RADIUS,
	//	CHARACON_HEIGHT,
	//	m_position
	//);

	//����L�����R���̏�����
	m_myCharaCon.Init(
		CHARACON_RADIUS,
		CHARACON_HEIGHT,
		m_position
	);

	//�������x�N�g���𐳋K��
	m_downVector.Normalize();

	////�O������z��
	//m_forward = g_vec3AxisZ;
	////�E������x��
	//m_right = g_vec3AxisX;
	////�������y��
	//m_up = g_vec3AxisY;
	
	//����L�����܂Ƃ�
	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	m_sphericalMove.Init(m_forward, m_right, m_up);
	
	//�J���������_���王�_�ւ̃x�N�g����ݒ�
	Vector3 toCamera;
	toCamera.x = 0.0f;
	toCamera.y = 700.0f;
	toCamera.z = 1000.0f;

	//�����_��ݒ�
	m_gameCamera.SetTargetPosition(m_position);
	//���_��ݒ�
	m_gameCamera.SetCameraPosition(m_position + toCamera);

	//���˕�����O���ɂ��Ă���
	//m_shotDirection = m_forward;
}

bool Player_new::Start()
{

	return true;
}

void Player_new::Move()
{
	//m_shotDirection = m_forward;

	//�e�X�g�F�ړ�
	//�p�b�h�̃X�e�B�b�N����x������y�������󂯎��
	float x = g_pad[0]->GetLStickXF();
	float y = g_pad[0]->GetLStickYF();

	//�O���x�N�g�����쐬
	Vector3 forward;
	//��x�N�g���ƃJ�����̉E�x�N�g���̊O�ς�O���x�N�g���ɂ���
	forward.Cross(m_up, g_camera3D->GetRight());
	forward.Normalize();

	//�v���C���[�̍��E�����ւ̈ړ�
	m_moveSpeed = g_camera3D->GetRight() * -x * PL_MOVE_SPEED;
	//�v���C���[�̑O��(���A��O)�����ւ̈ړ�
	m_moveSpeed += forward * y * PL_MOVE_SPEED;

	//����L�����R���Ɉړ����x��n��
	m_position = m_myCharaCon.Execute(m_moveSpeed,m_downVector);

	// ��x�N�g�����X�V
	//�������x�N�g��(=���C���΂�����)* -1.0�@= �v���C���[�̏�x�N�g��
	Vector3 newUp = m_downVector * -1.0f;
	// ���݂̏�x�N�g������A�V������x�N�g���Ɍ����邽�߂̉�]�N�H�[�^�j�I�����v�Z
	//		���@�J�����̌v�Z�Ŏg���B
	m_rotUpToGroundNormal.SetRotation(m_up, newUp);
	
	m_up = newUp;

	//�X�V������x�N�g���ƑO���x�N�g���̊O�ρ@=�@�E�x�N�g��
	m_right = g_camera3D->GetRight();
	//���߂��E�x�N�g���ƍX�V������x�N�g���̊O�ρ@=�@�O���x�N�g��
	m_forward.Cross(m_right, m_up);
	
	
	//���f�������_�[�̍��W�X�V
	m_skinModelRender->SetPosition(m_position);
}

void Player_new::Rotation()
{
	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
}

void Player_new::RotateShotDirection()
{
	//���˕�����������ƃJ�����̉E�̊O�ςɂ��Ă���
	m_shotDirection = Cross(m_up, g_camera3D->GetRight());
	m_shotDirection.Normalize();

	//��]���͏�x�N�g��
	Vector3 axis = m_up;
	//������̉�]�N�H�[�^�j�I�����쐬
	Quaternion rot;

	//�E�X�e�B�b�N�̓��͂��󂯎��
	float x = g_pad[0]->GetRStickXF() * -1.0f;
	float y = g_pad[0]->GetRStickYF() * -1.0f;

	//���͒l����p�x�����߂�
	float angle = atan2f(x, y);

	//������̉�]�����߂�
	rot.SetRotation(m_up, angle);

	//�x�N�g������]
	rot.Apply(m_shotDirection);
	
}

void Player_new::FireBullet()
{
	//RB1�������ƒe�𔭎�
	if (g_pad[0]->IsTrigger(enButtonRB1)) {

		m_bullet = NewGO<Bullet>(0, "bullet");
		m_bullet->Init(
			*RenderingEngine::GetInstance(),
			m_position,
			m_shotDirection
		);
	}
}

void Player_new::Update()
{
	Move();
	Rotation();
	RotateShotDirection();
	FireBullet();
	
	if (m_skinModelRender != nullptr) {
		m_skinModelRender->SetRotation(m_rot);
	}

	//�e�X�g�F�폜����
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		 DeleteGO(m_skinModelRender);
	}

	//�J�����Ǐ]
	//�J���������_���王�_�ւ̃x�N�g�����쐬
	Vector3 toCamera = g_camera3D->GetPosition() - g_camera3D->GetTarget();
	m_rotUpToGroundNormal.Apply(toCamera);

	//�����_�����g�ɐݒ�
	m_gameCamera.SetTargetPosition(m_position);
	//���_��ݒ�
	m_gameCamera.SetCameraPosition(m_position + toCamera);
	// �J�����̏�����̓v���C���[�̏�����Ɠ����B
	m_gameCamera.SetUp(m_up);
	////���ꂼ�ꐳ�K��
	//toCameraTmp.Normalize();
	//upVectorTmp.Normalize();

}