#include "stdafx.h"
#include "BulletEnemyNormal.h"

namespace {
	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* EFFECT_BULLET_FILEPATH = u"Assets/effect/enemy_bullet_1.efk";						//�e�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* EFFECT_BANISH_FILEPATH = u"Assets/effect/bullet_banish_enemy.efk";			//���ŃG�t�F�N�g�̃t�@�C���p�X
	//�G�t�F�N�g�̊g�嗦
	const Vector3 EFFECT_BULLET_SCALE = { 15.0f,15.0f,15.0f };		//�e�G�t�F�N�g�̊g�嗦
	const Vector3 EFFECT_BANISH_SCALE = { 4.0f,4.0f,4.0f };			//���ŃG�t�F�N�g�̊g�嗦

	//���\
	const float LIFE = 1.0f;							//�e�̑ϋv�l
	const float MOVE_SPEED = 8.0f;						//�e��
	const float LIFETIME = 1.0f;						//�e�̎c������
	const float POWER = 1.0f;							//�e�̈З�
	const float DAMAGE_INTERVAL = 0.15f;				//�_���[�W��^����Ԋu(����ɕt�^���閳�G����)
}

BulletEnemyNormal::~BulletEnemyNormal()
{

}

void BulletEnemyNormal::InitSub()
{
	//�e��p�����[�^��ݒ�
	m_life = LIFE;								//�ϋv��
	m_speed = MOVE_SPEED;						//�e��
	m_lifeTime = LIFETIME;						//���Ԏ���
	m_power = POWER;							//�З�
	m_damageInterval = DAMAGE_INTERVAL;			//�_���[�W�Ԋu
}

void BulletEnemyNormal::InitEffectSub()
{
	//�e�G�t�F�N�g�̏�����
	m_shotEffect.Init(EFFECT_BULLET_FILEPATH);
	//�e�G�t�F�N�g�̊g�嗦��ݒ�
	m_shotEffect.SetScale(EFFECT_BULLET_SCALE);

	//���ŃG�t�F�N�g�̏�����
	m_banishEffect.Init(EFFECT_BANISH_FILEPATH);
	//���ŃG�t�F�N�g�̊g�嗦��ݒ�
	m_banishEffect.SetScale(EFFECT_BANISH_SCALE);
}

void BulletEnemyNormal::MoveSub()
{

}

void BulletEnemyNormal::RotationSub()
{

}

void BulletEnemyNormal::DestroySub()
{
	//���ŃG�t�F�N�g�̈ʒu��ݒ�
	m_banishEffect.SetPosition(m_position);
	//�g�嗦��ݒ�
	m_banishEffect.SetScale(EFFECT_BANISH_SCALE);
	//�Đ�
	m_banishEffect.Play(false);
}

void BulletEnemyNormal::EffectUpdateSub()
{

}

void BulletEnemyNormal::UpdateSub()
{

}