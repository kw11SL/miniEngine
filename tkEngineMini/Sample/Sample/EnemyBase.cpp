#include "stdafx.h"
#include "EnemyBase.h"

namespace {
	float UPPER_OFFSET = 50.0f;
	const float ACTIVATE_COUNT = 0.7f;		//�����蔻�肪�L���ɂȂ�܂ł̃J�E���^�[
}

EnemyBase::EnemyBase()
{
	//����������G�l�~�[����+1
	GameDirector::GetInstance()->AddEnemyCount();
	//�G�l�~�[������+1
	GameDirector::GetInstance()->AddEnemyTotalCount();
}

void EnemyBase::Init(const Vector3& initPoint, const Vector3& initUp)
{
	//���f����NewGO
	m_skinModelRender = NewGO<SkinModelRender>(0);

	const char* modelPath = "hoge";

	m_position = initPoint;
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	//�e�탉�C�g�̎󂯎��
	//���C�g���������Ď󂯎��
	m_directionLight = FindGO<DirectionLight>("directionlight");
	m_pointLight = FindGO<PointLight>("pointlight");
	m_spotLight = FindGO<SpotLight>("spotlight");

	RecieveDirectionLight(m_directionLight);
	RecievePointLight(m_pointLight);
	RecieveSpotLight(m_spotLight);


	//�������ݒ�
	m_up = initUp;
	//����L�����R���̏�����
	m_myCharaCon.Init(
		m_position
	);
	//�������x�N�g���͏�����̔���
	m_downVector = m_up * -1.0f;
	//�������x�N�g���𐳋K��
	m_downVector.Normalize();
	//�L�����R���̏���
	m_myCharaCon.Execute(m_moveSpeed, m_downVector, UPPER_OFFSET);

	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	m_sphericalMove.Init(m_forward, m_right, m_up);

	//�����t���O���I��
	m_isExist = true;
	//���G��ԃt���O���I�t
	m_isInvincible = false;

	//�o�����瓖���蔻�肪�L���ɂȂ�܂ł̎��Ԃ��Z�b�g
	m_toActivateCounter = ACTIVATE_COUNT;

	//�p����Ō��߂�����
	InitSub();
	
}

void EnemyBase::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	Move();
	Rotation();
	Hit();
	DecInvTime();
	DecToActivateTime();
	Destroy();
	DestroyTimeUp();

	//�p����Ō��߂�����
	UpdateSub();
}
