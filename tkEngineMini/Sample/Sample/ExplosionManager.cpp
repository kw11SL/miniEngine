#include "stdafx.h"
#include "ExplosionManager.h"
#include "Explosion.h"

ExplosionManager* ExplosionManager::m_explosionManager = nullptr;


void ExplosionManager::InitExplosion(
	const Vector3& pos,
	const float scaleRate,
	const EnExplosionType& explosionType
)
{
	//�����̃^�C�v���Ƃɖ�����U�蕪��
	m_explosions.push_back(NewGO<Explosion>(0, Naming(explosionType)));

	//��Ŕz��ɒǉ��������̂�������
	m_explosions[m_explosions.size() - 1]->Init(
		pos,
		scaleRate,
		explosionType
	);
}

void ExplosionManager::DeleteExplosions()
{
	//�z����̔�����S��DeleteGO����
	for (auto& explosion : m_explosions) {
		DeleteGO(explosion);
	}

	//�z����̗v�f��S�ď���
	m_explosions.erase(m_explosions.begin(), m_explosions.end());
}

void ExplosionManager::ExecuteUpdate()
{
	//���݃t���O�𒲂ׂāA�I�t��������j��
	for (auto& explosion : m_explosions) {
		if (explosion->GetIsExist() == false) {
			DeleteGO(explosion);
		}
	}

	//�e���������߂̏������L�q�����֐��I�u�W�F�N�g
	auto func = [&](Explosion* explosion)->bool {
		//���݃t���O��false��������true��Ԃ�(=�폜�Ώۂɂ���)
		if (explosion->GetIsExist() == false) {
			return true;
		}
		//����ȊO��false
		return false;
	};

	//erase��remove_if��g�ݍ��킹
	//remove_if�Ŕz����̐擪����I�[�܂ł𒲍����A�֐��I�u�W�F�N�g��true��Ԃ��Ă����v�f(=�e�̑��݃t���O��false�A�܂�폜�Ώ�)�𖖔��ֈړ������Ă����B
	//remove_if�̖߂�l�͖����Ɉړ��������폜�Ώۂ����̐擪�C�e���[�^�Ȃ̂ł�������I�[�܂ł�erase���邱�ƂŔz�񂩂�폜�����
	m_explosions.erase(
		std::remove_if(m_explosions.begin(), m_explosions.end(), func),
		m_explosions.end()
	);
}

const char* ExplosionManager::Naming(const EnExplosionType& type)
{
	//�v���C���[�̎��@�e�̔���
	if (type == enPlayer_Spread_Bomb) {
		return EXPLOSION_PLAYER_NAME;
	}
	//�����^�G�l�~�[�̔���
	else if (type == enEnemy_Explosion) {
		return EXPLOSION_ENEMY_NAME;
	}
}