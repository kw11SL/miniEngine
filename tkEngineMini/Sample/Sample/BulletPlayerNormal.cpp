#include "stdafx.h"
#include "BulletPlayerNormal.h"

namespace {
	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* EFFECT_BULLET_FILEPATH = u"Assets/effect/shot_pl1.efk";						//�e�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* EFFECT_BANISH_FILEPATH = u"Assets/effect/bullet_banish_normal.efk";			//���ŃG�t�F�N�g�̃t�@�C���p�X
	//�G�t�F�N�g�̊g�嗦
	const Vector3 EFFECT_BULLET_SCALE = { 15.0f,15.0f,15.0f };		//�e�G�t�F�N�g�̊g�嗦
	const Vector3 EFFECT_BANISH_SCALE = { 4.0f,4.0f,4.0f };			//���ŃG�t�F�N�g�̊g�嗦

	//���\
	const float LIFE = 1.0f;							//�e�̑ϋv�l
	const float MOVE_SPEED = 40.0f;						//�e��
	const float LIFETIME = 1.5f;						//�e�̎c������
	const float POWER = 5.0f;							//�e�̈З�
	const float DAMAGE_INTERVAL = 0.15f;				//�_���[�W��^����Ԋu(����ɕt�^���閳�G����)
}

BulletPlayerNormal::~BulletPlayerNormal()
{

}

void BulletPlayerNormal::InitSub()
{
	//�e��p�����[�^��ݒ�
	m_life = LIFE;								//�ϋv��
	m_speed = MOVE_SPEED;						//�e��
	m_lifeTime = LIFETIME;						//���Ԏ���
	m_power = POWER;							//�З�
	m_damageInterval = DAMAGE_INTERVAL;			//�_���[�W�Ԋu
}

void BulletPlayerNormal::InitEffectSub()
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

void BulletPlayerNormal::MoveSub()
{

}

void BulletPlayerNormal::RotationSub()
{

}

void BulletPlayerNormal::DestroySub()
{
	//���ŃG�t�F�N�g�̈ʒu��ݒ�
	m_banishEffect.SetPosition(m_position);
	//�g�嗦��ݒ�
	m_banishEffect.SetScale(EFFECT_BANISH_SCALE);
	//�Đ�
	m_banishEffect.Play(false);
}

void BulletPlayerNormal::EffectUpdateSub()
{

}

void BulletPlayerNormal::UpdateSub()
{

}