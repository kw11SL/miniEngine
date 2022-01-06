#pragma once

enum EnUseWeapon {
	enNormalShot,
	enSpreadBomb,
	enUseWeaponNum
};

class Player_new : public IGameObject
{
public:
	Player_new(){}
	~Player_new();

	//�Q�b�^�[
	/// @brief	���W���擾 
	/// @return ���W
	const Vector3& GetPosition() const 
	{ 
		return m_position; 
	}

	/// @brief ��x�N�g�����擾
	/// @return ��x�N�g��
	const Vector3& GetUp() const
	{
		return m_up;
	}
	
	/// @brief �g�嗦���擾
	/// @return �g�嗦
	const Vector3& GetScale() const 
	{ 
		return m_scale; 
	}
	
	/// @brief ��]���擾
	/// @return ��]
	const Quaternion& GetRotation() const 
	{ 
		return m_rot; 
	}
	
	/// @brief ��]�p�x���擾
	/// @return ��]�p�x
	const float GetAngle() const 
	{ 
		return m_angle; 
	}

	/// @brief �X�L�����f�������_�[���擾
	/// @return �X�L�����f�������_�[
	const SkinModelRender* GetSkinModelRender()
	{
		return m_skinModelRender;
	}

	/// @brief ���G��Ԃ��擾
	/// @return 
	bool GetIsInvincible()
	{
		return m_isInvincible;
	}

	/// @brief ���݃t���O���擾
	/// @return 
	bool GetIsExist()
	{
		return m_isExist;
	}

	//�Z�b�^�[
	/// @brief	���W��ݒ� 
	/// @param pos ���W
	void SetPostion(const Vector3& pos)
	{
		m_position = pos;
		//m_skinModelRender->SetPosition(m_position);
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale �g�嗦
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		//m_skinModelRender->SetScale(m_scale);
	}

	/// @brief ��]��ݒ�
	/// @param rot	��]
	void SetRotation(const Quaternion& rot)
	{
		m_rot = rot;
	}

	/// @brief �s�񂩂��]��ݒ�
	/// @param mat �s��
	void SetRotationFromMatrix(const Matrix& mat)
	{
		m_rot.SetRotation(mat);
	}
	
	/// @brief �p�x��ݒ�
	/// @param angle �p�x
	void SetAngle(const float angle)
	{
		m_angle = angle;
	}

	/// @brief ���G��ԃt���O���Z�b�g
	/// @param flag 
	void SetIsInvFlag(const bool invFlag)
	{
		m_isInvincible = invFlag;
	}

	/// @brief �����t���O���Z�b�g
	/// @param existFlag 
	void SetIsExist(const bool existFlag)
	{
		m_isExist = existFlag;
	}

	/// @brief ���G���Ԃ�ݒ�
	/// @param invTime 
	void SetInvincibleTime(const float invTime)
	{
		m_invincebleTime = invTime;
	}

	/// @brief �������x�N�g����ݒ�
	/// @param up ������x�N�g��
	void SetDownVector(const Vector3& up)
	{
		m_downVector = up * -1.0f;
	}
	
	/// @brief ����������
	/// @param renderingEngine�@�����_�����O�G���W�� 
	void Init(RenderingEngine& renderingEngine);

	//���C�g��n�����߂̊֐�
	
	/// @brief �f�B���N�V�������C�g���󂯂Ƃ�
	/// @param dirLight �f�B���N�V�������C�g
	void RecieveDirectionLight(DirectionLight* dirLight)
	{
		m_skinModelRender->InitDirectionLight(dirLight);
	}
	
	/// @brief �|�C���g���C�g���󂯎��
	/// @param ptLight �|�C���g���C�g
	void RecievePointLight(PointLight* ptLight)
	{
		m_skinModelRender->InitPointLight(ptLight);
	}
	
	/// @brief �X�|�b�g���C�g���󂯎��
	/// @param spLight �X�|�b�g���C�g
	void RecieveSpotLight(SpotLight* spLight)
	{
		m_skinModelRender->InitSpotLight(spLight);
	}

	/// @brief ���f���̏�����
	void InitModelFromInitData()
	{
		m_skinModelRender->InitModel();
	}

	/// @brief �L�����R���̏�����
	void InitCharaCon()
	{
		m_myCharaCon.Init(m_position);
	}

	/// @brief ���f����������
	void Revive();

	/// @brief �������������ƃG�t�F�N�g�Đ�����
	void ReviveReady();

	/// @brief �N�H�[�^�j�I���ŉE�A��A�O����]
	/// @param rot �K�p����N�H�[�^�j�I��
	void SetVectorFromQuaternion(const Quaternion& rot)
	{
		//�e�x�N�g���ɃN�H�[�^�j�I����K�p
		rot.Apply(m_right);
		rot.Apply(m_up);
		rot.Apply(m_forward);

		//�e�x�N�g���𐳋K��
		m_right.Normalize();
		m_up.Normalize();
		m_forward.Normalize();

		//�ύX������x�N�g���ŉ������x�N�g����ݒ�
		SetDownVector(m_up);
	}

private:
	//�����Ŏg������

	bool Start() override;

	void Update() override;

	/// @brief �ړ�����
	void Move();

	/// @brief ��]����
	void Rotation();

	/// @brief �e�̔��˕�������]
	void RotateShotDirection();

	/// @brief �e�𔭎�
	void FireBullet();

	/// @brief ���G���Ԃ��������A0�ȉ��Ȃ疳�G�t���O������
	void DecInvTime();

	/// @brief ��e���畜���܂ł̃J�E���^�[�𑝉�
	void AddReviveCouter();

	/*void AddBullet(Bullet& bullet)
	{
		m_bullet.push_back(&bullet);
	}*/

	/// @brief ����̐؂�ւ��@�\
	void ChangeWeapon();

	/// @brief �G��G�e�Ƃ̓����蔻�菈��
	void Hit();

	/// @brief �G�t�F�N�g�̏�����
	void InitEffect();

private:

	SkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[
	MyCharacterController m_myCharaCon;					//����̃L�����N�^�[�R���g���[��
	SphericalMove m_sphericalMove;						//���ʈړ��p�N���X
	GameCamera m_gameCamera;							//�Q�[���J����
	
	//Bullet* m_bullet = nullptr;
	std::vector<Bullet*> m_bullet;							//�e�N���X�̉ϒ��z��
	
	Explosion* m_explosion = nullptr;

	Vector3 m_position = Vector3::Zero;					//���W
	Vector3 m_moveSpeed = Vector3::Zero;				//���x�x�N�g��
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//���C���΂������x�N�g��
	Vector3 m_forward = Vector3::Front;					//�O��
	Vector3 m_right	= Vector3::Right;					//�E
	Vector3 m_up = Vector3::Up;							//��
	Vector3 m_upPrev = Vector3::Zero;					//�O�t���[���̏�
	Vector3 m_cameraUp = Vector3::Zero;					//�J�����̏�(�J�����ɔ��f��������)
	Vector3 m_scale = Vector3::One;						//�g�嗦
	Quaternion m_rot = Quaternion::Identity;			//��]
	Quaternion m_rotUpToGroundNormal = Quaternion::Identity;
	
	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	float m_rotFraction = 1.0f;
	float m_cameraUpFraction = 0.0f;					//�J�������������]�������ԗ�
	float m_angle = 0.0f;								//��]�p�x

	int m_life = 0;										//�v���C���[�̃��C�t
	bool m_isInvincible = false;						//���G��ԃt���O
	bool m_isInvinciblePrev = false;					//�O�t���[���̖��G��ԃt���O
	float m_reviveCounter = 0.0f;						//��e���畜���܂ł̃J�E���^�[
	bool m_isExist = true;								//�����t���O
	bool m_isExistPrev = true;							//�O�t���[���̐����t���O
	bool m_isReviveReady = false;						//���������t���O(�G�t�F�N�g�Đ��p)
	bool m_isReviveReadyPrev = false;					//�O�t���[���̕��������t���O
	float m_markerCounter = 0.0f;						//���@�ʒu�\���̃G�t�F�N�g�Đ��J�E���^�[

	float m_invincebleTime = 0.0f;						//���G����
	float m_fireCounter = 0.0f;							//���ˊԊu�J�E���^
	Vector3 m_shotDirection = Vector3::Zero;			//�V���b�g��������

	EnUseWeapon m_enUseWeapon = enNormalShot;			//���ݎg�p���Ă���V���b�g
	EnBulletType m_enBulletType = enPlayerNormal;		//�e�ɓn���e�̃^�C�v���

	BulletManager* m_bulletManager = nullptr;			//�e�̊Ǘ��N���X�̃I�u�W�F�N�g

	Effect m_explosionEffect;							//��e���̃G�t�F�N�g
	Effect m_reviveEffect;								//�������̃G�t�F�N�g
	Effect m_moveTrackEffect;							//�ړ����̋O�ՃG�t�F�N�g
	Effect m_markerEffect;								//���@�ʒu�̕\���G�t�F�N�g
};

