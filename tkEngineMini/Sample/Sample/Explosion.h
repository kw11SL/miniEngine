#pragma once
#include "ExplosionBase.h"
#include "SpreadBombExplosion.h"

/// @brief �����N���X�̊��N���X���g���N���X

/// @brief �����̃^�C�v
enum EnExplosionType {
	enPlayer_Spread_Bomb,
	enExplosion_Type_Num
};

class Explosion : public IGameObject
{
public:
	Explosion() {}
	~Explosion() {}

	bool Start();
	void Update();

	/// @brief ����������
	/// @param pos �������W
	/// @param scaleRate �g�嗦
	void Init(
		const Vector3& pos ,
		const float scaleRate,
		const EnExplosionType& explosionType
	);

private:
	ExplosionBase* m_explosionBase = nullptr;		//���N���X�̃|�C���^�ϐ�

	//���N���X�ɓn���h���N���X�̃I�u�W�F�N�g
	SpreadBombExplosion m_sBomb;

};

