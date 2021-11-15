#include "stdafx.h"
#include "Player_new.h"

namespace{
	const char* MODELPATH_UTC = "Assets/modelData/unityChan.tkm";
	const char* SKELETON_PATH_UTC = "Assets/modelData/unityChan.tks";
	const float UTC_RADIUS = 40.0f;
	const float UTC_HEIHGT = 100.0f;
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	
	const Vector3 INIT_POINT = {0.0f,700.0f,0.0f};
	const float UPPER_OFFSET = 0.0f;

	const float CHARACON_RADIUS = 50.0f;
	const float CHARACON_HEIGHT = 120.0f;
	const float PL_MOVE_SPEED = -12.0f;
	const float FIRECOUNTER = 0.15f;
	
	const float CAMERA_ROTATE_FRACTION_ADD_RATE = 0.005f;		//�J�����̉�]�Ɏg����ԌW���ɉ��Z����萔
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MIN = 0.003f;		//�J�����̉�]�Ɏg����ԌW���ɉ��Z����萔
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MAX = 0.03f;		//�J�����̉�]�Ɏg����ԌW���ɉ��Z����萔


	const float CAMERA_MOVESPEED_MAX = 1000.0f;					//�J�����A�����_�̒Ǐ]�ō����x 
}

Player_new::~Player_new()
{
	DeleteGO(m_skinModelRender);
}

void Player_new::Init(RenderingEngine& renderingEngine)
{

	m_skinModelRender = NewGO<SkinModelRender>(0);
	
	m_skinModelRender->Init(
		MODELPATH_UTC, 
		enModelUpAxisZ,
		renderingEngine,
		true,
		false,
		SKELETON_PATH_UTC
	);

	m_position = INIT_POINT;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	//����L�����R���̏�����
	m_myCharaCon.Init(
		m_position
	);

	//�������x�N�g���𐳋K��
	m_downVector.Normalize();

	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	m_sphericalMove.Init(m_forward, m_right, m_up);
	
	//�J���������_���王�_�ւ̃x�N�g����ݒ�
	Vector3 toCamera;
	toCamera.x = 0.0f;
	toCamera.y = 700.0f;
	toCamera.z = 1000.0f;

	//�J�����̏�����
	m_gameCamera.Init(CAMERA_MOVESPEED_MAX);
	//�����_��ݒ�
	m_gameCamera.SetTargetPosition(m_position);
	//���_��ݒ�
	m_gameCamera.SetCameraPosition(m_position + toCamera);
	//�����_�ڕW��ݒ�
	m_gameCamera.SetTargetPositionTarget(m_position);
	//�J�����ڕW��ݒ�
	m_gameCamera.SetCameraPositionTarget(m_position + toCamera);

	//�J�����̏���������g�̏�ɂ��Ă���
	m_cameraUp = m_up;

	//���˕�����O���ɂ��Ă���
	//m_shotDirection = m_forward;
}

bool Player_new::Start()
{

	return true;
}

void Player_new::Move()
{
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
	//m_moveSpeed = g_camera3D->GetRight() * -x * PL_MOVE_SPEED;
	m_moveSpeed = m_right * x * PL_MOVE_SPEED;
	//�v���C���[�̑O��(���A��O)�����ւ̈ړ�
	m_moveSpeed += forward * y * PL_MOVE_SPEED;

	//����L�����R���Ɉړ����x��n��
	m_position = m_myCharaCon.Execute(m_moveSpeed,m_downVector,UPPER_OFFSET);


	// ��x�N�g�����X�V
	//�������x�N�g��(=���C���΂�����)* -1.0�@= �v���C���[�̏�x�N�g��
	Vector3 newUp = m_downVector * -1.0f;
	// ���݂̏�x�N�g������A�V������x�N�g���Ɍ����邽�߂̉�]�N�H�[�^�j�I�����v�Z
	//		���@�J�����̌v�Z�Ŏg���B
	m_rotUpToGroundNormal.SetRotation(m_up, newUp);

	//���g�̏�x�N�g�����X�V
	m_up = newUp;

	//�X�V������x�N�g���ƑO���x�N�g���̊O�ρ@=�@�E�x�N�g��
	//m_right = g_camera3D->GetRight();
	m_right.Cross(m_up,m_forward);
	//���߂��E�x�N�g���ƍX�V������x�N�g���̊O�ρ@=�@�O���x�N�g��
	m_forward.Cross(m_right, m_up);
	
	
	//���f���̍��W�X�V
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
	//R1�{�^���������Ɣ��ˁA�������ςȂ��ŘA��
	if (g_pad[0]->IsPress(enButtonRB1)) {
		
		//�J�E���^�[��0�̂Ƃ��ƃJ�E���^�[�����l�𒴂���Ɣ���
		if (m_fireCounter > FIRECOUNTER || m_fireCounter == 0.0f) {
			m_bullet = NewGO<Bullet>(0, "bullet");
			m_bullet->Init(
				*RenderingEngine::GetInstance(),
				m_position,
				m_shotDirection,
				m_enBulletType
			);

			//���ˌ�A�J�E���^�[��0�Ƀ��Z�b�g
			m_fireCounter = 0.0f;
		}
	}

	//R1�{�^���������A�J�E���^�[�����Z�A�����Ă��Ȃ��Ƃ��̓J�E���^�[��0�ɂ���B
	if (g_pad[0]->IsPress(enButtonRB1)) {
		m_fireCounter += g_gameTime->GetFrameDeltaTime();
	}
	else{
		m_fireCounter = 0.0f;
	}

}

void Player_new::ChangeWeapon()
{
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		switch (m_enUseWeapon)
		{
		case enNormalShot:
			m_enUseWeapon = enSpreadBomb;
			m_enBulletType = enPlayerSpreadBomb;
			break;
		case enSpreadBomb:
			m_enUseWeapon = enNormalShot;
			m_enBulletType = enPlayerNormal;
		default:
			break;
		}
	}
}

void Player_new::Update()
{
	float addRate = 0.0f;
	float maxAddRate = CAMERA_ROTATE_FRACTION_ADD_RATE_MAX;
	float minAddRate = CAMERA_ROTATE_FRACTION_ADD_RATE_MIN;

	Vector3 addRateVec = { 0.0f,0.0f,0.0f, };
	Vector3 maxAddRateVec = { maxAddRate,0.0f,0.0f };
	Vector3 minAddRateVec = { minAddRate,0.0f,0.0f };

	float dotVec = Dot(m_cameraUp,m_up);
	dotVec += 1.0f;
	dotVec /= 2.0f;

	addRateVec.Lerp(dotVec, maxAddRateVec, minAddRateVec);
	addRate = addRateVec.Length();

	//�J�����̏��⊮����W�������Z
	//m_cameraUpFraction += CAMERA_ROTATE_FRACTION_ADD_RATE;
	m_cameraUpFraction += addRate;
	//1�𒴂�����1�ɕ␳
	if (m_cameraUpFraction > 1.0f) {
		m_cameraUpFraction = 1.0f;
	}

	Move();
	Rotation();
	RotateShotDirection();
	ChangeWeapon();
	FireBullet();
	
	if (m_skinModelRender != nullptr) {
		m_skinModelRender->SetRotation(m_rot);
	}

	//�O�t���[���̏���������t���[���̏�ƕς���Ă������ԌW����0�ɂ���
	if (m_upPrev.x != m_up.x
		|| m_upPrev.y != m_up.y
		|| m_upPrev.z != m_up.z) {
		m_cameraUpFraction = 0.0f;
	}

	//�J�����Ǐ]
	////�J���������_���王�_�ւ̃x�N�g�����쐬
	//Vector3 toCamera = m_gameCamera.GetCameraPosition() - m_gameCamera.GetTargetPosition();
	//�����_�ڕW����J�����ڕW�ւ̃x�N�g�����쐬
	Vector3 toCamera = m_gameCamera.GetCameraPositionTarget() - m_gameCamera.GetTargetPositionTarget();

	//�x�N�g���ɃN�H�[�^�j�I����K�p
	m_rotUpToGroundNormal.Apply(toCamera);

	////�����_�����g�ɐݒ�
	//m_gameCamera.SetTargetPosition(m_position);
	//�����_�ڕW�����g�ɐݒ�
	m_gameCamera.SetTargetPositionTarget(m_position);

	////���_��ݒ�
	//m_gameCamera.SetCameraPosition(m_position + toCamera);
	//�J�����ڕW��ݒ�
	m_gameCamera.SetCameraPositionTarget(m_position + toCamera);
	
	////�J�����ڕW��ǂ������鏈��
	//m_gameCamera.ChaseCameraPosition();
	////�����_�ڕW��ǂ������鏈��
	//m_gameCamera.ChaseTargetPosition();

	// �J�����̏�����ڕW���v���C���[�̏�����ɐݒ�B
	m_gameCamera.SetUpVectorTarget(m_up);

	//�J�����̏����������Ԃ��Ă���
	m_gameCamera.LerpUpVector(m_cameraUpFraction, m_cameraUp);
	//��Ԃ����J�����̏�ŃJ�����̏���X�V
	m_gameCamera.SetUp(m_cameraUp);
	//�J�����̍X�V
	m_gameCamera.UpdateCamera();
	
	//���t���[���̏���L�^
	m_upPrev = m_up;
}