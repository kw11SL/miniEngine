#include "stdafx.h"
#include "BulletManager.h"
#include "Bullet.h"

//�I�u�W�F�N�g�̏�����
BulletManager* BulletManager::m_bulletManager = nullptr;

void BulletManager::InitBullets(
	const Vector3& initPoint,
	const Vector3& initUp,
	const Vector3& direction,
	const EnBulletType bulletType
)
{
	m_bullets.push_back(NewGO<Bullet>(0, "bullet"));

	m_bullets[m_bullets.size() - 1]->Init(
		*RenderingEngine::GetInstance(),
		initPoint,
		initUp,
		direction,
		bulletType
	);
}

void BulletManager::DeleteBullets()
{
	//�z����̒e��S��DeleteGO����
	for (auto& bullet : m_bullets) {
		DeleteGO(bullet);
	}

	//�z����̗v�f��S�ď���
	m_bullets.erase(m_bullets.begin(), m_bullets.end());
}

void BulletManager::ExecuteUpdate()
{
	//���݃t���O�𒲂ׂāA�I�t��������j��
	for (auto& bullet : m_bullets) {
		if (bullet->GetIsExist() == false) {
			//bullet->DeleteSkinModel();
			DeleteGO(bullet);
		}
	}

	//�e���������߂̏������L�q�����֐��I�u�W�F�N�g
	auto func = [&](Bullet* bullet)->bool {
		//���݃t���O��false��������true��Ԃ�(=�폜�Ώۂɂ���)
		if (bullet->GetIsExist() == false) {
			return true;
		}
		//����ȊO��false
		return false;
	};

	//erase��remove_if��g�ݍ��킹
	//remove_if�Ŕz����̐擪����I�[�܂ł𒲍����A�֐��I�u�W�F�N�g��true��Ԃ��Ă����v�f(=�e�̑��݃t���O��false�A�܂�폜�Ώ�)�𖖔��ֈړ������Ă����B
	//remove_if�̖߂�l�͖����Ɉړ��������폜�Ώۂ����̐擪�C�e���[�^�Ȃ̂ł�������I�[�܂ł�erase���邱�ƂŔz�񂩂�폜�����
	m_bullets.erase(
		std::remove_if(m_bullets.begin(),m_bullets.end(), func),
		m_bullets.end()
	);
	
}