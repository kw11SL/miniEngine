#include "stdafx.h"
#include "BulletPlayerSpread.h"

namespace {
	//�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* EFFECT_BULLET_FILEPATH = u"Assets/effect/shot_pl_spread.efk";				//�e�G�t�F�N�g�̃t�@�C���p�X
	const char16_t* EFFECT_BANISH_FILEPATH = u"Assets/effect/bullet_banish_normal.efk";			//���ŃG�t�F�N�g�̃t�@�C���p�X
	//�G�t�F�N�g�̊g�嗦
	const Vector3 EFFECT_BULLET_SCALE = { 15.0f,15.0f,15.0f };		//�e�G�t�F�N�g�̊g�嗦
	const Vector3 EFFECT_BANISH_SCALE = { 4.0f,4.0f,4.0f };			//���ŃG�t�F�N�g�̊g�嗦
	//�����ɐݒ肷��G�t�F�N�g�̊g�嗦
	const float BURST_EFFECT_SCALE_RATE = 10.0f;

	//����SE
	const wchar_t* BURST_SE_FILEPATH = L"Assets/wav/shot_spread_exp.wav";		//����SE�̃t�@�C���p�X
	float BURST_SE_VOLUME = 0.4f;												//�ݒ肷��{�����[��

	//���\
	const float LIFE = 1.0f;							//�e�̑ϋv�l
	const float MOVE_SPEED = 15.0f;						//�e��
	const float LIFETIME = 1.5f;						//�e�̎c������
	const float POWER = 1.0f;							//�e�̈З�
	const float DAMAGE_INTERVAL = 0.15f;				//�_���[�W��^����Ԋu(����ɕt�^���閳�G����)
	const float SPEED_DEC_RATE = 0.15f;					//�X�v���b�h�{���̑��x����
}

BulletPlayerSpread::~BulletPlayerSpread()
{

}

void BulletPlayerSpread::InitSub()
{
	//�e��p�����[�^��ݒ�
	m_life = LIFE;								//�ϋv��
	m_speed = MOVE_SPEED;						//�e��
	m_lifeTime = LIFETIME;						//���Ԏ���
	m_power = POWER;							//�З�
	m_damageInterval = DAMAGE_INTERVAL;			//�_���[�W�Ԋu
}

void BulletPlayerSpread::InitEffectSub()
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

void BulletPlayerSpread::MoveSub()
{
	//���x��������
	m_speed -= SPEED_DEC_RATE;

	//0�����Ȃ�0�ɐݒ�
	if (m_speed < 0.0f) {
		m_speed = 0.0f;
	}
}

void BulletPlayerSpread::RotationSub()
{

}

void BulletPlayerSpread::DestroySub()
{
	//��������SE���Đ�
	CSoundSource* ssBurst = NewGO<CSoundSource>(0);
	ssBurst->Init(BURST_SE_FILEPATH);
	ssBurst->SetVolume(BURST_SE_VOLUME);
	ssBurst->Play(false);

	//�����̃}�l�[�W���[���ɔ����𐶐�
	ExplosionManager::GetInstance()->InitExplosion(
		m_position,
		BURST_EFFECT_SCALE_RATE,
		enPlayer_Spread_Bomb
	);
}

void BulletPlayerSpread::EffectUpdateSub()
{

}

void BulletPlayerSpread::UpdateSub()
{

}