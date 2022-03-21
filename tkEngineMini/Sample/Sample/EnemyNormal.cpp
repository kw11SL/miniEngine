#include "stdafx.h"
#include "EnemyNormal.h"
#include "Player_new.h"

namespace {
	
	//���f���̃t�@�C���p�X
	const char* MODELPATH = "Assets/modelData/enemy/enemy_common.tkm";

	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* DESTROY_EFFECT_FILEPATH = u"Assets/effect/destroy.efk";		//���j�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* HIT_EFFECT_FILEPATH = u"Assets/effect/hit.efk";				//�q�b�g�G�t�F�N�g�̃t�@�C���p�X

	//�p�����[�^�e��
	const float MOVE_SPEED = 5.0f;		//�ړ����x
	const float LIFE = 1.0f;			//�ϋv�x
	const float DURABILITY = 1.0f;		//�e�ւ̋��x�B���l�������e�̑ϋv�x�����B
	const int SCORE = 100;				//���j���ɉ��Z�����X�R�A
	const float ACTIVATE_COUNT = 0.7f;	//�ڐG���肪�L���ɂȂ�܂ł̃J�E���^�[
	const float HIT_RANGE = 100.0f;		//�����蔻��͈̔�
	const float STOP_RANGE = 60.0f;		//�ړ���~����͈�
	const float RE_MOVE_RANGE = 70.0f;	//�ړ��ĊJ����͈�
}

EnemyNormal::~EnemyNormal()
{
	DeleteGO(m_skinModelRender);
}

void EnemyNormal::InitSub()
{
	//���f���̏�����
	m_skinModelRender->Init(MODELPATH, enModelUpAxisZ, true, false);

	//�e��p�����[�^��ݒ�
	m_life = LIFE;
	m_speed = MOVE_SPEED;
	m_score = SCORE;
	m_durability = DURABILITY;
}

void EnemyNormal::InitEffect()
{
	m_destroyEffect.Init(DESTROY_EFFECT_FILEPATH);
	m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });

	m_hitEffect.Init(HIT_EFFECT_FILEPATH);
	m_hitEffect.SetScale({ 10.0f,10.0f,10.0f });
}

void EnemyNormal::UpdateEffect()
{
	m_destroyEffect.Update();
	m_hitEffect.Update();
}

void EnemyNormal::MoveSub()
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

void EnemyNormal::RotationSub()
{

}

void EnemyNormal::SelfDestroySub()
{

}

void EnemyNormal::UpdateSub()
{

}
