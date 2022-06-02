#include "stdafx.h"
#include "EnemyBomb.h"
#include "Player_new.h"

namespace {

	//���f���̃t�@�C���p�X
	const char* MODELPATH = "Assets/modelData/enemy/enemy_bomb.tkm";

	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* DESTROY_EFFECT_FILEPATH = u"Assets/effect/destroy.efk";			//���j�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* HIT_EFFECT_FILEPATH = u"Assets/effect/hit.efk";					//�q�b�g�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* LIFE_EFFECT_FILEPATH = u"Assets/effect/enemy_life_ring.efk";	//���Ԏ����G�t�F�N�g�̃t�@�C���p�X
	
	//�G�t�F�N�g�̃X�P�[���Ȃ�
	const Vector3 EFFECT_DESTROY_SCALE = { 20.0f,20.0f,20.0f };						//���j�G�t�F�N�g�̃X�P�[��
	const Vector3 EFFECT_HIT_SCALE = { 10.0f,10.0f,10.0f };							//�q�b�g�G�t�F�N�g�̃X�P�[��
	const Vector3 EFFECT_LIFE_INIT_SCALE = { 6.0f,6.0f,6.0f };						//���Ԏ����G�t�F�N�g�̃X�P�[��(�J�n)
	const Vector3 EFFECT_LIFE_END_SCALE = { 1.0f,1.0f,1.0f };						//���Ԏ����G�t�F�N�g�̃X�P�[��(�I��)
	const float EFFECT_BOMB_SCALE_RATE = 10.0f;										//�����G�t�F�N�g�̃X�P�[��

	//�p�����[�^�e��
	const float MOVE_SPEED = 3.0f;		//�ړ����x
	const float LIFE = 15.0f;			//�ϋv�x
	const float DURABILITY = 1.0f;		//�e�ւ̋��x�B���l�������e�̑ϋv�x�����B
	const int SCORE = 1000;				//���j���ɉ��Z�����X�R�A
	const float ACTIVATE_COUNT = 0.7f;	//�ڐG���肪�L���ɂȂ�܂ł̃J�E���^�[
	const float HIT_RANGE = 100.0f;		//�����蔻��͈̔�
	const float STOP_RANGE = 60.0f;		//�ړ���~����͈�
	const float RE_MOVE_RANGE = 70.0f;	//�ړ��ĊJ����͈�
	const float LIFE_TIME_BOMB = 5.0f;	//���Ԏ���
	const int BULLET_WAY_NUM = 6;		//�������ɂ΂�܂��e�̐�

}

EnemyBomb::~EnemyBomb()
{
	//���f���̍폜
	DeleteGO(m_skinModelRender);
	//���Ԏ����G�t�F�N�g�̒�~
	m_lifeRingEffect.Stop();
}

void EnemyBomb::InitSub()
{
	//���f����NewGO
	//m_skinModelRender = NewGO<SkinModelRender>(0);
	
	//���f���̏�����
	if (m_skinModelRender != nullptr) {
		m_skinModelRender->Init(MODELPATH, enModelUpAxisZ, true, false);
	}

	//�e��p�����[�^��ݒ�
	m_life = LIFE;
	m_speed = MOVE_SPEED;
	m_score = SCORE;
	m_durability = DURABILITY;
	m_lifeTime = LIFE_TIME_BOMB;

	//���������A�����G�t�F�N�g���Đ��J�n
	m_lifeRingEffect.SetPosition(m_position);
	m_lifeRingEffect.SetRotation(m_rot);
	m_lifeRingEffect.SetScale(EFFECT_LIFE_INIT_SCALE);
	m_lifeRingEffect.Play(false);
}

void EnemyBomb::InitEffect()
{
	//���j�G�t�F�N�g�̏�����
	m_destroyEffect.Init(DESTROY_EFFECT_FILEPATH);
	m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });
	
	//�q�b�g�G�t�F�N�g�̏�����
	m_hitEffect.Init(HIT_EFFECT_FILEPATH);
	m_hitEffect.SetScale({ 10.0f,10.0f,10.0f });
	
	//���Ԏ����G�t�F�N�g�̏�����
	m_lifeRingEffect.Init(LIFE_EFFECT_FILEPATH);
	m_lifeRingEffect.SetScale(EFFECT_LIFE_INIT_SCALE);
}

void EnemyBomb::UpdateEffect()
{
	m_destroyEffect.Update();
	m_hitEffect.Update();
	m_lifeRingEffect.Update();
}

void EnemyBomb::MoveSub()
{
	//�v���C���[�ǔ�����
	Vector3 toPlayer = Vector3::Zero;
	float toPlayerLength = 0.0f;

	//�v���C���[������
	if (m_player == nullptr) {
		m_player = FindGO<Player_new>(PLAYER_NAME);
	}

	//�v���C���[�ւ̃x�N�g�������A���̕������ړ������ɂ���
	if (m_player != nullptr) {
		toPlayer = m_player->GetPosition() - m_position;
		toPlayerLength = toPlayer.Length();
		toPlayer.Normalize();

		m_moveSpeed = toPlayer * m_speed;
	}

	Vector3 playerToMe = m_position - m_player->GetPosition();

	//�v���C���[�̈ʒu�܂ł̃x�N�g�������A��苗���܂ŋ߂Â��Ƒ��x��0�ɂ���
	if (toPlayerLength < STOP_RANGE) {
		m_speed = 0.0f;
	}
	//�����ƈړ��ĊJ
	else if (toPlayerLength >= RE_MOVE_RANGE) {
		m_speed = MOVE_SPEED;
	}
}

void EnemyBomb::RotationSub()
{

}

void EnemyBomb::SelfDestroySub()
{
	//�����𔭐�������
	ExplosionManager::GetInstance()->InitExplosion(
		m_position,
		10.0f,
		enEnemy_Explosion
	);

	//�������ɒe���΂�܂�
	FireBulletEqually(BULLET_WAY_NUM,enEnemyNormal);
}

void EnemyBomb::LifeRingScaling()
{
	//�g�嗦
	Vector3 scale;
	//���݂̎��Ԏ��� / ���Ԏ����̏����l ��0.0�`1.0�̕�ԗ����o��
	float fraction = m_lifeTime / LIFE_TIME_BOMB;
	//�o������ԗ��Ŋg�嗦���ő�l����ŏ��l�܂ŕ��
	scale.Lerp(fraction, EFFECT_LIFE_END_SCALE, EFFECT_LIFE_INIT_SCALE);
	
	//���W�A��]�A�g�嗦��ݒ�
	m_lifeRingEffect.SetPosition(m_position);
	m_lifeRingEffect.SetRotation(m_rot);
	m_lifeRingEffect.SetScale(scale);
	
	//�G�t�F�N�g���Đ�
	if (m_lifeRingEffect.IsPlay() != true) {
		m_lifeRingEffect.Play();
	}
}

void EnemyBomb::UpdateSub()
{
	//���Ԏ���������
	DecLifeTime();
	//���Ԏ����G�t�F�N�g���k��
	LifeRingScaling();
}