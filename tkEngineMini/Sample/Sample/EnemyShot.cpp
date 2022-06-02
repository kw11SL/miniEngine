#include "stdafx.h"
#include "EnemyShot.h"
#include "Player_new.h"

namespace {

	//���f���̃t�@�C���p�X
	const char* MODELPATH = "Assets/modelData/enemy/enemy_shot.tkm";

	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* DESTROY_EFFECT_FILEPATH = u"Assets/effect/destroy.efk";					//���j�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* HIT_EFFECT_FILEPATH = u"Assets/effect/hit.efk";							//�q�b�g�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* SHOT_NOTICE_EFFECT_FILEPATH = u"Assets/effect/enemy_shot_notice.efk";	//�e�����\���G�t�F�N�g�̃t�@�C���p�X

	//�G�t�F�N�g�̃X�P�[��
	const Vector3 EFFECT_DESTROY_SCALE = { 20.0f,20.0f,20.0f };					//���j�G�t�F�N�g�̃X�P�[��
	const Vector3 EFFECT_HIT_SCALE = { 10.0f,10.0f,10.0f };						//�q�b�g�G�t�F�N�g�̃X�P�[��
	const Vector3 EFFECT_SHOT_NOTICE_INIT_SCALE = { 15.0f,15.0f,15.0f };		//�ˌ��\���G�t�F�N�g�̃X�P�[��(�J�n)
	const Vector3 EFFECT_SHOT_NOTICE_END_SCALE = { 1.0f,1.0f,1.0f };			//�ˌ��\���G�t�F�N�g�̃X�P�[��(�I��)

	//�p�����[�^�e��
	const float MOVE_SPEED = 2.0f;		//�ړ����x
	const float LIFE = 1.0f;			//�ϋv�x
	const float DURABILITY = 1.0f;		//�e�ւ̋��x�B���l�������e�̑ϋv�x�����B
	const int SCORE = 300;				//���j���ɉ��Z�����X�R�A
	const float ACTIVATE_COUNT = 0.7f;	//�ڐG���肪�L���ɂȂ�܂ł̃J�E���^�[
	const float HIT_RANGE = 100.0f;		//�����蔻��͈̔�
	const float STOP_RANGE = 60.0f;		//�ړ���~����͈�
	const float RE_MOVE_RANGE = 70.0f;	//�ړ��ĊJ����͈�
	const float SHOT_COUNTER = 2.0f;	//�ˌ��Ԋu
	const int SHOT_WAY_NUM = 1;			//1�x�̔��˒e��

	const float EFFECT_SHOT_NOTICE_ACTIVATE_TIME = 1.7f;	//�ˌ����O�̃G�t�F�N�g�𔭐�������^�C�~���O

}

EnemyShot::~EnemyShot()
{
	//���f���̍폜
	DeleteGO(m_skinModelRender);
	//�\���G�t�F�N�g�̒�~
	m_shotNoticeEffect.Stop();
}

void EnemyShot::InitSub()
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
}

void EnemyShot::InitEffect()
{
	//���j�G�t�F�N�g�̏�����
	m_destroyEffect.Init(DESTROY_EFFECT_FILEPATH);
	m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });
	
	//�q�b�g�G�t�F�N�g�̏�����
	m_hitEffect.Init(HIT_EFFECT_FILEPATH);
	m_hitEffect.SetScale({ 10.0f,10.0f,10.0f });
	
	//�ˌ��̗\���G�t�F�N�g�̏�����
	m_shotNoticeEffect.Init(SHOT_NOTICE_EFFECT_FILEPATH);
	m_shotNoticeEffect.SetScale(EFFECT_SHOT_NOTICE_INIT_SCALE);
}

void EnemyShot::UpdateEffect()
{
	m_destroyEffect.Update();
	m_hitEffect.Update();
	m_shotNoticeEffect.Update();
}

void EnemyShot::MoveSub()
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

void EnemyShot::RotationSub()
{

}

void EnemyShot::SelfDestroySub()
{

}

void EnemyShot::ShotNoticeScaling(const float activateTime)
{
	//�g�嗦
	Vector3 scale;
	//�k�����J�n���鎞�Ԃ�ݒ�
	const float remain = activateTime;
	
	//�X�P�[�����O�Ɏg����ԗ����v�Z
	//��:�ˌ��J�E���^:2.0 �Đ��J�n����:1.7;�@�̂Ƃ�
	//2.0 - 1.7 = 0.3sec �� �X�P�[�����O
	float fraction = (m_shotCounter - remain) / (SHOT_COUNTER - remain);
	
	//�ˌ��J�E���^�ϐ����Đ��J�n���ԂɒB���Ă��Ȃ�
	//(=��ԗ������̒l)�ꍇ�A��ԗ���0
	if (m_shotCounter < remain) {
		fraction = 0.0f;
	}

	//�g�嗦����`�⊮�A�X�P�[�����O
	scale.Lerp(fraction, EFFECT_SHOT_NOTICE_INIT_SCALE, EFFECT_SHOT_NOTICE_END_SCALE);
	m_shotNoticeEffect.SetPosition(m_position);
	m_shotNoticeEffect.SetRotation(m_rot);
	m_shotNoticeEffect.SetScale(scale);


	if (m_shotCounter >= remain && m_shotCounter < SHOT_COUNTER) {
		//�Đ����łȂ���΍Đ�
		if (m_shotNoticeEffect.IsPlay() == false) {
			m_shotNoticeEffect.Play();
		}
	}

	//�V���b�g�J�E���^�[��0�ɂȂ��Ă���΍Đ����~
	else if (m_shotCounter == 0.0f) {
		m_shotNoticeEffect.Stop();
	}
}

void EnemyShot::UpdateSub()
{
	//�ˌ��J�E���^�𑝉�
	AddShotCounter();
	
	//�J�E���^�[���l�Ŏˌ�(�ˌ���͎ˌ��J�E���^��0�ɂȂ�)
	if (m_shotCounter >= SHOT_COUNTER) {
		FireBulletEqually(SHOT_WAY_NUM, enEnemyNormal);
	}

	//���˗\���G�t�F�N�g�𔭐�(�ˌ��J�E���^0�𔻒�Ɏg������)
	ShotNoticeScaling(EFFECT_SHOT_NOTICE_ACTIVATE_TIME);
}