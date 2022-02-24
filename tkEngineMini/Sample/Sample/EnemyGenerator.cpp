#include "stdafx.h"
#include "EnemyGenerator.h"
#include<random>

namespace {
	const char16_t* ENEMY_SPAWN_EFFECT_FILEPATH = u"Assets/effect/enemySpawn.efk";		//�G�l�~�[���X�|�[������Ƃ��̃G�t�F�N�g�̃t�@�C���p�X
	const Vector3 ENEMY_SPAWN_EFFECT_SCALE = { 20.0f,20.0f,20.0f };

	const float BORDER_TIMEUP = 10.0f;						//�X�|�[�������𑁂����鎞�Ԃ̃{�[�_�[
	const float ENEMY_SPAWN_TIME = 2.0f;					//�G�l�~�[�̃X�|�[������
	const float EFFECT_PLAY_TIME = 1.4f;					//�X�|�[�����̃G�t�F�N�g���Đ��������
	const float ENEMY_SPAWN_TIME_NEAR_TIMEUP = 1.0f;		//�^�C���A�b�v�O�̃G�l�~�[�̃X�|�[������
	const float GENERATOR_ACTIVE_COUNT_SHOT = 45.0f;		//�ˌ��^�G�l�~�[��������A�N�e�B�u�ɂ��鎞��
	const float GENERATOR_ACTIVE_COUNT_BOMB = 30.0f;		//�����^�G�l�~�[��������A�N�e�B�u�ɂ��鎞��
}

EnemyGenerator::~EnemyGenerator()
{
	//�z����̑S�G�l�~�[��DeleteGO
	for (auto& enemy : m_enemies) {
		DeleteGO(enemy);
	}

	//�z����̗v�f���ŏ�����Ō�܂őS�ď���
	m_enemies.erase(m_enemies.begin(), m_enemies.end());
}

void EnemyGenerator::Init(const Vector3& pos, const Quaternion& rot, const bool isActive ,const EnEnemyType& enemyType)
{
	////�G�l�~�[�̊Ǘ��N���X�̃|�C���^���擾
	//m_enemyManager = EnemyManager::GetInstance();

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
	if (GameDirector::GetInstance()->GetGameState() != enGame || 
		GameDirector::GetInstance()->GetTime() <= 0.0f) {
		
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
		//�^�C���A�b�v�������Ă����甭���Ԋu������������
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
			
			//���g�̉ϒ��z��̒���NewGO����
			m_enemies.push_back(NewGO<Enemy>(0, "enemy"));
			
			//����������͉̂ϒ��z��̃T�C�Y - 1�̗v�f
			m_enemies[m_enemies.size() - 1]->Init(
				m_position,
				m_up,
				enemyType
			);

			//�J�E���^�[��0�Ƀ��Z�b�g
			m_spawnCounter = 0.0f;
			//�G�t�F�N�g�Đ��p�J�E���^�[��0�Ƀ��Z�b�g
			m_spawnEffectCounter = 0.0f;
		}
	}
	else{
		m_spawnCounter = 0.0f;
		//m_spawnEffectCounter = 0.0f;
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

void EnemyGenerator::DeleteEnemy()
{
	//�G�l�~�[�̑��݃t���O���I�t��������DeleteGO
	for (auto& enemy : m_enemies) {
		if (enemy->GetIsExist() == false) {
			DeleteGO(enemy);
		}
	}

	//�z�񂩂�G�l�~�[���������߂̏������L�q�����֐��I�u�W�F�N�g
	auto func = [&](Enemy* enemy)->bool {
		//���݃t���O��false��������true��Ԃ�(=�폜�Ώۂɂ���)
		if (enemy->GetIsExist() == false) {
			return true;
		}
		//����ȊO�̏ꍇ��false
		return false;
	};

	//erase��remove_if��g�ݍ��킹
	//remove_if�Ŕz����̐擪����I�[�܂ł𒲍����A�֐��I�u�W�F�N�g��true��Ԃ��Ă����v�f(=�e�̑��݃t���O��false�A�܂�폜�Ώ�)�𖖔��ֈړ������Ă����B
	//remove_if�̖߂�l�͖����Ɉړ��������폜�Ώۂ����̐擪�C�e���[�^�Ȃ̂ł�������I�[�܂ł�erase���邱�ƂŔz�񂩂�폜�����
	m_enemies.erase(
		std::remove_if(m_enemies.begin(), m_enemies.end(), func),
		m_enemies.end()
	);

}

void EnemyGenerator::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//������̃A�N�e�B�x�[�g����
	Activate();
	Move();
	Rotation();
	AddCounter();
	AddSpawnEffectPlayCounter();
	GenerateEnemy(m_spawnEnemyType);
	DeleteEnemy();
	//PlaySpawnEffect();

	//�G�t�F�N�g���X�V
	UpdateEffect();
}