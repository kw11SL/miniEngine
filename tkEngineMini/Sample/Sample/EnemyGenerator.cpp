#include "stdafx.h"
#include "EnemyGenerator.h"
#include<random>

namespace {
	const char16_t* ENEMY_SPAWN_EFFECT_FILEPATH = u"Assets/effect/enemySpawn.efk";		//�G�l�~�[���X�|�[������Ƃ��̃G�t�F�N�g�̃t�@�C���p�X
	const Vector3 ENEMY_SPAWN_EFFECT_SCALE = { 20.0f,20.0f,20.0f };

	const float BORDER_TIMEUP = 10.0f;						//�X�|�[�������𑁂����鎞�Ԃ̃{�[�_�[
	const float ENEMY_SPAWN_TIME = 2.0f;					//�G�l�~�[�̃X�|�[������
	const float EFFECT_PLAY_TIME = 1.4f;					//�X�|�[�����̃G�t�F�N�g���Đ��������
	const float ENEMY_SPAWN_TIME_NEAR_TIMEUP = 1.5f;		//�^�C���A�b�v�O�̃G�l�~�[�̃X�|�[������
	const float GENERATOR_ACTIVE_COUNT_SHOT = 45.0f;		//�ˌ��^�G�l�~�[��������A�N�e�B�u�ɂ��鎞��
	const float GENERATOR_ACTIVE_COUNT_BOMB = 30.0f;		//�����^�G�l�~�[��������A�N�e�B�u�ɂ��鎞��
}

EnemyGenerator::~EnemyGenerator()
{
	DeleteGO(m_enemy);
}

void EnemyGenerator::Init(const Vector3& pos, const Quaternion& rot, const bool isActive ,const EnEnemyType& enemyType)
{
	m_position = pos;
	m_rotation = rot;
	m_spawnEnemyType = enemyType;

	//���������ɃA�N�e�B�u�ɂ��邩�ǂ�����I��
	SetActive(isActive);

	//�L�����R����������
	m_myCharaCon.Init(m_position);
	m_sphericalMove.Init(m_forward, m_right, m_up);

	//�G�t�F�N�g��������
	m_spawnEffect.Init(ENEMY_SPAWN_EFFECT_FILEPATH);
}

void EnemyGenerator::PlaySpawnEffect()
{
	//�c�莞�Ԃ�0�ȉ��������琶�����Ȃ�
	if (GameDirector::GetInstance()->GetTime() <= 0.0f) {
		return;
	}

	//�A�N�e�B�u�t���O���I���Ȃ甭��
	if (m_spawnEffectCounter > EFFECT_PLAY_TIME && m_isActive == true) {
		m_spawnEffect.SetPosition(m_position);
		m_spawnEffect.SetRotation(m_rotation);
		m_spawnEffect.SetScale(ENEMY_SPAWN_EFFECT_SCALE);

		m_spawnEffect.Play(false);

		//�J�E���^�[�����Z�b�g
		m_spawnEffectCounter = 0.0f;
	}
}

void EnemyGenerator::GenerateEnemy(const EnEnemyType& enemyType)
{
	//�c�莞�Ԃ�0�ȉ��������琶�����Ȃ�
	if (GameDirector::GetInstance()->GetTime() <= 0.0f) {
		return;
	}

	//�G�l�~�[�̍ő吔�𒴂��Ă��Ȃ�������X�|�[��
	if (GameDirector::GetInstance()->GetEnemyCount() <= GameDirector::GetInstance()->GetMaxEnemyNum()) {

		//�����𐶐�
		std::random_device rnd;
		std::mt19937 mt(rnd());
		//�w�肵���l�͈̔͂Ń����_����float�l��Ԃ�
		std::uniform_real_distribution<float> randFloat(0.0f, 0.5f);

		float interval = 0.0f;

		//�����̒l���X�|�[�������ɉ�����
		if (GameDirector::GetInstance()->GetTime() <= BORDER_TIMEUP) {
			interval = ENEMY_SPAWN_TIME_NEAR_TIMEUP;
			interval += randFloat(mt);
		}
		else {
			interval = ENEMY_SPAWN_TIME;
			interval += randFloat(mt);
		}

		//�X�|�[���G�t�F�N�g���Đ�
		PlaySpawnEffect();

		//�G�l�~�[�𐶐��B�A�N�e�B�u�ɂȂ�܂ł͐������Ȃ��B
		if (m_spawnCounter > interval && m_isActive == true) {
			m_enemy = NewGO<Enemy>(0, "enemy");
			
			m_enemy->Init(
				*RenderingEngine::GetInstance(),
				m_position,
				m_up,
				enemyType);

			//�J�E���^�[��0�Ƀ��Z�b�g
			m_spawnCounter = 0.0f;
			//�G�t�F�N�g�Đ��p�J�E���^�[��0�Ƀ��Z�b�g
			m_spawnEffectCounter = 0.0f;
		}
	}
	else{
		m_spawnCounter = 0.0f;
		m_spawnEffectCounter = 0.0f;
	}

}

void EnemyGenerator::UpdateEffect()
{
	m_spawnEffect.Update();
}

void EnemyGenerator::Activate()
{
	//�ˌ��^�G�l�~�[������̂Ƃ�
	if (m_spawnEnemyType == enShot &&
		GameDirector::GetInstance()->GetTime() <= GENERATOR_ACTIVE_COUNT_SHOT) {
		SetActive(true);
	}

	//�����킪�{���̂Ƃ�
	if (m_spawnEnemyType == enBomb &&
		GameDirector::GetInstance()->GetTime() <= GENERATOR_ACTIVE_COUNT_BOMB) {
		SetActive(true);
	}
}

void EnemyGenerator::Move()
{
	m_myCharaCon.Execute(m_moveSpeed, m_downVector, 0.0f);
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);
}

void EnemyGenerator::Rotation()
{
	m_sphericalMove.Rotation(m_forward,m_right,m_up,m_rotation);
}

void EnemyGenerator::Update()
{
	//������̃A�N�e�B�x�[�g����
	Activate();

	Move();
	Rotation();

	AddCounter();
	AddSpawnEffectPlayCounter();

	GenerateEnemy(m_spawnEnemyType);
	//PlaySpawnEffect();

	//�G�t�F�N�g���X�V
	UpdateEffect();
}