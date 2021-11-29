#include "stdafx.h"
#include "Enemy.h"
#include "Player_new.h"

namespace {
	//���f�����̃t�@�C���p�X
	const char* MODELPATH_COMMON = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_POWERED = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_CHASER = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_SHOT = "Assets/modelData/enemy/enemy_shot.tkm";
	const char* MODELPATH_BOMB = "Assets/modelData/enemy/enemy_bomb.tkm";

	const float UPPER_OFFSET = 50.0f;
	
	//�G�l�~�[�̃^�C�v���̈ړ����x
	const float MOVE_SPEED_COMMON = 3.0f;
	const float MOVE_SPEED_POWERED = 5.0f;
	const float MOVE_SPEED_CHASER = 10.0f;
	const float MOVE_SPEED_SHOT = 2.0f;
	const float MOVE_SPEED_BOMB = 2.0f;

	//�G�l�~�[�̃^�C�v���̑ϋv�l
	const float LIFE_COMMON = 1.0f;
	const float LIFE_POWERED = 1.0f;
	const float LIFE_CHASER = 1.0f;
	const float LIFE_SHOT = 1.0f;
	const float LIFE_BOMB = 10.0f;

	//�G�l�~�[�̃^�C�v���̒e�ւ̉e���l
	const float DURABILITY_COMMON = 1.0f;
	const float DURABILITY_POWERED = 1.0f;
	const float DURABILITY_CHASER = 1.0f;
	const float DURABILITY_SHOT = 1.0f;
	const float DURABILITY_BOMB = 1.0f;

	//�G�l�~�[�̃^�C�v�ʃX�R�A
	const int SCORE_COMMON = 100;
	const int SCORE_POWERED = 500;
	const int SCORE_CHASER = 300;
	const int SCORE_SHOT = 300;
	const int SCORE_BOMB = 200;

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

Enemy::Enemy()
{
	GameDirector::GetInstance()->AddEnemyCount();
}

Enemy::~Enemy()
{
	GameDirector::GetInstance()->DecEnemyCount();
	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	return true;
}

void Enemy::Init(
	RenderingEngine& renderingEngine,
	const Vector3& initPoint,
	const Vector3& initUp,
	const EnEnemyType& enemyType)
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

	const char* modelPath = "hoge";
	
	m_enEnemyType = enemyType;

	switch (m_enEnemyType) {
	case enCommon:
		modelPath = MODELPATH_COMMON;
		m_life = LIFE_COMMON;
		m_speed = MOVE_SPEED_COMMON;
		m_score = SCORE_COMMON;
		m_durability = DURABILITY_COMMON;

		break;
	case enPowered:
		modelPath = MODELPATH_POWERED;
		m_life = LIFE_POWERED;
		m_speed = MOVE_SPEED_POWERED;
		m_score = SCORE_POWERED;
		m_durability = DURABILITY_POWERED;
		break;
	case enChaser:
		modelPath = MODELPATH_CHASER;
		m_life = LIFE_CHASER;
		m_speed = MOVE_SPEED_CHASER;
		m_score = SCORE_CHASER;
		m_durability = DURABILITY_CHASER;
		break;
	case enShot:
		modelPath = MODELPATH_SHOT;
		m_life = LIFE_SHOT;
		m_speed = MOVE_SPEED_SHOT;
		m_score = SCORE_SHOT;
		m_durability = DURABILITY_SHOT;
		break;
	case enBomb:
		modelPath = MODELPATH_BOMB;
		m_life = LIFE_BOMB;
		m_speed = MOVE_SPEED_BOMB;
		m_score = SCORE_BOMB;
		m_durability = DURABILITY_COMMON;
		m_durability = DURABILITY_BOMB;
		break;
	default:
		break;
	}

	m_skinModelRender->Init(modelPath, enModelUpAxisZ, renderingEngine, true, false);

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

	//���f�����X�V
	InitModelFromInitData();

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


	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	m_sphericalMove.Init(m_forward, m_right, m_up);
	
	//�����t���O���I��
	m_exist = true;

	//���G��ԃt���O���I�t
	m_isInvincible = false;

	//�G�t�F�N�g�̏�����
	m_destroyEffect.Init(u"Assets/effect/destroy.efk");


}

void Enemy::Move()
{
	//�v���C���[�ǐՏ���
	Vector3 toPlayer = Vector3::Zero;
	float toPlayerLength = 0.0f;

	//�v���C���[������
	if (m_player == nullptr) {
		m_player = FindGO<Player_new>("player");
	}

	//�v���C���[�ւ̃x�N�g�������A���̕������ړ������ɂ���
	if(m_player != nullptr) {

		toPlayer = m_player->GetPosition() - m_position;
		toPlayerLength = toPlayer.Length();
		toPlayer.Normalize();

		m_moveSpeed = toPlayer * m_speed;
	}

	Vector3 playerToMe = m_position - m_player->GetPosition();
	
	//�v���C���[�̈ʒu�܂ł̃x�N�g�������A��苗���܂ŋ߂Â��Ƒ��x��0�ɂ���
	if (toPlayerLength < 300.0f){
		m_speed = 0.0f;
	}
	else if (toPlayerLength >= 300.0f && m_enEnemyType == enCommon) {
		m_speed = MOVE_SPEED_COMMON;
	}

	//if (toPlayer.Dot(playerToMe) < 0.0f) {
	//	//m_position = m_player->GetPosition();
	//	OutputDebugStringA("hoge");
	//}
	//else {
	//	OutputDebugStringA("true");
	//}

	/*m_moveSpeed = m_right * m_speed * 0.0f;
	m_moveSpeed += m_forward * m_speed;*/

	//�L�����R���ɂ����W�X�V
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector,UPPER_OFFSET);
	//����������ʂ̖@���ōX�V���A�E�ƑO�����X�V
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//���f���̍��W���X�V
	m_skinModelRender->SetPosition(m_position);
}

void Enemy::Rotation()
{
	m_sphericalMove.Rotation(m_forward,m_right,m_up,m_rot);
}

void Enemy::Hit()
{
	//�e������
	QueryGOs<Bullet>("bullet", [&](Bullet* bullet) {
		Vector3 diff = bullet->GetPosition() - m_position;
		float length = diff.Length();
		
		//���������l�ȉ��̂Ƃ�
		if (length < 50.0f) {
			
			//���G��ԂłȂ����
			if (m_isInvincible == false) {
				//�ϋv�͂����炷
				m_life -= bullet->GetPower();
				//�G�l�~�[�ɖ��G���Ԃ�ݒ�
				SetInvincibleTime(bullet->GetDamageInterval());
				m_isInvincible = true;
			}
			
			//�e�̑ϋv�l�����炷
			bullet->DecLife(m_durability);
			
			//�₢���킹�I��
			return false;
		}
		
		//�₢���킹���s
		return true;
	});

	//����������
	QueryGOs<Explosion>("explosion", [&](Explosion* explosion) {
		Vector3 diff = explosion->GetPosition() - m_position;
		float length = diff.Length();

		//�����͈͓̔��̂Ƃ�
		if (length < explosion->GetDamageArea()) {

			//���G��ԂłȂ����
			if (m_isInvincible == false) {
				//�ϋv�͂����炷
				m_life -= explosion->GetPower();
				//�G�l�~�[�ɖ��G���Ԃ�ݒ�
				SetInvincibleTime(explosion->GetDamageInterval());
				m_isInvincible = true;
			}

			//�₢���킹�I��
			return false;
		}

		//�₢���킹���s
		return true;
	});

}

void Enemy::Destroy()
{
	if (m_life <= 0.0f) {
		m_life = 0.0f;
		
		//�����t���O���I�t
		m_exist = false;
	}
	//�����t���O���I�t�Ȃ�G�t�F�N�g���o���č폜����
	if (m_exist == false) {
		m_destroyEffect.SetPosition(m_position);
		m_destroyEffect.SetRotation(m_rot);
		m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });
		m_destroyEffect.Play(false);
		
		DeleteGO(this);

		//�_�������_
		GameDirector::GetInstance()->AddScore(m_score);
	}

}

void Enemy::DecInvTime()
{
	//���G���Ԃ�����
	m_invTime -= g_gameTime->GetFrameDeltaTime();

	
	//0�ȉ��Ȃ疳�G��Ԃ��I�t
	if(m_invTime <= 0.0f){
		m_invTime = 0.0f;
		m_isInvincible = false;
	}

}

void Enemy::Update()
{
	Move();
	Rotation();
	Hit();
	DecInvTime();
	Destroy();

	m_destroyEffect.Update();

	m_skinModelRender->SetRotation(m_rot);

}