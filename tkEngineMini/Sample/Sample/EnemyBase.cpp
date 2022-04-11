#include "stdafx.h"
#include "EnemyBase.h"

namespace {
	float UPPER_OFFSET = 50.0f;
	const float ACTIVATE_COUNT = 0.7f;		//�����蔻�肪�L���ɂȂ�܂ł̃J�E���^�[
}

EnemyBase::EnemyBase()
{
	//����������G�l�~�[����+1
	GameDirector::GetInstance()->AddEnemyCount();
	//�G�l�~�[������+1
	GameDirector::GetInstance()->AddEnemyTotalCount();
}

void EnemyBase::Init(const Vector3& initPoint, const Vector3& initUp)
{
	//���f����NewGO
	m_skinModelRender = NewGO<SkinModelRender>(0);

	const char* modelPath = "hoge";

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

	//�p����Ō��߂�����
	InitSub();
	
	//�p����Ō��߂��G�t�F�N�g����������
	InitEffect();
}

void EnemyBase::Move()
{
	//�p����Ō��߂�����
	MoveSub();

	//�L�����R���ɂ����W�X�V
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector, UPPER_OFFSET);
	//����������ʂ̖@���ōX�V���A�E�ƑO�����X�V
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//���f���̍��W���X�V
	m_skinModelRender->SetPosition(m_position);
}

void EnemyBase::Rotation()
{
	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
	//�p����Ō��߂�����
	RotationSub();
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

		DeleteGO(this);

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

	DeleteGO(this);
}

void EnemyBase::DestroyTimeUp()
{
	//�^�C���A�b�v�������玩��
	if (GameDirector::GetInstance()->GetTime() <= 0.0f) {
		SelfDestroy();
	}
}



void EnemyBase::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	Move();
	Rotation();
	Hit();
	DecInvTime();
	DecToActivateTime();
	Destroy();
	DestroyTimeUp();

	//�p����Ō��߂�����
	UpdateSub();
	//�p����Ō��߂��G�t�F�N�g�̍X�V����
	UpdateEffect();

	m_skinModelRender->SetRotation(m_rot);
}
