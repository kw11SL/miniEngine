#include "stdafx.h"
#include "EnemyManager.h"

//�I�u�W�F�N�g�̏�����
EnemyManager* EnemyManager::m_enemyManager = nullptr;


void EnemyManager::InitEnemies(
	const Vector3& initPoint,
	const Vector3& initUp,
	const EnEnemyType enemyType
)
{
	//�z����ɃG�l�~�[���쐬
	m_enemies.push_back(NewGO<Enemy>(0, "enemy"));

	//�쐬���ɏ�����
	m_enemies[m_enemies.size() - 1]->Init(
		initPoint,
		initUp,
		enemyType
	);
}

void EnemyManager::ExecuteUpdate()
{
	//���݃t���O�𒲂ׂāA�I�t��������j��
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