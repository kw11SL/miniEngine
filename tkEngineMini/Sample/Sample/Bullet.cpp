#include "stdafx.h"
#include "Bullet.h"
#include "BulletManager.h"

namespace{
	//���f�����̃t�@�C���p�X
	const char* MODELPATH_PLAYER_NORMAL = "Assets/modelData/bullet/bullet.tkm";
	const char* MODELPATH_PLAYER_SPREAD_BOMB = "Assets/modelData/bullet/bullet.tkm";

	const char* MODELPATH_ENEMY_NORMAL = "Assets/modelData/bullet/bullet.tkm";
	
	//�n�`����ǂꂭ�炢�������邩
	const float UPPER_OFFSET = 50.0f;

	//�e�̑ϋv�l
	const float LIFE_PLAYER_NORMAL = 1.0f;
	const float LIFE_PLAYER_SPREAD_BOMB = 1.0f;
	
	const float LIFE_ENEMY_NORMAL = 1.0f;
	
	//�e���Ƃ̒e��
	const float MOVE_SPEED_PLAYER_NORMAL = 25.0f;
	const float MOVE_SPEED_PLAYER_SPREAD_BOMB = 10.0f;
	
	const float MOVE_SPEED_ENEMY_NORMAL = 3.0f;

	//�X�v���b�h�{���̑��x����
	const float SPREAD_BOMB_DEC_RATE = 0.15f;

	//�e�̎c������
	const float LIFETIME_PLAYER_NORMAL = 1.0f;
	const float LIFETIME_PLAYER_SPREAD_BOMB = 1.5f;
	
	const float LIFETIME_ENEMY_NORMAL = 6.0f;

	//�e�̈З�
	const float POWER_PLAYER_NORMAL = 5.0f;
	const float POWER_PLAYER_SPREAD_BOMB = 1.0f;
	
	const float POWER_ENEMY_NORMAL = 1.0f;

	//�_���[�W��^����Ԋu
	const float DAMAGE_INTERVAL_PLAYER_NORMAL = 0.15f;

	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* EFFECT_FILEPATH_PLAYER_NORMAL = u"Assets/effect/shot_pl1.efk";
	const char16_t* EFFECT_FILEPATH_PLAYER_SPREAD_BOMB = u"Assets/effect/shot_pl_spread.efk";
	const char16_t* EFFECT_FILEPATH_PLAYER_SPREAD_BOMB_BURST = u"Assets/effect/shot_spread_burst.efk";

	
	const char16_t* EFFECT_FILEPATH_ENEMY_NORMAL = u"Assets/effect/shot_pl1.efk";

	//�V�F�[�_�[�̃t�@�C���p�X
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	//�V�F�[�_�[�̃G���g���[�|�C���g��
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	//�X�L�����f���̃G���g���[�|�C���g��
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	
	//�������W
	const Vector3 INIT_POINT = { 0.0f,0.0f,0.0f };

	const float CHARACON_RADIUS = 50.0f;
	const float CHARACON_HEIGHT = 120.0f;
}


Bullet::~Bullet()
{
	//�G�t�F�N�g���~
	m_shotEffect.Stop();
	DeleteGO(m_skinModelRender);
}

bool Bullet::Start()
{
	return true;
}

void Bullet::Init(
	RenderingEngine& renderingEngine,
	const Vector3& initPoint, 
	const Vector3& initUp,
	const Vector3& direction, 
	const EnBulletType& bulletType)
{
	
	m_skinModelRender = NewGO<SkinModelRender>(0);

	const char* modelPath = "hoge";
	
	//�e�̃^�C�v��ݒ�
	m_enBulletType = bulletType;

	switch (m_enBulletType) {
	case enPlayerNormal:
		modelPath = MODELPATH_PLAYER_NORMAL;
		m_life = LIFE_PLAYER_NORMAL;
		m_lifeTime = LIFETIME_PLAYER_NORMAL;
		m_power = POWER_PLAYER_NORMAL;
		m_speed = MOVE_SPEED_PLAYER_NORMAL;

		//�e�X�g�F�_���[�W�Ԋu
		m_damageInterval = DAMAGE_INTERVAL_PLAYER_NORMAL;
		break;

	case enPlayerSpreadBomb:
		modelPath = MODELPATH_PLAYER_SPREAD_BOMB;
		m_life = LIFE_PLAYER_SPREAD_BOMB;
		m_lifeTime = LIFETIME_PLAYER_SPREAD_BOMB;
		m_power = POWER_PLAYER_SPREAD_BOMB;
		m_speed = MOVE_SPEED_PLAYER_SPREAD_BOMB;

		m_damageInterval = DAMAGE_INTERVAL_PLAYER_NORMAL;
		break;

	case enEnemyNormal:
		modelPath = MODELPATH_ENEMY_NORMAL;
		m_life = LIFETIME_ENEMY_NORMAL;
		m_lifeTime = LIFETIME_ENEMY_NORMAL;
		m_power = POWER_ENEMY_NORMAL;
		m_speed = MOVE_SPEED_ENEMY_NORMAL;

		m_damageInterval = DAMAGE_INTERVAL_PLAYER_NORMAL;
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
		m_position
	);

	//�������ݒ�
	m_up = initUp;

	//�������x�N�g���͏�����̔���
	m_downVector = m_up * -1.0f;
	//�������x�N�g���𐳋K��
	m_downVector.Normalize();

	//�i�s�������w��
	m_direction = direction;
	m_direction.Normalize();

	//�G�t�F�N�g�̏�����
	InitEffect(bulletType);

	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	m_sphericalMove.Init(m_forward, m_right, m_up);
	
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
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector,UPPER_OFFSET);
	
	//�X�V�O�̑O���x�N�g�����L�^
	m_oldForward = m_forward;

	//����������ʂ̖@���ōX�V���A�E�ƑO�����X�V
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//���f���̍��W���X�V
	m_skinModelRender->SetPosition(m_position);

	//�X�v���b�h�{���̑��x��������
	if (m_enBulletType == enPlayerSpreadBomb) {
		m_speed -= SPREAD_BOMB_DEC_RATE;
	}
	if (m_speed < 0.0f) {
		m_speed = 0.0f;
	}

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
	m_lifeTime -= g_gameTime->GetFrameDeltaTime();
	

	if (m_life < 0.0f 
		||m_lifeTime < 0.0f
		||m_speed < 0.0f) {
		
		m_isExist = false;
	}
}

void Bullet::Destroy()
{
	//���݃t���O���I�t�ɂȂ����Ƃ�
	if (m_isExist == false) {

		//���g���X�v���b�h�{���̂Ƃ�
		if (m_enBulletType == enPlayerSpreadBomb) {

			m_spreadExplosion = NewGO<Explosion>(0, "explosion");
			m_spreadExplosion->Init(
				m_position,
				10.0f,
				enPlayer_Spread_Bomb
			);
		}
		
	}
}

void Bullet::InitEffect(const EnBulletType& bulletType)
{
	switch(bulletType)
	{
	case enPlayerNormal:
		m_shotEffect.Init(EFFECT_FILEPATH_PLAYER_NORMAL);
		break;
	case enPlayerSpreadBomb:
		m_shotEffect.Init(EFFECT_FILEPATH_PLAYER_SPREAD_BOMB);
		m_spreadBurstEffect.Init(EFFECT_FILEPATH_PLAYER_SPREAD_BOMB_BURST);
		break;
	case enEnemyNormal:
		m_shotEffect.Init(EFFECT_FILEPATH_ENEMY_NORMAL);
		break;
	default:
		break;
	}

	//�ʒu�A��]�A�g�嗦��ݒ�
	m_shotEffect.SetPosition(m_position);
	m_shotEffect.SetRotation(m_rot);
	m_shotEffect.SetScale({ 15.0f,15.0f,15.0f });

}

void Bullet::Update()
{


	Move();
	Rotation();
	DecLifeTime();
	Destroy();

	m_shotEffect.SetPosition(m_position);
	m_shotEffect.SetRotation(m_rot);
	m_shotEffect.SetScale({ 15.0f,15.0f,15.0f });

	if (m_shotEffect.IsPlay() != true) {
		m_shotEffect.Play(false);
	}

	//�G�t�F�N�g�̍X�V
	m_shotEffect.Update();

}