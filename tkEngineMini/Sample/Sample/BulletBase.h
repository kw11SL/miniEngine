#pragma once
class BulletBase : public IGameObject
{
public:
	BulletBase(){}
	virtual ~BulletBase();

	/// @brief ����������
	/// @param renderingEngine �����_�����O�G���W��
	/// @param initPoint �����ʒu
	/// @param initUp �����ʒu�̏�x�N�g��
	/// @param direction ����
	void Init(
		RenderingEngine& renderingEngine,
		const Vector3& initPoint,
		const Vector3& initUp,
		const Vector3& direction
		);

	/// @brief �����������ɕt�����鏈��
	virtual void InitSub();

	/// @brief �X�V�����ɕt�����鏈��
	virtual void UpdateSub();

	//�Q�b�^�[
	/// @brief	���W���擾 
	/// @return ���W
	Vector3 GetPosition() { return m_position; }

	/// @brief �g�嗦���擾
	/// @return �g�嗦
	Vector3 GetScale() { return m_scale; }

	/// @brief ��]���擾
	/// @return ��]
	Quaternion GetRotation() { return m_rot; }

	/// @brief ��]�p�x���擾
	/// @return ��]�p�x
	float GetAngle() { return m_angle; }

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
		m_skinModelRender->SetRotation(m_rot);
	}

	/// @brief �p�x��ݒ�
	/// @param angle �p�x
	void SetAngle(const float& angle)
	{
		m_angle = angle;
	}

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

	/// @brief ���݃t���O���I��
	void SetIsExist()
	{
		m_isExist = true;
	}

	/// @brief ���݃t���O���擾
	/// @return 
	bool GetIsExist()
	{
		return m_isExist;
	}

	/// @brief ���f�����폜���ꂽ���ǂ���
	/// @return 
	bool GetIsModelDeleted()
	{
		return m_isModelDeleted;
	}

	/// @brief �e�̃_���[�W���擾
	/// @return 
	float GetPower()
	{
		return m_power;
	}

	/// @brief �c�葶�ݎ��Ԃ��擾
	/// @return 
	float GetLifeTime()
	{
		return m_lifeTime;
	}

	/// @brief �_���[�W�Ԋu���擾
	/// @return 
	float GetDamageInterval()
	{
		return m_damageInterval;
	}

	/// @brief �X�L�����f�����폜
	void DeleteSkinModel()
	{
		DeleteGO(m_skinModelRender);
		m_isModelDeleted = true;
	}

	/// @brief �ϋv�l������
	/// @param decVal ����������l
	void DecLife(float decVal)
	{
		m_life -= decVal;
	}

	//�����Ŏg���֐�
	bool Start() override
	{
		return true;
	}

	/// @brief �X�V����
	void Update() override;

	/// @brief �ړ�����
	void Move();

	/// @brief ��]����
	void Rotation();

	/// @brief �������Ԃ��������A0�ȉ��Ŕj��
	void DecLifeTime();

	/// @brief �폜����
	void Destroy();

	/// @brief �e�G�t�F�N�g�̏���������
	/// @param filePath �t�@�C���p�X
	void InitEffect(const char16_t* filePath);

protected:
	SkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[
	MyCharacterController m_myCharaCon;					//����̃L�����N�^�[�R���g���[��
	SphericalMove m_sphericalMove;						//���ʈړ��p�N���X

	float m_life = 0.0f;								//�ϋv�l
	float m_lifeTime = 0.0f;							//���Ԏ���
	float m_speed = 0.0f;								//�ړ����x
	float m_power = 0.0f;								//�e���^����_���[�W
	bool m_isExist = true;								//���݃t���O
	float m_damageInterval = 0.5f;						//�_���[�W��^����Ԋu�B���̐��l�����G�l�~�[�ɖ��G���Ԃ�ݒ肷��B

	Vector3 m_position = Vector3::Zero;					//���W
	Vector3 m_moveSpeed = Vector3::Zero;				//���x�x�N�g��
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//���C���΂������x�N�g��
	Vector3 m_forward = Vector3::Zero;					//�O��
	Vector3 m_oldForward = Vector3::Zero;				//���ʈړ��N���X�ɂ��X�V�O�̋L�^�p(��]�Ɏg�p����)
	Vector3 m_right = Vector3::Zero;					//�E
	Vector3 m_up = Vector3::Zero;						//��
	Vector3 m_scale = Vector3::One;						//�g�嗦
	Quaternion m_rot = Quaternion::Identity;			//��]
	float m_angle = 0.0f;								//�p�x

	//���C�g�ێ��p�̃����o
	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	Vector3 m_direction = Vector3::Zero;				//�ŏ��̔��˕���
	bool m_isDecideDirection = false;					//���˕�����O���x�N�g���ɂ������ǂ���

	Effect m_shotEffect;								//�e�ɕt������G�t�F�N�g

	//�X�L�����f�������_�[�̍폜�t���O
	bool m_isModelDeleted = false;
};

