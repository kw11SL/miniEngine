#include "stdafx.h"
#include "Enemy.h"
#include "Player_new.h"
#include "ExplosionManager.h"

namespace {
	//���f�����̃t�@�C���p�X
	const char* MODELPATH_COMMON = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_POWERED = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_CHASER = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_SHOT = "Assets/modelData/enemy/enemy_shot.tkm";
	const char* MODELPATH_BOMB = "Assets/modelData/enemy/enemy_bomb.tkm";

	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* DESTROY_EFFECT_FILEPATH = u"Assets/effect/destroy.efk";					//���j�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* HIT_EFFECT_FILEPATH = u"Assets/effect/hit.efk";							//�q�b�g�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* SHOT_NOTICE_EFFECT_FILEPATH = u"Assets/effect/enemy_shot_notice.efk";	//�e�����\���G�t�F�N�g�̃t�@�C���p�X
	const char16_t* LIFE_EFFECT_FILEPATH = u"Assets/effect/enemy_life_ring.efk";			//���Ԏ����G�t�F�N�g�̃t�@�C���p�X

	//�G�t�F�N�g�̊g�嗦
	const Vector3 EFFECT_DESTROY_SCALE = { 20.0f,20.0f,20.0f };
	const Vector3 EFFECT_HIT_SCALE = { 10.0f,10.0f,10.0f };
	const Vector3 EFFECT_SHOT_NOTICE_INIT_SCALE = { 15.0f,15.0f,15.0f };
	const Vector3 EFFECT_SHOT_NOTICE_END_SCALE = { 1.0f,1.0f,1.0f };
	const Vector3 EFFECT_LIFE_INIT_SCALE = { 6.0f,6.0f,6.0f };
	const Vector3 EFFECT_LIFE_END_SCALE = { 1.0f,1.0f,1.0f };
	const float EFFECT_BOMB_SCALE_RATE = 10.0f;

	//�n�`���畂�������
	const float UPPER_OFFSET = 50.0f;
	//�����蔻��͈̔�
	const float HIT_RANGE = 100.0f;
	//�ړ���~����͈�
	const float STOP_RANGE = 60.0f;
	//�ړ��ĊJ����͈�
	const float RE_MOVE_RANGE = 70.0f;

	//�G�l�~�[�̃^�C�v���̈ړ����x
	const float MOVE_SPEED_COMMON = 5.0f;
	const float MOVE_SPEED_POWERED = 5.0f;
	const float MOVE_SPEED_CHASER = 10.0f;
	const float MOVE_SPEED_SHOT = 2.0f;
	const float MOVE_SPEED_BOMB = 3.0f;

	//�G�l�~�[�̃^�C�v���̑ϋv�l
	const float LIFE_COMMON = 1.0f;
	const float LIFE_POWERED = 1.0f;
	const float LIFE_CHASER = 1.0f;
	const float LIFE_SHOT = 1.0f;
	const float LIFE_BOMB = 15.0f;

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
	const int SCORE_BOMB = 1000;

	const float LIFE_TIME_BOMB = 5.0f;						//�����^�̎��Ԏ���
	const float ACTIVATE_COUNT = 0.7f;						//�����蔻�肪�L���ɂȂ�܂ł̃J�E���^�[
	const float SHOT_COUNTER = 2.0f;						//�ˌ��܂ł̎���
	const float EFFECT_SHOT_NOTICE_ACTIVATE_TIME = 1.7f;	//�ˌ����O�̃G�t�F�N�g�𔭐�������^�C�~���O

	//�V�F�[�_�[�̃t�@�C���p�X
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	//�V�F�[�_�[�̃G���g���[�|�C���g��
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	//�X�L�����f���̃G���g���[�|�C���g��
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	//�������W
	const Vector3 INIT_POINT = { 0.0f,700.0f,0.0f };
}

Enemy::Enemy()
{
	//����������G�l�~�[����+1
	GameDirector::GetInstance()->AddEnemyCount();
	//�G�l�~�[������+1
	GameDirector::GetInstance()->AddEnemyTotalCount();
}

Enemy::~Enemy()
{
	//���j���ꂽ��G�l�~�[����-1
	GameDirector::GetInstance()->DecEnemyCount();
	DeleteGO(m_skinModelRender);
	
	m_shotNoticeEffect.Stop();
	m_lifeRingEffect.Stop();
}

bool Enemy::Start()
{
	return true;
}

void Enemy::Init(
	const Vector3& initPoint,
	const Vector3& initUp,
	const EnEnemyType& enemyType)
{
	//�e��}�l�[�W���[�̃|�C���^���擾
	m_explosionManager = ExplosionManager::GetInstance();
	m_bulletManager = BulletManager::GetInstance();

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
		m_lifeTime = LIFE_TIME_BOMB;
		break;
	default:
		break;
	}

	m_position = initPoint;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	//���C�g������,�󂯎��
	m_directionLight = FindGO<DirectionLight>(DIRECTION_LIGHT_NAME);
	m_pointLight = FindGO<PointLight>(POINT_LIGHT_NAME);
	m_spotLight = FindGO<SpotLight>(SPOT_LIGHT_NAME);
	if (m_directionLight != nullptr) { RecieveDirectionLight(m_directionLight); }
	if (m_pointLight != nullptr) { RecievePointLight(m_pointLight); }
	if (m_pointLight != nullptr) { RecieveSpotLight(m_spotLight); }

	//���f�������_�[�̏�����
	m_skinModelRender->Init(modelPath, enModelUpAxisZ, true, false);

	////���f�����X�V
	//InitModelFromInitData();

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

	m_myCharaCon.Execute(m_moveSpeed, m_downVector, UPPER_OFFSET);


	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	m_sphericalMove.Init(m_forward, m_right, m_up);
	
	//�����t���O���I��
	m_isExist = true;

	//���G��ԃt���O���I�t
	m_isInvincible = false;

	//�o�����瓖���蔻�肪�L���ɂȂ�܂ł̎��Ԃ��Z�b�g
	m_toActivateCounter = ACTIVATE_COUNT;

	//�G�t�F�N�g�̏�����
	m_destroyEffect.Init(DESTROY_EFFECT_FILEPATH);
	m_hitEffect.Init(HIT_EFFECT_FILEPATH);
	m_shotNoticeEffect.Init(SHOT_NOTICE_EFFECT_FILEPATH);
	m_lifeRingEffect.Init(LIFE_EFFECT_FILEPATH);

	//�����^�G�l�~�[������������\���G�t�F�N�g���Đ��J�n
	if (m_enEnemyType == enBomb) {
		m_lifeRingEffect.SetPosition(m_position);
		m_lifeRingEffect.SetScale(EFFECT_LIFE_INIT_SCALE);
		m_lifeRingEffect.Play(false);
	}

	/*switch (enemyType) {
	case enCommon:
		m_enemyBase = &m_enemyNormal;
		break;
	case enShot:
		m_enemyBase = &m_enemyShot;
		break;
	case enBomb:
		m_enemyBase = &m_enemyBomb;
		break;
	default:
		break;
	}

	m_position = initPoint;
	m_up = initUp;

	m_enemyBase->Init(
		m_position,
		m_up
	);*/

}

void Enemy::Move()
{
	//�v���C���[�ǐՏ���
	Vector3 toPlayer = Vector3::Zero;
	float toPlayerLength = 0.0f;

	//�v���C���[������
	if (m_player == nullptr) {
		m_player = FindGO<Player_new>(PLAYER_NAME);
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
	if (toPlayerLength < STOP_RANGE){
		m_speed = 0.0f;
	}
	else if (toPlayerLength >= RE_MOVE_RANGE && m_enEnemyType == enCommon) {
		m_speed = MOVE_SPEED_COMMON;
	}

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
	//���@�e������
	QueryGOs<Bullet>("bullet", [&](Bullet* bullet) {
		Vector3 diff = bullet->GetPosition() - m_position;
		float length = diff.Length();
		
		//���������l�ȉ��̂Ƃ�
		if (length < HIT_RANGE) {
			
			//���G��ԂłȂ����
			if (m_isInvincible == false) {
				//�ϋv�͂����炷
				m_life -= bullet->GetPower();

				//�q�b�g�G�t�F�N�g�̍Đ�
				m_hitEffect.SetPosition(m_position + m_up * 50.0f);
				m_hitEffect.SetRotation(m_rot);
				m_hitEffect.SetScale(EFFECT_HIT_SCALE);
				m_hitEffect.Play(false);

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
	QueryGOs<Explosion>(EXPLOSION_PLAYER_NAME, [&](Explosion* explosion) {
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

void Enemy::SelfDestroy()
{
	m_destroyEffect.SetPosition(m_position);
	m_destroyEffect.SetRotation(m_rot);
	m_destroyEffect.SetScale(EFFECT_DESTROY_SCALE);
	m_destroyEffect.Play(false);

	//�����^�G�l�~�[�̂Ƃ��͔����𔭐�������
	if (m_enEnemyType == enBomb && GameDirector::GetInstance()->GetTime() > 0.0f) {
		
		m_explosionManager->InitExplosion(
			m_position,
			EFFECT_BOMB_SCALE_RATE,
			enEnemy_Explosion
		);

		//�������ɒe���΂�܂�
		FireBulletEqually(6,enEnemyNormal);

	}
	//���݃t���O���I�t
	m_isExist = false;

}


void Enemy::Destroy()
{
	if (m_life <= 0.0f) {
		m_life = 0.0f;
		
		//�����t���O���I�t
		m_isExist = false;
	}
	//�����t���O���I�t�Ȃ�G�t�F�N�g���o���č폜����
	if (m_isExist == false) {
		m_destroyEffect.SetPosition(m_position);
		m_destroyEffect.SetRotation(m_rot);
		m_destroyEffect.SetScale(EFFECT_DESTROY_SCALE);
		m_destroyEffect.Play(false);
		
		//���jse�̍Đ�
		CSoundSource* ssDestroy = NewGO<CSoundSource>(0);
		ssDestroy->Init(L"Assets/wav/destroySe.wav");
		ssDestroy->SetVolume(0.6f);
		ssDestroy->Play(false);
		
		//�_�������_
		GameDirector::GetInstance()->AddScore(m_score);
		//�v���C���[�̌��j������+1
		GameDirector::GetInstance()->AddDestroyedEnemyCount();
	}

}

void Enemy::DestroyTimeUp()
{
	//�^�C���A�b�v�������玩��
	if (GameDirector::GetInstance()->GetTime() <= 0.0f) {
		SelfDestroy();
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

void Enemy::DecToActivateTime()
{
	m_toActivateCounter -= g_gameTime->GetFrameDeltaTime();

	//���@�Ƃ̐ڐG�����L���ɂ���
	if (m_toActivateCounter <= 0.0f) {
		m_toActivateCounter = 0.0f;
		m_isActive = true;
	}
}

void Enemy::DecLifeTime()
{
	m_lifeTime -= g_gameTime->GetFrameDeltaTime();

	//���Ԏ���������
	if (m_lifeTime <= 0.0f) {
		m_lifeTime = 0.0f;
		SelfDestroy();
	}
}

void Enemy::AddShotCounter()
{
	//���˃J�E���^�[�̑���
	if (m_enEnemyType == enShot) {
		m_shotCounter += g_gameTime->GetFrameDeltaTime();
	}
}

void Enemy::FireBulletEqually(const int wayNum,const EnBulletType& bulletType)
{
	//����0����������s���Ȃ�
	if (wayNum == 0) {
		return;
	}

	//�v���C���[�������i�[����x�N�g�����쐬
	Vector3 toPlayer = Vector3::Zero;
	//�v���C���[�����ւ̃x�N�g�����v�Z�A���K��
	if (m_player != nullptr) {
		toPlayer = m_player->GetPosition() - m_position;
		toPlayer.Normalize();
	}

	Vector3 dir = toPlayer;
	//�p�x�����߂邽�߂̉�]�N�H�[�^�j�I�����쐬
	Quaternion rot;

	//�w�肵���������Œe�ƒe�̊Ԃ̊p�x���v�Z(���W�A��)
	//��*2(=2�� = 1��)�𓙕����Ă���
	float angle = Math::PI * 2.0f / (float)wayNum;

	//for���ɑ������ޗp�̊p�x
	float angleTmp = 0.0f;
	
	//�e�̊p�x����,�ˌ�
	for (int i = 0; i < wayNum; i++) {
		//��e��1����
		if (i == 0 && wayNum % 2 == 1) {
			//�v���C���[������1����
			rot.SetRotation(m_up, 0.0f);
			rot.Apply(dir);
		}
		//�����e��1����(�v���C���[�����ɂ͏o���Ȃ�)
		else if (i == 0 && wayNum % 2 == 0) {
			//�v���C���[���������Ȃ̂�1���ڂ��o������́A
			//(���������p�x / 2)���W�A����]�����������ƂȂ�
			angleTmp += angle / 2.0f;
			rot.SetRotation(m_up, angleTmp);
			rot.Apply(dir);
		}
		//2���ڈȍ~�͓��������p�x�����˕�������
		else {
			angleTmp += angle;
			rot.SetRotation(m_up, angleTmp);
			rot.Apply(dir);
		}

		//�e�𐶐�
		m_bulletManager->InitBullets(
			m_position,
			m_up,
			dir,
			bulletType
		);

		//������A�������v���C���[�����ɖ߂�(���ʂ���ɕ��������肵�Ă��邽��)
		dir = toPlayer;
		dir.Normalize();
	}
		
	//���˃J�E���^�[��߂�
	m_shotCounter = 0.0f;
	
}

void Enemy::UpdateEffect()
{
	//�G�t�F�N�g�̍X�V
	m_destroyEffect.Update();
	m_hitEffect.Update();
	m_shotNoticeEffect.Update();
	m_lifeRingEffect.Update();
}

void Enemy::LifeRingScaling()
{
	if (m_enEnemyType == enBomb) {
		Vector3 scale;
		//���݂̎��Ԏ��� / ���Ԏ����̏����l ��0.0�`1.0�̕�ԗ����o��
		float fraction = m_lifeTime / LIFE_TIME_BOMB;
		//�o������ԗ��ŃX�P�[�����ő�l����ŏ��l�܂ŕ��
		scale.Lerp(fraction, EFFECT_LIFE_END_SCALE, EFFECT_LIFE_INIT_SCALE);

		m_lifeRingEffect.SetPosition(m_position);
		m_lifeRingEffect.SetRotation(m_rot);
		m_lifeRingEffect.SetScale(scale);

		if (m_lifeRingEffect.IsPlay() != true) {
			m_lifeRingEffect.Play();
		}
	}
}

void Enemy::ShotNoticeScaling(const float activateTime)
{
	//�e�X�g�F���˗\���G�t�F�N�g�̃X�P�[�����O
	if (m_enEnemyType == enShot) {
		Vector3 scale;
		const float remain = activateTime;
		//�X�P�[�����O�Ɏg����ԗ����v�Z
		float fraction = (m_shotCounter - remain) / (SHOT_COUNTER - remain);
		if (m_shotCounter < remain) {
			fraction = 0.0f;
		}

		//�g�嗦����`�⊮�A�X�P�[�����O
		scale.Lerp(fraction, EFFECT_SHOT_NOTICE_INIT_SCALE, EFFECT_SHOT_NOTICE_END_SCALE);
		m_shotNoticeEffect.SetPosition(m_position);
		m_shotNoticeEffect.SetRotation(m_rot);
		m_shotNoticeEffect.SetScale(scale);

		if (m_shotCounter >= remain && m_shotCounter < SHOT_COUNTER) {
			if (m_shotNoticeEffect.IsPlay() == false) {
				m_shotNoticeEffect.Play();
			}
		}
		//�V���b�g�J�E���^�[��0�ɂȂ��Ă���΍Đ����~
		else if (m_shotCounter == 0.0f) {
			m_shotNoticeEffect.Stop();
		}
	}
}

void Enemy::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if(GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//m_enemyBase->Update();

	

	Move();
	Rotation();
	Hit();
	DecInvTime();
	DecToActivateTime();
	Destroy();
	DestroyTimeUp();
	AddShotCounter();

	//�e�X�g�F�ˌ��J�E���^���őS���ʎˌ�
	if (m_shotCounter > SHOT_COUNTER) {
		FireBulletEqually(1,enEnemyNormal);
	}

	//�����^�G�l�~�[�̂Ƃ��A���Ԏ������}�C�i�X
	if (m_enEnemyType == enBomb) {
		DecLifeTime();
	}

	LifeRingScaling();
	ShotNoticeScaling(EFFECT_SHOT_NOTICE_ACTIVATE_TIME);
	
	//�G�t�F�N�g�̍X�V
	UpdateEffect();

	//���f�������_�[�̍X�V
	m_skinModelRender->SetRotation(m_rot);

}