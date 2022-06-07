#include "stdafx.h"
#include "EnemyBase.h"
#include "Player_new.h"

namespace {
	float UPPER_OFFSET = 50.0f;				//����������
	const float ACTIVATE_COUNT = 0.7f;		//�����蔻�肪�L���ɂȂ�܂ł̃J�E���^�[
}

EnemyBase::EnemyBase()
{
}

void EnemyBase::Init(const Vector3& initPoint, const Vector3& initUp)
{	
	///////////////////////////////////////////////////////////////////
	// ���ʏ���
	//���f����NewGO
	m_skinModelRender = NewGO<SkinModelRender>(0);

	//�o���b�g�}�l�[�W���̃|�C���^���擾
	m_bulletManager = BulletManager::GetInstance();

	m_position = initPoint;
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	//���C�g������,�󂯎��
	m_directionLight = FindGO<DirectionLight>(DIRECTION_LIGHT_NAME);
	m_pointLight = FindGO<PointLight>(POINT_LIGHT_NAME);
	m_spotLight = FindGO<SpotLight>(SPOT_LIGHT_NAME);
	if (m_directionLight != nullptr) { RecieveDirectionLight(m_directionLight); }
	if (m_pointLight != nullptr) { RecievePointLight(m_pointLight); }
	if (m_pointLight != nullptr) { RecieveSpotLight(m_spotLight); }

	//�������ݒ�
	m_up = initUp;
	//����L�����R���̏�����
	m_myCharaCon.Init(
		m_position
	);
	//�������x�N�g���͏�����̔���
	m_downVector = m_up * -1.0f;
	//�������x�N�g���𐳋K��
	m_downVector.Normalize();
	//�L�����R���̏���
	m_myCharaCon.Execute(m_moveSpeed, m_downVector, UPPER_OFFSET);

	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	m_sphericalMove.Init(m_forward, m_right, m_up);

	//�����t���O���I��
	m_isExist = true;
	//���G��ԃt���O���I�t
	m_isInvincible = false;

	//�o�����瓖���蔻�肪�L���ɂȂ�܂ł̎��Ԃ��Z�b�g
	m_toActivateCounter = ACTIVATE_COUNT;

	///////////////////////////////////////////////////////////////////
	//�p����Ō��߂��G�t�F�N�g����������
	InitEffect();
	//�p����Ō��߂�����������(���f���̏�������ŗL�p�����[�^�ݒ�Ȃ�)
	InitSub();
	
}

void EnemyBase::InitSub()
{
}

void EnemyBase::Move()
{
	//�v���C���[������
	if (m_player == nullptr) {
		m_player = FindGO<Player_new>(PLAYER_NAME);
	}

	//�p����Ō��߂�����
	MoveSub();

	//�L�����R���ɂ����W�X�V
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector, UPPER_OFFSET);
	//����������ʂ̖@���ōX�V���A�E�ƑO�����X�V
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//���f���̍��W���X�V
	m_skinModelRender->SetPosition(m_position);
}

void EnemyBase::MoveSub()
{
}

void EnemyBase::Rotation()
{
	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
	//�p����Ō��߂�����
	RotationSub();
}

void EnemyBase::RotationSub()
{
}

void EnemyBase::Hit()
{
	//���@�e������
	QueryGOs<Bullet>("bullet", [&](Bullet* bullet) {
		Vector3 diff = bullet->GetPosition() - m_position;
		float length = diff.Length();

		//���������l�ȉ��̂Ƃ�
		if (length < m_hitRange) {

			//���G��ԂłȂ����
			if (m_isInvincible == false) {
				//�ϋv�͂����炷
				m_life -= bullet->GetPower();

				//�q�b�g�G�t�F�N�g�̍Đ�
				m_hitEffect.SetPosition(m_position + m_up * 50.0f);
				m_hitEffect.SetRotation(m_rot);
				m_hitEffect.SetScale({ 10.0f,10.0f,10.0f });
				m_hitEffect.Play(false);

				//�G�l�~�[�ɖ��G���Ԃ�ݒ�
				SetInvincibleTime(bullet->GetDamageInterval());
				m_isInvincible = true;
			}

			//�e�̑ϋv�l�����炷
			bullet->DecLife(m_durability);

			//�₢���킹�I��
			return false;
		}

		//�₢���킹���s
		return true;
	});

	//����������
	QueryGOs<Explosion>("explosion", [&](Explosion* explosion) {
		Vector3 diff = explosion->GetPosition() - m_position;
		float length = diff.Length();

		//�����͈͓̔��̂Ƃ�
		if (length < explosion->GetDamageArea()) {

			//���G��ԂłȂ����
			if (m_isInvincible == false) {
				//�ϋv�͂����炷
				m_life -= explosion->GetPower();
				//�G�l�~�[�ɖ��G���Ԃ�ݒ�
				SetInvincibleTime(explosion->GetDamageInterval());
				m_isInvincible = true;
			}

			//�₢���킹�I��
			return false;
		}

		//�₢���킹���s
		return true;
	});
}

void EnemyBase::DecInvTime()
{
	//���G���Ԃ�����
	m_invTime -= g_gameTime->GetFrameDeltaTime();

	//0�ȉ��Ȃ疳�G��Ԃ��I�t
	if (m_invTime <= 0.0f) {
		m_invTime = 0.0f;
		m_isInvincible = false;
	}
}

void EnemyBase::DecToActivateTime()
{
	m_toActivateCounter -= g_gameTime->GetFrameDeltaTime();

	//���@�Ƃ̐ڐG�����L���ɂ���
	if (m_toActivateCounter <= 0.0f) {
		m_toActivateCounter = 0.0f;
		m_isActive = true;
	}
}

void EnemyBase::DecLifeTime()
{
	m_lifeTime -= g_gameTime->GetFrameDeltaTime();

	//���Ԏ���������,������0�ȉ��������玩�ŏ���
	if (m_lifeTime <= 0.0f) {
		m_lifeTime = 0.0f;
		SelfDestroy();
	}
}

void EnemyBase::Destroy()
{
	//���C�t��0�ȉ�
	if (m_life <= 0.0f) {
		m_life = 0.0f;

		//�����t���O���I�t
		m_isExist = false;
	}

	//�����t���O���I�t�Ȃ�G�t�F�N�g���o���č폜����
	if (m_isExist == false) {
		m_destroyEffect.SetPosition(m_position);
		m_destroyEffect.SetRotation(m_rot);
		m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });
		m_destroyEffect.Play(false);

		//���jse�̍Đ�
		CSoundSource* ssDestroy = NewGO<CSoundSource>(0);
		ssDestroy->Init(L"Assets/wav/destroySe.wav");
		ssDestroy->SetVolume(0.6f);
		ssDestroy->Play(false);

		//DeleteGO(this);

		//�_�������_
		GameDirector::GetInstance()->AddScore(m_score);
		//�v���C���[�̌��j������+1
		GameDirector::GetInstance()->AddDestroyedEnemyCount();
	}
}

void EnemyBase::SelfDestroy()
{
	//�p����Ō��߂�����
	SelfDestroySub();

	//DeleteGO(this);
	m_isExist = false;
}

void EnemyBase::SelfDestroySub()
{
}

void EnemyBase::DestroyTimeUp()
{
	//�^�C���A�b�v�������玩��
	if (GameDirector::GetInstance()->GetTime() <= 0.0f) {
		SelfDestroy();
	}
}

void EnemyBase::FireBulletEqually(const int wayNum, const EnBulletType bulletType)
{
	//����0����������s���Ȃ�
	if (wayNum == 0) {
		return;
	}

	//�v���C���[�������i�[����x�N�g�����쐬
	Vector3 toPlayer = Vector3::Zero;
	
	
	if (m_player == nullptr) {
		m_player = FindGO<Player_new>(PLAYER_NAME);
	}
	//�v���C���[�����ւ̃x�N�g�����v�Z�A���K��
	if (m_player != nullptr) {
		toPlayer = m_player->GetPosition() - m_position;
		toPlayer.Normalize();
	}

	Vector3 dir = toPlayer;
	//�p�x�����߂邽�߂̉�]�N�H�[�^�j�I�����쐬
	Quaternion rot;

	//�w�肵���������Œe�ƒe�̊Ԃ̊p�x���v�Z(���W�A��)
	//��*2(=2�� = 1��)�𓙕����Ă���
	float angle = Math::PI * 2.0f / (float)wayNum;

	//for���ɑ������ޗp�̊p�x
	float angleTmp = 0.0f;

	//���ˏ���
	//�e�̊p�x����,�ˌ�
	for (int i = 0; i < wayNum; i++) {
		//��e��1����
		if (i == 0 && wayNum % 2 == 1) {
			//�v���C���[������1����
			rot.SetRotation(m_up, 0.0f);
			rot.Apply(dir);
		}
		//�����e��1����(�v���C���[�����ɂ͏o���Ȃ�)
		else if (i == 0 && wayNum % 2 == 0) {
			//�v���C���[���������Ȃ̂�1���ڂ��o������́A
			//(���������p�x / 2)���W�A����]�����������ƂȂ�
			angleTmp += angle / 2.0f;
			rot.SetRotation(m_up, angleTmp);
			rot.Apply(dir);
		}
		//2���ڈȍ~�͓��������p�x�����˕�������
		else {
			angleTmp += angle;
			rot.SetRotation(m_up, angleTmp);
			rot.Apply(dir);
		}

		//�e�𐶐�
		m_bulletManager->InitBullets(
			m_position,
			m_up,
			dir,
			bulletType
		);

		//������A�������v���C���[�����ɖ߂�(���ʂ���ɕ��������肵�Ă��邽��)
		dir = toPlayer;
		dir.Normalize();
	}

	//���˃J�E���^�[��߂�
	m_shotCounter = 0.0f;
}

void EnemyBase::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}
	
	//���ʏ���
	///////////////////////////////////////////////////
	Move();
	Rotation();
	Hit();
	DecInvTime();
	DecToActivateTime();
	Destroy();
	DestroyTimeUp();
	///////////////////////////////////////////////////

	//�p����Ō��߂�����
	UpdateSub();
	//�p����Ō��߂��G�t�F�N�g�̍X�V����
	UpdateEffect();

	//���f���̉�]���X�V
	m_skinModelRender->SetRotation(m_rot);
}

void EnemyBase::UpdateSub()
{

}