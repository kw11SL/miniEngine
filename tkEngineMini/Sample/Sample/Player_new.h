#pragma once

class Player_new : public IGameObject
{
public:
	Player_new(){}
	~Player_new();

	//�Q�b�^�[
	/// @brief	���W���擾 
	/// @return ���W
	Vector3 GetPosition() 
	{ 
		return m_position; 
	}
	
	/// @brief �g�嗦���擾
	/// @return �g�嗦
	Vector3 GetScale() 
	{ 
		return m_scale; 
	}
	
	/// @brief ��]���擾
	/// @return ��]
	Quaternion GetRotation() 
	{ 
		return m_rot; 
	}
	
	/// @brief ��]�p�x���擾
	/// @return ��]�p�x
	float GetAngle() 
	{ 
		return m_angle; 
	}

	/// @brief �X�L�����f�������_�[���擾
	/// @return �X�L�����f�������_�[
	SkinModelRender* GetSkinModelRender()
	{
		return m_skinModelRender;
	}

	//�Z�b�^�[
	/// @brief	���W��ݒ� 
	/// @param pos ���W
	void SetPostion(const Vector3& pos)
	{
		m_position = pos;
		m_skinModelRender->SetPosition(m_position);
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale �g�嗦
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_skinModelRender->SetScale(m_scale);
	}

	/// @brief ��]��ݒ�
	/// @param rot	��]
	void SetRotation(const Quaternion rot)
	{
		m_rot = rot;
	}
	
	/// @brief �p�x��ݒ�
	/// @param angle �p�x
	void SetAngle(const float& angle)
	{
		m_angle = angle;
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

private:
	//�����Ŏg������

	bool Start() override;

	void Update() override;

	/// @brief �ړ�����
	void Move();

	/// @brief ��]����
	void Rotation();

	void RotateShotDirection();

	/// @brief �e�𔭎�
	void FireBullet();

private:

	SkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[
	MyCharacterController m_myCharaCon;					//����̃L�����N�^�[�R���g���[��
	SphericalMove m_sphericalMove;						//���ʈړ��p�N���X
	GameCamera m_gameCamera;							//�Q�[���J����
	//Vector3 m_cameraPoint = Vector3::Zero;				//�J�������ǂ�������|�C���g
	Bullet* m_bullet = nullptr;							//�e�N���X

	Vector3 m_position = Vector3::Zero;					//���W
	Vector3 m_moveSpeed = Vector3::Zero;				//���x�x�N�g��
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//���C���΂������x�N�g��
	Vector3 m_forward = Vector3::Zero;					//�O��
	Vector3 m_right	= Vector3::Zero;					//�E
	Vector3 m_up = Vector3::Zero;						//��
	Vector3 m_scale = Vector3::One;						//�g�嗦
	Quaternion m_rot = Quaternion::Identity;			//��]
	Quaternion m_rotUpToGroundNormal = Quaternion::Identity;					//�v���C���[�̏�x�N�g����n�ʂ̖@���Ɍ��������]�N�H�[�^�j�I��
	float m_angle = 0.0f;								//��]�p�x

	float m_fireCounter = 0.0f;							//���ˊԊu�J�E���^
	Vector3 m_shotDirection = Vector3::Zero;			//�V���b�g��������

};

