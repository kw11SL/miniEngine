#include "stdafx.h"
#include "Player_new.h"
#include "Enemy.h"


namespace{
	//const char* MODELPATH_UTC = "Assets/modelData/unityChan.tkm";
	//const char* MODELPATH_UTC = "Assets/modelData/player/player_object.tkm";
	const char* MODELPATH_UTC = "Assets/modelData/player/player_object_c.tkm";
	//const char* MODELPATH_UTC = "Assets/modelData/player_a.tkm";
	const char* SKELETON_PATH_UTC = "Assets/modelData/unityChan.tks";
	const float UTC_RADIUS = 40.0f;
	const float UTC_HEIHGT = 100.0f;
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	
	const Vector3 INIT_POINT = {0.0f,700.0f,0.0f};			//�����ʒu
	const float UPPER_OFFSET = 10.0f;						//�n�`���畂�����鋗��

	const float PL_MOVE_SPEED = -12.0f;			//�ړ����x
	const float FIRECOUNTER_NORMAL = 0.075f;	//�ʏ�e�̔��ˊԊu
	const float FIRECOUNTER_SPREAD = 0.8f;		//�X�v���b�h�{���̔��ˊԊu
	const int INIT_LIFE = 3;					//�����c�@
	const float INVINCIBLE_TIME_REVIVE = 7.0f;	//�������ɐݒ肳��閳�G����
	const float REVIVE_TIME = 3.0f;				//��e���畜���܂ł̎���

	
	const float CAMERA_ROTATE_FRACTION_ADD_RATE = 0.005f;			//�J�����̉�]�Ɏg����ԌW���ɉ��Z����萔
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MIN = 0.003f;		//�J�����̉�]�Ɏg����ԌW���ɉ��Z����萔
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MAX = 0.03f;		//�J�����̉�]�Ɏg����ԌW���ɉ��Z����萔
	const float CAMERA_MOVESPEED_MAX = 1000.0f;						//�J�����A�����_�̒Ǐ]�ō����x 

	//�G�t�F�N�g�֘A
	const char16_t* EFFECT_FILEPATH_START = u"Assets/effect/player_born.efk";		//�J�n�G�t�F�N�g�t�@�C���p�X
	const Vector3 EFFECT_SCALE_START = { 50.0f,50.0f,50.0f };

	const char16_t* EFFECT_FILEPATH_EXPLOSION = u"Assets/effect/justguard.efk";		//��e�G�t�F�N�g�t�@�C���p�X
	const Vector3 EFFECT_SCALE_EXPLOSION = { 30.0f,30.0f,30.0f };					//��e�G�t�F�N�g�̊g�嗦

	const char16_t* EFFECT_FILEPATH_REVIVE = u"Assets/effect/revive_a.efk";			//�����G�t�F�N�g�̃t�@�C���p�X
	const Vector3 EFFECT_SCALE_REVIVE = { 20.0f,20.0f,20.0f };						//�����G�t�F�N�g�̊g�嗦

	const char16_t* EFFECT_FILEPATH_TRACK = u"Assets/effect/moveTrack.efk";			//�O�ՃG�t�F�N�g�̃t�@�C���p�X
	const Vector3 EFFECT_SCALE_TRACK = { 10.0f,10.0f,10.0f };						//�O�ՃG�t�F�N�g�̊g�嗦

	const char16_t* EFFECT_FILEPATH_MARKER = u"Assets/effect/positionMarker.efk";	//�����蔻��G�t�F�N�g�̃t�@�C���p�X
	const Vector3 EFFECT_SCALE_MARKER = { 30.0f,30.0f,30.0f };						//�����蔻��G�t�F�N�g�̊g�嗦
	const float MARKER_PLAY_INTERVAL = 0.02f;										//�����蔻��G�t�F�N�g�̔����Ԋu
}

Player_new::Player_new()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
}

Player_new::~Player_new()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_missSe);
	DeleteGO(m_normalShotSe);
}

void Player_new::Init()
{
	//�G�t�F�N�g��������
	InitEffect();

	//SE��������
	m_missSe = NewGO<CSoundSource>(0);
	m_missSe->Init(L"Assets/wav/missSe.wav");

	m_normalShotSe = NewGO<CSoundSource>(0);
	m_normalShotSe->Init(L"Assets/wav/normalShotSe_1.wav");

	////���C�g������
	//m_directionLight = FindGO<DirectionLight>("directionlight");
	//m_pointLight = FindGO<PointLight>("pointlight");
	//m_spotLight = FindGO<SpotLight>("spotlight");

	//�e�̊Ǘ��N���X�̃|�C���^���擾
	m_bulletManager = BulletManager::GetInstance();
	
	//���f����������
	m_skinModelRender->Init(
		MODELPATH_UTC, 
		enModelUpAxisZ,
		true,
		false,
		SKELETON_PATH_UTC
	);

	m_position = INIT_POINT;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	//////////////////////////////
	//���@���C�t�̏�����
	m_life = INIT_LIFE;
	//���G�t���O�̏�����
	m_isInvincible = false;
	//�����t���O�̏�����
	m_isExist = true;
	//////////////////////////////

	//����L�����R���̏�����
	m_myCharaCon.Init(
		m_position
	);

	//�������͏�����̔���
	m_downVector = m_up * -1.0f;
	//�������x�N�g���𐳋K��
	m_downVector.Normalize();

	//�O���A�E�A��̊e�x�N�g�����e���ŏ�����
	m_sphericalMove.Init(m_forward, m_right, m_up);
	
	//�J���������_���王�_�ւ̃x�N�g����ݒ�
	Vector3 toCamera;
	toCamera.x = 0.0f;
	toCamera.y = 700.0f;
	toCamera.z = 1000.0f;
	/*toCamera.x = 0.0f;
	toCamera.y = 1600.0f;
	toCamera.z = 2400.0f;*/
	toCamera *= 1.5f;

	//�J�����̏�����
	m_gameCamera.Init(CAMERA_MOVESPEED_MAX);
	//�����_���v���C���[�̈ʒu�ɐݒ�
	m_gameCamera.SetTargetPosition(m_position);
	//�����_����̑��Έʒu�Ŏ��_��ݒ�
	m_gameCamera.SetCameraPosition(m_position + toCamera);

	//�����_�ڕW��ݒ�
	m_gameCamera.SetTargetPositionTarget(m_position);
	//�J�����ڕW��ݒ�
	m_gameCamera.SetCameraPositionTarget(m_position + toCamera);

	//�J�����̏���������g�̏�ɂ��Ă���
	m_cameraUp = m_up;

	//���˕�����O���ɂ��Ă���
	//m_shotDirection = m_forward;

	//���������ɊJ�n���o�p�G�t�F�N�g�𔭐�������
	m_startEffect.SetScale(EFFECT_SCALE_START);
	m_startEffect.SetPosition(m_position/* + m_up * 50.0f*/);
	m_startEffect.SetRotation(m_rot);
	m_startEffect.Play();
	
}

void Player_new::InitEffect()
{
	m_startEffect.Init(EFFECT_FILEPATH_START);
	m_explosionEffect.Init(EFFECT_FILEPATH_EXPLOSION);
	m_reviveEffect.Init(EFFECT_FILEPATH_REVIVE);
	m_moveTrackEffect.Init(EFFECT_FILEPATH_TRACK);
	m_markerEffect.Init(EFFECT_FILEPATH_MARKER);
}

bool Player_new::Start()
{

	return true;
}

void Player_new::Move()
{
	//m_markerCounter += g_gameTime->GetFrameDeltaTime();

	//�p�b�h�̃X�e�B�b�N����x������y�������󂯎��
	float x = g_pad[0]->GetLStickXF();
	float y = g_pad[0]->GetLStickYF();

	//�O���x�N�g�����쐬
	Vector3 forward;
	//��x�N�g���ƃJ�����̉E�x�N�g���̊O�ς�O���x�N�g���ɂ���
	forward.Cross(m_up,g_camera3D->GetRight());
	forward.Normalize();

	//�v���C���[�̍��E�����ւ̈ړ�
	m_moveSpeed = g_camera3D->GetRight() * -x * PL_MOVE_SPEED;
	//m_moveSpeed = m_right * x * PL_MOVE_SPEED;
	//�v���C���[�̑O��(���A��O)�����ւ̈ړ�
	m_moveSpeed += forward * y * PL_MOVE_SPEED;

	//�Q�[�����ȊO,���C�t��0�A���݃t���O���I�t�Ȃ�ړ����x��0��
	if (m_isExist == false 
		|| GameDirector::GetInstance()->GetPlayerLife() <= 0
		|| GameDirector::GetInstance()->GetGameState() != enGame) {
		
		m_moveSpeed *= 0.0f;
	}

	//�O�ՃG�t�F�N�g�̍Đ�
	if (m_moveSpeed.Length() > 5.0f) {
		m_moveTrackEffect.SetPosition(m_position + m_up*50.0f);
		m_moveTrackEffect.SetRotation(m_rot);
		m_moveTrackEffect.SetScale(EFFECT_SCALE_TRACK);
		m_moveTrackEffect.Play(false);

		/*if (m_markerCounter >= MARKER_PLAY_INTERVAL) {
			m_markerEffect.SetPosition(m_position + m_up * 50.0f);
			m_markerEffect.SetRotation(m_rot);
			m_markerEffect.SetScale(EFFECT_SCALE_MARKER);
			m_markerEffect.Play(true);

			m_markerCounter = 0.0f;
		}*/
	}

	//����L�����R���Ɉړ����x��n��
	m_position = m_myCharaCon.Execute(m_moveSpeed,m_downVector,m_upperOffset);

	//{
	//	// ��x�N�g�����X�V
	//	//�������x�N�g��(=���C���΂�����)* -1.0�@= �v���C���[�̏�x�N�g��
	//	Vector3 newUp = m_downVector * -1.0f;
	//	// ���݂̏�x�N�g������A�V������x�N�g���Ɍ����邽�߂̉�]�N�H�[�^�j�I�����v�Z
	//	//		���@�J�����̌v�Z�Ŏg���B
	//	m_rotUpToGroundNormal.SetRotation(m_up, newUp);

	//	//���g�̏�x�N�g�����X�V
	//	m_up = newUp;

	//	//�X�V������x�N�g���ƑO���x�N�g���̊O�ρ@=�@�E�x�N�g��
	//	//m_right = g_camera3D->GetRight();
	//	m_right.Cross(m_up, m_forward);
	//	//���߂��E�x�N�g���ƍX�V������x�N�g���̊O�ρ@=�@�O���x�N�g��
	//	m_forward.Cross(m_right, m_up);
	//}
	//����L�܂Ƃ�
	Vector3 oldUp = m_up;
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);
	m_rotUpToGroundNormal.SetRotation(oldUp, m_up);
	
	//���f���̍��W�X�V
	m_skinModelRender->SetPosition(m_position);
}

void Player_new::Rotation()
{
	//�O���A�E�A�ォ���]���v�Z����
	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);

	//�X�e�B�b�N�ɂ���]����
	float stickX, stickY = 0.0f;
	stickX = g_pad[0]->GetLStickXF();
	stickY = g_pad[0]->GetLStickYF();

	//���͂����������]����
	if (fabs(stickX) >= 0.001f || fabsf(stickY) >= 0.001f){
		m_angleAxisUp = atan2f(stickX, stickY);
		m_rotAxisUp.SetRotation(m_up, m_angleAxisUp);
	}

	//�N�H�[�^�j�I�����������A�O���x�N�g������]
	m_rot.Multiply(m_rotAxisUp);
	m_rotAxisUp.Apply(m_forward);

	//���f���ɉ�]��K�p����
	m_skinModelRender->SetRotation(m_rot);
}

void Player_new::RotateShotDirection()
{
	//�V���b�g�̕����̓v���C���[�̑O��
	//m_shotDirection = m_forward;
	//���˕����̃x�[�X
	m_shotDirection = g_camera3D->GetForward();

	//��]���͏�x�N�g��
	Vector3 axis = m_up;
	
	//������̉�]�N�H�[�^�j�I�����쐬
	Quaternion rot;

	//�E�X�e�B�b�N�̓��͂��󂯎��
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//���͒l����p�x�����߂�
	float angle = atan2f(x, y);

	//������̉�]�����߂�
	rot.SetRotation(axis, angle);

	//�x�N�g������]
	rot.Apply(m_shotDirection);
}

void Player_new::FireBullet()
{
	//�Q�[�����ȊO�A���C�t��0�A���݃t���O���I�t�Ȃ�return
	if (m_isExist == false 
		|| GameDirector::GetInstance()->GetPlayerLife() <= 0
		|| GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//R1�{�^���������Ɣ��ˁA�������ςȂ��ŘA��
	if (g_pad[0]->IsPress(enButtonRB1)) {
		
		//�E�X�e�B�b�N�����ɃV���b�g������
		//�J�E���^�[��0�̂Ƃ��ƃJ�E���^�[�����l�𒴂���Ɣ���
		if (m_fireCounter > FIRECOUNTER_NORMAL || m_fireCounter == 0.0f) {
			
			//�V���b�gSE�̍Đ�
			if (m_enBulletType == enNormalShot) {
				CSoundSource* ssNormalSe = NewGO<CSoundSource>(0);
				ssNormalSe->Init(L"Assets/wav/normalShotSe_1.wav");
				ssNormalSe->SetVolume(0.4f);
				ssNormalSe->Play(false);

				/*m_normalShotSe->SetVolume(0.4f);
				m_normalShotSe->Play(false);*/

			}

			//�e�Ǘ��N���X�̊֐����g�p���ďo��������
			m_bulletManager->InitBullets(
				m_position,
				m_up,
				m_shotDirection,
				m_enBulletType
			);
			
			/*m_bullet.push_back(NewGO<Bullet>(0, "bullet"));
			
			m_bullet[m_bullet.size()-1]->Init(
				*RenderingEngine::GetInstance(),
				m_position,
				m_up,
				m_shotDirection,
				m_enBulletType
			);*/

			/*m_bullet = NewGO<Bullet>(0, "bullet");

			m_bullet->Init(
				*RenderingEngine::GetInstance(),
				m_position,
				m_up,
				m_shotDirection,
				m_enBulletType
			); */

			//���ˌ�A�J�E���^�[��0�Ƀ��Z�b�g
			m_fireCounter = 0.0f;

		}
	}

	//R1�{�^���������A�J�E���^�[�����Z�A�����Ă��Ȃ��Ƃ��̓J�E���^�[��0�ɂ���B
	if (g_pad[0]->IsPress(enButtonRB1)) {
		m_fireCounter += g_gameTime->GetFrameDeltaTime();
	}
	else{
		m_fireCounter = 0.0f;
	}

}

void Player_new::ChangeWeapon()
{
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		switch (m_enUseWeapon)
		{
		case enNormalShot:
			m_enUseWeapon = enSpreadBomb;
			m_enBulletType = enPlayerSpreadBomb;
			break;
		case enSpreadBomb:
			m_enUseWeapon = enNormalShot;
			m_enBulletType = enPlayerNormal;
		default:
			break;
		}
	}
}

void Player_new::Hit()
{
	//�v���C���[�̃��C�t��0�̂Ƃ��͏������Ȃ�
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		return;
	}

	//��e����
	//�G�l�~�[�Ƃ̔���
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		//�������v�Z
		Vector3 diff = enemy->GetPosition() - m_position;
		float length = diff.Length();

		//�G�l�~�[�ɐڐG�����Ƃ�
		if (length < 60.0f) {
			
			//���g�����G��ԂłȂ��A�G�̓����蔻�肪�L���ł����
			if (m_isInvincible == false && enemy->GetIsActive() == true) {
				//1�@���炷
				m_life -= 1;
				//�Q�[���f�B���N�^�[�̕ێ����郉�C�t�����炷
				GameDirector::GetInstance()->DecPlayerLife();
				
				//�`��t���O���I�t�ɂ���
				if (m_skinModelRender->GetIsDraw() == true) {
					m_skinModelRender->SetIsDraw(false);
				}

				//�����t���O���I�t
				SetIsExist(false);

				//���G��Ԃɂ���
				SetInvincibleTime(INVINCIBLE_TIME_REVIVE);
				SetIsInvFlag(true);

				//�~�X����se���Đ�
				CSoundSource* ssMissSe = NewGO<CSoundSource>(0);
				ssMissSe->Init(L"Assets/wav/missSe.wav");
				ssMissSe->SetVolume(1.0f);
				ssMissSe->Play(false);
				/*m_missSe->SetVolume(0.8f);
				m_missSe->Play(false);*/


				//���U�G�t�F�N�g�𔭐�
				m_explosionEffect.Init(EFFECT_FILEPATH_EXPLOSION);
				m_explosionEffect.SetPosition(m_position);
				m_explosionEffect.SetRotation(m_rot);
				m_explosionEffect.SetScale(EFFECT_SCALE_EXPLOSION);
				m_explosionEffect.Play();

				return false;
			}
		}
		return true;
	});

	//�G�̔����Ƃ̔���
	QueryGOs<Explosion>("enemyExplosion", [&](Explosion* enemyExplosion) {
		//�������v�Z
		Vector3 diff = enemyExplosion->GetPosition() - m_position;
		float length = diff.Length();

		//�����ɐڐG�����Ƃ�
		if (length < enemyExplosion->GetDamageArea()) {

			//���g�����G��ԂłȂ����
			if (m_isInvincible == false) {
				//1�@���炷
				m_life -= 1;
				//�Q�[���f�B���N�^�[�̕ێ����郉�C�t�����炷
				GameDirector::GetInstance()->DecPlayerLife();

				//�`��t���O���I�t�ɂ���
				if (m_skinModelRender->GetIsDraw() == true) {
					m_skinModelRender->SetIsDraw(false);
				}

				//�����t���O���I�t
				SetIsExist(false);

				//���G��Ԃɂ���
				SetInvincibleTime(INVINCIBLE_TIME_REVIVE);
				SetIsInvFlag(true);

				//��e�G�t�F�N�g�𔭐�
				m_explosionEffect.SetPosition(m_position);
				m_explosionEffect.SetRotation(m_rot);
				m_explosionEffect.SetScale(EFFECT_SCALE_EXPLOSION);
				m_explosionEffect.Play();

				return false;
			}
		}
		return true;
	});

}

void Player_new::DecInvTime()
{
	//���G���Ԃ�����
	m_invincebleTime -= g_gameTime->GetFrameDeltaTime();
	//���G���Ԃ��؂ꂽ�疳�G�t���O���I�t�ɂ���
	if (m_invincebleTime <= 0.0f) {
		m_invincebleTime = 0.0f;
		m_isInvincible = false;
	}
}

void Player_new::AddReviveCouter()
{
	//��e���Ă����畜���܂ł̃J�E���^�[�����Z
	if (m_isExist == false) {
		m_reviveCounter += g_gameTime->GetFrameDeltaTime();
	}
	
	if (m_reviveCounter >= REVIVE_TIME) {
		//���݃t���O���I��
		m_isExist = true;
		//�J�E���^�[�����Z�b�g
		m_reviveCounter = 0.0f;
	}

}

void Player_new::Revive()
{
	//�Q�[���f�B���N�^�[���̃��C�t��0�Ȃ畜�����Ȃ�
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		return;
	}

	if (m_isExist == true
		&& m_isExistPrev == false) {

		//���������͂��łɊ������Ă���̂Ńt���O���I�t
		m_isReviveReady = false;

		//���f���̕`��t���O���I��
		if (m_skinModelRender->GetIsDraw() == false) {
			m_skinModelRender->SetIsDraw(true);
		}

	}
}

void Player_new::ReviveReady()
{
	//�Q�[���f�B���N�^�[���̃��C�t��0�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		return;
	}

	if (m_invincebleTime < 5.0f
		&& m_isExist == false) {
		//���������t���O���I��
		m_isReviveReady = true;
	}

	if (m_isReviveReadyPrev == false
		&& m_isReviveReady == true) {
		
		//�����G�t�F�N�g���Đ�
		m_reviveEffect.SetPosition(m_position);
		m_reviveEffect.SetRotation(m_rot);
		m_reviveEffect.SetScale(EFFECT_SCALE_REVIVE);
		m_reviveEffect.Play();
	}
}

void Player_new::CalcCameraUpFractionAddRate()
{
	//�v���C���[�̏�����ƃJ�����̏�����̓��ς���A�J�����̏��������]�������ԗ��ɑ����l�𒲐����鏈��
	//���ς�0�ɋ߂��قǑ����l�͑傫���Ȃ�(������]����)�A���ς�1�ɋ߂��قǑ����l�͏������Ȃ�(��������]����)

	//�����l�̍ő�l�ƍŏ��l�����
	float addRate = 0.0f;
	float maxAddRate = CAMERA_ROTATE_FRACTION_ADD_RATE_MAX;
	float minAddRate = CAMERA_ROTATE_FRACTION_ADD_RATE_MIN;
	//��x�x�N�g���Ɋi�[����
	Vector3 addRateVec = { 0.0f,0.0f,0.0f, };
	Vector3 maxAddRateVec = { maxAddRate,0.0f,0.0f };
	Vector3 minAddRateVec = { minAddRate,0.0f,0.0f };

	//�v���C���[�̏�����ƃJ�����̏�����œ��ς��Ƃ�
	float dotVec = Dot(m_cameraUp, m_up);
	//1�𑫂�2�Ŋ��邱�Ƃ�-1.0f�`1.0f��0.0f�`1.0f�ɕϊ�����
	dotVec += 1.0f;
	dotVec /= 2.0f;
	//���ς̌��ʂ�1�ɋ߂��قǑ����l���������Ȃ�
	addRateVec.Lerp(dotVec, maxAddRateVec * 0.9f, minAddRateVec * 0.5f);
	//�x�N�g������float�l�ɕϊ�
	addRate = addRateVec.Length();

	//�J�����̏��⊮����W�������Z
	m_cameraUpFraction += addRate;
	//1�𒴂�����1�ɕ␳
	if (m_cameraUpFraction > 1.0f) {
		m_cameraUpFraction = 1.0f;
	}

}

void Player_new::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame &&
		GameDirector::GetInstance()->GetGameState() != enStart) {
		return;
	}

	CalcCameraUpFractionAddRate();
	Move();
	Rotation();
	RotateShotDirection();
	ChangeWeapon();
	FireBullet();
	Hit();
	AddReviveCouter();
	ReviveReady();
	Revive();
	DecInvTime();

	//�O�t���[���̏���������t���[���̏�ƕς���Ă������ԌW����0�ɂ���
	if (m_upPrev.x != m_up.x
		|| m_upPrev.y != m_up.y
		|| m_upPrev.z != m_up.z) {
		m_cameraUpFraction = 0.0f;
	}

	//�J�����Ǐ]
	////�J���������_���王�_�ւ̃x�N�g�����쐬
	//Vector3 toCamera = m_gameCamera.GetCameraPosition() - m_gameCamera.GetTargetPosition();
	//�����_�ڕW����J�����ڕW�ւ̃x�N�g�����쐬
	Vector3 toCamera = m_gameCamera.GetCameraPositionTarget() - m_gameCamera.GetTargetPositionTarget();

	//�x�N�g���ɃN�H�[�^�j�I����K�p
	m_rotUpToGroundNormal.Apply(toCamera);
	/*m_cameraRot.Apply(toCamera);
	m_cameraRot.Apply(toCamera);*/
	//mulRot.Apply(toCamera);

	////�����_�����g�ɐݒ�
	//m_gameCamera.SetTargetPosition(m_position);
	//�����_�ڕW�����g�ɐݒ�
	m_gameCamera.SetTargetPositionTarget(m_position);

	////���_��ݒ�
	//m_gameCamera.SetCameraPosition(m_position + toCamera);
	//�J�����ڕW��ݒ�
	m_gameCamera.SetCameraPositionTarget(m_position + toCamera);

	// �J�����̏�����ڕW���v���C���[�̏�����ɐݒ�B
	m_gameCamera.SetUpVectorTarget(m_up);

	//�J�����̏����������Ԃ��Ă���
	m_gameCamera.LerpUpVector(m_cameraUpFraction, m_cameraUp);
	//��Ԃ����J�����̏�ŃJ�����̏���X�V
	m_gameCamera.SetUp(m_cameraUp);
	//�J�����̍X�V
	m_gameCamera.UpdateCamera();


	//�e��t���O�̋L�^
	//////////////////////////////////
	//���t���[���̏���L�^
	m_upPrev = m_up;
	//���t���[���̖��G�t���O���L�^
	m_isInvinciblePrev = m_isInvincible;
	//���t���[���̑��݃t���O���L�^
	m_isExistPrev = m_isExist;
	m_isReviveReadyPrev = m_isReviveReady;

	//�G�t�F�N�g�̍X�V
	//m_startEffect.SetScale(EFFECT_SCALE_START);
	m_startEffect.SetPosition(m_position + m_up * 50.0f);
	//m_startEffect.SetRotation(m_rot);

	m_startEffect.Update();
	m_explosionEffect.Update();
	m_reviveEffect.Update();
	m_moveTrackEffect.Update();
	m_markerEffect.Update();

}