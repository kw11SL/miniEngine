#include "stdafx.h"
#include "EnemyGenerator.h"
#include<random>

namespace {
	//�G�t�F�N�g�֘A
	const char16_t* ENEMY_SPAWN_EFFECT_FILEPATH = u"Assets/effect/enemySpawn.efk";		//�X�|�[���G�t�F�N�g�̃t�@�C���p�X
	const Vector3 ENEMY_SPAWN_EFFECT_SCALE = { 20.0f,20.0f,20.0f };						//�X�|�[���G�t�F�N�g�̊g�嗦

	//�J�E���^�[�֘A
	const float BORDER_TIMEUP = 10.0f;						//�X�|�[�������𑁂����鎞�Ԃ̃{�[�_�[
	//�ʏ펞
	const float GENERATE_TIME = 2.0f;						//�X�|�[�����J�n����J�E���^
	const float ENEMY_SPAWN_TIME = 2.0f;					//�G�l�~�[�̃X�|�[������
	const float EFFECT_PLAY_TIME = 1.4f;					//�X�|�[�����̃G�t�F�N�g���Đ��������
	
	//�^�C���A�b�v�O
	const float GENERATE_TIME_NEAR_TIMEUP = 1.0f;			//�^�C���A�b�v�O�̃X�|�[���J�n�J�E���^
	const float ENEMY_SPAWN_TIME_NEAR_TIMEUP = 1.0f;		//�^�C���A�b�v�O�̃G�l�~�[�̃X�|�[������
	const float EFFECT_PLAY_TIME_NEAR_TIMEUP = 0.4f;		//�^�C���A�b�v�O�̃X�|�[�����̃G�t�F�N�g���Đ��������

	//������̃A�N�e�B�x�[�g�܂ł̎���
	const float GENERATOR_ACTIVE_COUNT_SHOT = 45.0f;		//�ˌ��^�G�l�~�[��������A�N�e�B�u�ɂ��鎞��
	const float GENERATOR_ACTIVE_COUNT_BOMB = 30.0f;		//�����^�G�l�~�[��������A�N�e�B�u�ɂ��鎞��
}

EnemyGenerator::~EnemyGenerator()
{
	
}

void EnemyGenerator::Init(const Vector3& pos, const Quaternion& rot, const bool isActive ,const EnEnemyType& enemyType)
{
	//�G�l�~�[�Ǘ��N���X�̃|�C���^���擾
	m_enemyManager = EnemyManager::GetInstance();

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

void EnemyGenerator::SpawnEnemy()
{
	//�Ǘ��N���X���̉ϒ��z��̒���NewGO���A������
	m_enemyManager->InitEnemies(
		m_position,
		m_up,
		m_spawnEnemyType
	);
}

void EnemyGenerator::PlaySpawnEffect()
{
	//�c�莞�Ԃ�0�ȉ��������琶�����Ȃ�
	if (GameDirector::GetInstance()->GetTime() <= 0.0f) {
		return;
	}

	//�A�N�e�B�u�t���O���I���Ȃ甭��
	if (m_isActive == true) {
		m_spawnEffect.SetPosition(m_position);
		m_spawnEffect.SetRotation(m_rotation);
		m_spawnEffect.SetScale(ENEMY_SPAWN_EFFECT_SCALE);

		m_spawnEffect.Play(false);
	}
}

void EnemyGenerator::Generate()
{

	//�c�莞�Ԃ�0�ȉ��������琶�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame || 
		GameDirector::GetInstance()->GetTime() <= 0.0f) {
		return;
	}

	//������𒴂��Ă����珈�����Ȃ�
	if (GameDirector::GetInstance()->GetEnemyCount() > GameDirector::GetInstance()->GetMaxEnemyNum()){
		return;
	}

	//�����𐶐�
	std::random_device rnd;
	std::mt19937 mt(rnd());
	//�w�肵���l�͈̔͂Ń����_����float�l��Ԃ�
	std::uniform_real_distribution<float> randFloat(0.0f, 0.5f);

	//�����Ԋu
	float interval = 0.0f;

	//�����̒l���X�|�[�������ɉ�����
	//�^�C���A�b�v�������Ă����甭���Ԋu������������
	if (GameDirector::GetInstance()->GetTime() <= BORDER_TIMEUP) {
		interval = GENERATE_TIME_NEAR_TIMEUP;
		interval += randFloat(mt);
	}
	else {
		interval = GENERATE_TIME;
		interval += randFloat(mt);
	}

	//���������t���O���I���B�A�N�e�B�u�t���O���I���ɂȂ�܂Ő����������Ȃ��B
	if (m_generateCounter > interval && m_isActive == true) {

		//�X�|�[���������t���O���I��(=�X�|�[�������̊J�n)
		m_isSpawning = true;

		//�G�t�F�N�g���Đ��\�ɂ���
		if (m_isValidPlayEffect == false) {
			m_isValidPlayEffect = true;
		}
		//�G�l�~�[���X�|�[���\�ɂ���
		if (m_isValidSpawnEnemy == false) {
			m_isValidSpawnEnemy = true;
		}

		//���������J�E���^�[�����Z�b�g
		m_generateCounter = 0.0f;

	}
}

void EnemyGenerator::SpawnEnemyWithEffect()
{
	float effectPlayTime = 0.0f;
	float enemySpawnTime = 0.0f;

	//�^�C���A�b�v���O�Ȃ�^�C���A�b�v�O�̔����Ԋu(�Z��)�ɂ���
	if (GameDirector::GetInstance()->GetTime() <= BORDER_TIMEUP) {
		effectPlayTime = EFFECT_PLAY_TIME_NEAR_TIMEUP;
		enemySpawnTime = ENEMY_SPAWN_TIME_NEAR_TIMEUP;
	}
	//�����łȂ���Βʏ펞�̔����Ԋu��
	else {
		effectPlayTime = EFFECT_PLAY_TIME;
		enemySpawnTime = ENEMY_SPAWN_TIME;
	}


	//�G�t�F�N�g�Đ��J�E���^�[�����ɒB���Ă��ăG�t�F�N�g�Đ��\�Ȃ�
	if (m_spawnEffectCounter >= effectPlayTime 
		&& m_isValidPlayEffect == true) {
		
		//�G�t�F�N�g���Đ�
		PlaySpawnEffect();
		//�J�E���^�[�����Z�b�g
		m_spawnEffectCounter = 0.0f;
		//���̃X�|�[���������͍Đ����Ȃ�
		m_isValidPlayEffect = false;
	}

	//�G�l�~�[���X�|�[������J�E���^�[�����ɒB���Ă��Đ����\�Ȃ�
	if (m_spawnCounter >=  enemySpawnTime
		&& m_isValidSpawnEnemy == true) {
		
		//�G�l�~�[���X�|�[��
		SpawnEnemy();
		//�G�l�~�[���X�|�[�����I�����̂ŃJ�E���^�[�����Z�b�g
		m_spawnCounter = 0.0f;
		//���̃X�|�[���������͐������Ȃ�
		m_isValidSpawnEnemy = false;
		//�X�|�[���������I������̂ŏ������t���O���I�t
		m_isSpawning = false;
	}
}

void EnemyGenerator::UpdateEffect()
{
	//�X�|�[�����̃G�t�F�N�g���X�V
	m_spawnEffect.Update();
}

void EnemyGenerator::Activate()
{
	//�ˌ��^�G�l�~�[������̂Ƃ�
	//��莞�Ԉȉ��ŃA�N�e�B�u�t���O���I��
	if (m_spawnEnemyType == enShot &&
		GameDirector::GetInstance()->GetTime() <= GENERATOR_ACTIVE_COUNT_SHOT) {
		SetActive(true);
	}

	//�����킪�{���̂Ƃ�
	//��莞�Ԉȉ��ŃA�N�e�B�u�t���O���I��
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

void EnemyGenerator::AddCounter()
{
	//�X�|�[���������łȂ���Ίe�J�E���^�[��0�ɂ���
	if (m_isSpawning != true) {
		m_spawnCounter = 0.0f;
		m_spawnEffectCounter = 0.0f;
	}
	//�X�|�[���������͊e�J�E���^�[���㏸������
	else{
		AddSpawnCounter();
		AddSpawnEffectPlayCounter();
	}
}

void EnemyGenerator::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//������̃A�N�e�B�x�[�g����
	Activate();
	//�ړ�����
	Move();
	//��]����
	Rotation();
	//���������J�E���^�[�𑝉�
	AddGenerateCounter();
	//���������J�E���^�[�̐��l�����ɒB���Ă�����X�|�[���������J�n
	Generate();
	//�X�|�[���������A�G�t�F�N�g�����A�G�l�~�[�̃X�|�[���J�E���^�[�𑝉�������
	AddCounter();
	//�G�t�F�N�g�Đ��A�G�l�~�[�����̊e�J�E���^�[�����ɒB������A�G�t�F�N�g�Đ��A�G�l�~�[���X�|�[��
	SpawnEnemyWithEffect();

	//�G�t�F�N�g���X�V
	UpdateEffect();
}