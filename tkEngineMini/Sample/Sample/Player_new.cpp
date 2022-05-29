#include "stdafx.h"
#include "Player_new.h"
#include "Enemy.h"


namespace{
	const char* MODELPATH_UTC = "Assets/modelData/player/player_object_c.tkm";
	const char* SKELETON_PATH_UTC = "Assets/modelData/unityChan.tks";
	const float UTC_RADIUS = 40.0f;
	const float UTC_HEIHGT = 100.0f;
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	
	const Vector3 INIT_POINT = {0.0f,700.0f,0.0f};			//�����ʒu
	const float UPPER_OFFSET = 20.0f;						//�n�`���畂�����鋗��

	//�v���C���[�̃X�y�b�N
	const float PL_MOVE_SPEED = -12.0f;			//�ړ����x
	const float PL_HIT_RANGE = 60.0f;			//�v���C���[�̔�e���苗��
	const float FIRECOUNTER_NORMAL = 0.075f;	//�ʏ�e�̔��ˊԊu
	const float FIRECOUNTER_SPREAD = 0.25f;		//�X�v���b�h�{���̔��ˊԊu
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

	const char16_t* EFFECT_FILEPATH_DIRECTION = u"Assets/effect/shotDirection.efk";	//�����G�t�F�N�g�̃t�@�C���p�X
	const Vector3 EFFECT_SCALE_DIRECTION = { 10.0f,10.0f,10.0f };					//�����G�t�F�N�g�̊g�嗦

	const char16_t* EFFECT_FILEPATH_MARKER = u"Assets/effect/positionMarker.efk";	//�����蔻��G�t�F�N�g�̃t�@�C���p�X
	const Vector3 EFFECT_SCALE_MARKER = { 30.0f,30.0f,30.0f };						//�����蔻��G�t�F�N�g�̊g�嗦
	const float MARKER_PLAY_INTERVAL = 0.02f;										//�����蔻��G�t�F�N�g�̔����Ԋu

	const char16_t* EFFECT_FILEPATH_CHANGE_NORMAL = u"Assets/effect/change_weapon_normal.efk";	//�V���b�g�ύX�G�t�F�N�g(�m�[�}��)�̃t�@�C���p�X
	const Vector3 EFFECT_SCALE_CHANGE_NORMAL = { 15.0f,15.0f,15.0f };							//�V���b�g�ύX�G�t�F�N�g(�m�[�}��)�̊g�嗦

	const char16_t* EFFECT_FILEPATH_CHANGE_SPREAD = u"Assets/effect/change_weapon_spread.efk";	//�V���b�g�ύX�G�t�F�N�g(�X�v���b�h)�̃t�@�C���p�X
	const Vector3 EFFECT_SCALE_CHANGE_SPREAD = { 15.0f,15.0f,15.0f };							//�V���b�g�ύX�G�t�F�N�g(�X�v���b�h)�̊g�嗦

	//SE�̃t�@�C���p�X,����
	//�m�[�}���V���b�gse
	const wchar_t* SHOT_NORMAL_SE_FILEPATH = L"Assets/wav/normalShotSe_1.wav";
	const float SHOT_NORMAL_SE_VOLUME = 0.4f;
	//�X�v���b�h�{��se
	const wchar_t* SHOT_SPREAD_SE_FILEPATH = L"Assets/wav/shot_spread.wav";
	const float SHOT_SPREAD_SE_VOLUME = 0.4f;
	//��ese
	const wchar_t* MISS_SE_FILEPATH = L"Assets/wav/missSe.wav";
	const float MISS_SE_VOLUME = 1.0f;
}

Player_new::Player_new()
{

}

Player_new::~Player_new()
{
	DeleteGO(m_skinModelRender);
}

void Player_new::Init()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

	//�G�t�F�N�g��������
	InitEffect();

	//���C�g������,�󂯎��
	m_directionLight = FindGO<DirectionLight>(DIRECTION_LIGHT_NAME);
	m_pointLight = FindGO<PointLight>(POINT_LIGHT_NAME);
	m_spotLight = FindGO<SpotLight>(SPOT_LIGHT_NAME);
	if (m_directionLight != nullptr) { RecieveDirectionLight(m_directionLight); }
	if (m_pointLight != nullptr) { RecievePointLight(m_pointLight); }
	if (m_pointLight != nullptr) { RecieveSpotLight(m_spotLight); }


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

	//���������ɊJ�n���o�p�G�t�F�N�g�𔭐�������
	m_startEffect.SetScale(EFFECT_SCALE_START);
	m_startEffect.SetPosition(m_position);
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
	m_shotDirectionEffect.Init(EFFECT_FILEPATH_DIRECTION);
	m_changeEffectNormal.Init(EFFECT_FILEPATH_CHANGE_NORMAL);
	m_changeEffectSpread.Init(EFFECT_FILEPATH_CHANGE_SPREAD);
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
	//�O���A��A�E���X�V
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);
	
	//��A�E�x�N�g���̏��������h�~�p
	Vector3 fixedUp = m_up;
	Vector3 fixedRight = m_right;
	//�V���b�g�����̊�ƂȂ�O�����X�V
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_fixedForward, fixedUp, fixedRight);
	

	// ���݂̏�x�N�g������A�V������x�N�g���Ɍ����邽�߂̉�]�N�H�[�^�j�I�����v�Z
	// ���@�J�����̌v�Z�Ŏg���B
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
	//�ˌ������̊�͌Œ�̏����
	m_shotDirection = m_fixedForward * -1.0f;

	//��]���͏�x�N�g��
	Vector3 axis = m_up;
	//������̉�]�N�H�[�^�j�I�����쐬
	Quaternion rot;

	//�E�X�e�B�b�N�̓��͂��󂯎��
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//���X�e�B�b�N�̓��͂��󂯎��
	float xL = g_pad[0]->GetLStickXF();
	float yL = g_pad[0]->GetLStickYF();

	//���͒l����p�x�����߂�
	float angle = atan2f(x, y);
	//������̉�]�����߂�
	rot.SetRotation(axis, angle);

	//�V���b�g�̎ˏo�����̌���
	//�ړ���
	if (fabsf(xL) > 0.001f || fabsf(yL) > 0.001f) {
		//���A�E�X�e�B�b�N�̓��͂�����Ƃ��A�E�X�e�B�b�N�̕��֎ˌ�
		if (fabsf(x) > 0.001f || fabsf(y) > 0.001f) {
			rot.Apply(m_shotDirection);
		}
		//�E�X�e�B�b�N�̓��͂��Ȃ��Ƃ��A���@�̌����Ă�������֎ˌ�
		else {
			m_shotDirection = m_forward * -1.0f;
		}
	}
	//��~��
	else {
		//���E�X�e�B�b�N�̓��͂�����Ƃ��A�E�X�e�B�b�N�̕��֎ˌ�
		if (fabsf(x) > 0.001f || fabsf(y) > 0.001f) {
			rot.Apply(m_shotDirection);
		}
		//�E�X�e�B�b�N�̓��͂��Ȃ��Ƃ��A���@�̌����Ă�������֎ˌ�
		else {
			m_shotDirection = m_forward * -1.0f;
		}
	}

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
		//�ʏ�V���b�g
		if (m_enUseWeapon == enNormalShot) {
			if (m_fireCounter > FIRECOUNTER_NORMAL || m_fireCounter == 0.0f) {

				//�V���b�gSE�̍Đ�
				if (m_enBulletType == enNormalShot) {
					/*CSoundSource* ssNormalSe = NewGO<CSoundSource>(0);
					ssNormalSe->Init(SHOT_NORMAL_SE_FILEPATH);
					ssNormalSe->SetVolume(SHOT_NORMAL_SE_VOLUME);
					ssNormalSe->Play(false);*/
					m_ssNormalShotSe = NewGO<CSoundSource>(0);
					m_ssNormalShotSe->Init(SHOT_NORMAL_SE_FILEPATH);
					m_ssNormalShotSe->SetVolume(SHOT_NORMAL_SE_VOLUME);
					m_ssNormalShotSe->Play(false);
				}


				//�e�Ǘ��N���X�̊֐����g�p���ďo��������
				m_bulletManager->InitBullets(
					m_position,
					m_up,
					m_shotDirection,
					m_enBulletType
				);

				//���ˌ�A�J�E���^�[��0�Ƀ��Z�b�g
				m_fireCounter = 0.0f;

			}
		}
		//�X�v���b�h�{��
		if (m_enBulletType == enSpreadBomb) {
			if (m_fireCounter > FIRECOUNTER_SPREAD || m_fireCounter == 0.0f) {
				//�V���b�gSE�̍Đ�
				if (m_enBulletType == enSpreadBomb) {
					CSoundSource* ssSpreadSe = NewGO<CSoundSource>(0);
					ssSpreadSe->Init(SHOT_SPREAD_SE_FILEPATH);
					ssSpreadSe->SetVolume(SHOT_SPREAD_SE_VOLUME);
					ssSpreadSe->Play(false);
				}

				//�e�Ǘ��N���X�̊֐����g�p���ďo��������
				m_bulletManager->InitBullets(
					m_position,
					m_up,
					m_shotDirection,
					m_enBulletType
				);

				//���ˌ�A�J�E���^�[��0�Ƀ��Z�b�g
				m_fireCounter = 0.0f;
			}
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
			
			//�X�v���b�h�{���ɕύX����G�t�F�N�g���Đ�
			m_changeEffectSpread.SetPosition(m_position);
			m_changeEffectSpread.SetRotation(m_rot);
			m_changeEffectSpread.SetScale(EFFECT_SCALE_CHANGE_SPREAD);
			m_changeEffectSpread.Play(false);

			break;
		case enSpreadBomb:
			m_enUseWeapon = enNormalShot;
			m_enBulletType = enPlayerNormal;
			
			//�m�[�}���V���b�g�ɕύX����G�t�F�N�g���Đ�
			m_changeEffectNormal.SetPosition(m_position);
			m_changeEffectNormal.SetRotation(m_rot);
			m_changeEffectNormal.SetScale(EFFECT_SCALE_CHANGE_NORMAL);
			m_changeEffectNormal.Play(false);

			break;
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
	QueryGOs<Enemy>(ENEMY_NAME, [&](Enemy* enemy) {
		//�������v�Z
		Vector3 diff = enemy->GetPosition() - m_position;
		float length = diff.Length();

		//�G�l�~�[�ɐڐG�����Ƃ�
		if (length < PL_HIT_RANGE) {
			
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
				ssMissSe->Init(MISS_SE_FILEPATH);
				ssMissSe->SetVolume(MISS_SE_VOLUME);
				ssMissSe->Play(false);

				//��e�G�t�F�N�g�𔭐�
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

	//�G�e�Ƃ̔���
	QueryGOs<Bullet>(BULLET_ENEMY_NAME, [&](Bullet* enemyBullet) {
		//�������v�Z
		Vector3 diff = enemyBullet->GetPosition() - m_position;
		float length = diff.Length();

		//�G�l�~�[�ɐڐG�����Ƃ�
		if (length < PL_HIT_RANGE) {

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

				//�~�X����se���Đ�
				CSoundSource* ssMissSe = NewGO<CSoundSource>(0);
				ssMissSe->Init(MISS_SE_FILEPATH);
				ssMissSe->SetVolume(MISS_SE_VOLUME);
				ssMissSe->Play(false);

				//��e�G�t�F�N�g�𔭐�
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
	QueryGOs<Explosion>(EXPLOSION_ENEMY_NAME, [&](Explosion* enemyExplosion) {
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

	//���t���[���Ő����t���O����������
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

	//���t���[���ŕ��������t���O���I���Ȃ�
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

void Player_new::EffectUpdate()
{
	m_startEffect.SetPosition(m_position + m_up * 50.0f);
	//m_startEffect.SetRotation(m_rot);
	m_startEffect.Update();
	m_explosionEffect.Update();
	m_reviveEffect.Update();
	m_moveTrackEffect.Update();
	m_markerEffect.Update();

	m_shotDirectionEffect.SetPosition(m_position);
	m_shotDirectionEffect.SetScale(EFFECT_SCALE_DIRECTION);
	//m_shotDirectionEffect.SetRotation(m_rot);

	m_changeEffectNormal.SetPosition(m_position);
	//m_changeEffectNormal.SetRotation(m_rot);
	m_changeEffectNormal.SetScale(EFFECT_SCALE_CHANGE_NORMAL);
	m_changeEffectNormal.Update();

	m_changeEffectSpread.SetPosition(m_position);
	//m_changeEffectSpread.SetRotation(m_rot);
	m_changeEffectSpread.SetScale(EFFECT_SCALE_CHANGE_SPREAD);
	m_changeEffectSpread.Update();

	if (m_shotDirectionEffect.IsPlay() != true) {
		m_shotDirectionEffect.Play();
	}
	//m_shotDirectionEffect.Update();
}

void Player_new::Update()
{
	//�Q�[�����A�X�^�[�g���ȊO�Ȃ珈�����Ȃ�
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
	////////////////////////////////////////////////////////////
	//�����_�ڕW����J�����ڕW�ւ̃x�N�g�����쐬
	Vector3 toCamera = m_gameCamera.GetCameraPositionTarget() - m_gameCamera.GetTargetPositionTarget();
	//�x�N�g���ɃN�H�[�^�j�I����K�p
	m_rotUpToGroundNormal.Apply(toCamera);
	
	//�����_�ڕW�����g�ɐݒ�
	m_gameCamera.SetTargetPositionTarget(m_position);
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
	////////////////////////////////////////////////////////////

	//�e��t���O�̋L�^
	////////////////////////////////////////////////////////////
	//���t���[���̏���L�^
	m_upPrev = m_up;
	//���t���[���̖��G�t���O���L�^
	m_isInvinciblePrev = m_isInvincible;
	//���t���[���̑��݃t���O���L�^
	m_isExistPrev = m_isExist;
	m_isReviveReadyPrev = m_isReviveReady;
	////////////////////////////////////////////////////////////

	//�G�t�F�N�g�̍X�V
	EffectUpdate();
}