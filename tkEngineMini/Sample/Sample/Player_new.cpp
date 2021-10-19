#include "stdafx.h"
#include "Player_new.h"
//#include "MyCharacterController.h"

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

	const float PL_MOVE_SPEED = -10.0f;
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

	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	//�O������z��
	m_forward = g_vec3AxisZ;
	//�E������x��
	m_right = g_vec3AxisX;
	//�������y��
	m_up = g_vec3AxisY;

}

bool Player_new::Start()
{

	return true;
}

void Player_new::SetPostion(const Vector3& pos)
{
	m_position.x = pos.x;
	m_position.y = pos.y;
	m_position.z = pos.z;

	m_skinModelRender->SetPosition(m_position);
}

void Player_new::SetScale(const Vector3& scale)
{
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;

	m_skinModelRender->SetScale(m_scale);
}

void Player_new::SetRotation(const Quaternion rot)
{
	m_rot = rot;
}
void Player_new::SetAngle(const float& angle)
{
	m_angle = angle;
}

void Player_new::Move()
{
	//�e�X�g�F�ړ�
	//�p�b�h�̃X�e�B�b�N����x������y�������󂯎��
	float x = g_pad[0]->GetLStickXF();
	float y = g_pad[0]->GetLStickYF();

	//�v���C���[�̍��E�����ւ̈ړ�
	m_moveSpeed = m_right * x * PL_MOVE_SPEED;
	//�v���C���[�̑O��(���A��O)�����ւ̈ړ�
	m_moveSpeed += m_forward * y * PL_MOVE_SPEED;

	//�������x�N�g���̍��W�X�V
	//m_downVector.x = m_position.x;
	//m_downVector.z = m_position.z;

	//�d��
	//m_moveSpeed.y += g_gameTime->GetFrameDeltaTime() * -10.0f;

	////�L�����R���Ɉړ����x��n��
	//m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//����L�����R���Ɉړ����x��n��
	m_position = m_myCharaCon.Execute(m_moveSpeed,m_downVector);

	// ��x�N�g�����X�V
	//�������x�N�g��(=���C���΂�����)* -1.0�@= �v���C���[�̏�x�N�g��
	m_up = m_downVector * -1.0f;
	//�X�V������x�N�g���ƑO���x�N�g���̊O�ρ@=�@�E�x�N�g��
	m_right.Cross(m_up, m_forward);
	//���߂��E�x�N�g���ƍX�V������x�N�g���̊O�ρ@=�@�O���x�N�g��
	m_forward.Cross(m_right, m_up);
	
	
	//���f�������_�[�̍��W�X�V
	m_skinModelRender->SetPosition(m_position);
}

void Player_new::Rotation()
{
	// �L�����N�^�[�̑O���A�E�A�ォ���]�N�H�[�^�j�I�������߂�B
	Matrix mRot;
	// ��]�s���1�s�ڂ́A���̍��W�n��ex�ɂȂ�
	mRot.m[0][0] = m_right.x;
	mRot.m[0][1] = m_right.y;
	mRot.m[0][2] = m_right.z;
	
	// ��]�s���1�s�ڂ́A���̍��W�n��ey�ɂȂ�
	mRot.m[1][0] = m_up.x;
	mRot.m[1][1] = m_up.y;
	mRot.m[1][2] = m_up.z;

	// ��]�s���1�s�ڂ́A���̍��W�n��ez�ɂȂ�
	mRot.m[2][0] = m_forward.x;
	mRot.m[2][1] = m_forward.y;
	mRot.m[2][2] = m_forward.z;

	// ��]�s�񂩂�N�H�[�^�j�I�����v�Z����
	m_rot.SetRotation(mRot);
}

void Player_new::RecieveDirectionLight(DirectionLight* dirLight)
{
	m_skinModelRender->InitDirectionLight(dirLight);
}

void Player_new::RecievePointLight(PointLight* ptLight)
{
	m_skinModelRender->InitPointLight(ptLight);
}

void Player_new::RecieveSpotLight(SpotLight* spLight)
{
	m_skinModelRender->InitSpotLight(spLight);
}

SkinModelRender* Player_new::GetSkinModelRender()
{
	return m_skinModelRender;
}

void Player_new::InitModelFromInitData()
{
	m_skinModelRender->InitModel();
}

void Player_new::Update()
{
	Move();
	Rotation();
	
	if (m_skinModelRender != nullptr) {
		m_skinModelRender->SetRotation(m_rot);
	}

	//�e�X�g�F�폜����
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		 DeleteGO(m_skinModelRender);
	}

}