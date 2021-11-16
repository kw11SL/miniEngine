#pragma once
/// @brief �����̊��N���X

class ExplosionBase : public IGameObject
{
public:
	ExplosionBase() {}
	virtual ~ExplosionBase() {}

	bool Start() override
	{
		return true;
	}

	/// @brief ����������
	/// @param pos �����ʒu
	/// @param scale �g�嗦
	/// @param effectFilePath �G�t�F�N�g�̃t�@�C���p�X 
	void Init(
		const Vector3& pos,
		const float scale
	);

	/// @brief �������ɕt�����鏈��
	virtual void InitSub();

	/// @brief �G�t�F�N�g�̏�����
	void InitEffect(const char16_t* filePath);

	/// @brief �X�V����
	void Update() override;
	
	/// @brief �X�V�����ɕt�����鏈�� 
	virtual void UpdateSub();

	/// @brief ���W��ݒ�
	/// @param pos 
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale 
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// @brief �g�嗦(float��)��ݒ�
	/// @param scaleRate 
	void SetScaleRate(const float scaleRate)
	{
		m_scaleRate = scaleRate;
	}

	/// @brief ��]�N�H�[�^�j�I����ݒ�
	/// @param rot 
	void SetRotation(const Quaternion& rot)
	{
		m_rot = rot;
	}

	/// @brief �ړ����x�x�N�g����ݒ�
	/// @param speed 
	void SetMoveSpeed(const Vector3& moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}

	/// @brief ������ݒ�
	/// @param speed 
	void SetSpeed(const float speed)
	{
		m_speed = speed;
	}

	/// @brief ���W���擾
	/// @return 
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// @brief �g�嗦���擾
	/// @return 
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/// @brief �g�嗦(float��)���擾
	/// @return 
	const float GetScaleRate() const
	{
		return m_scaleRate;
	}

	/// @brief ��]�N�H�[�^�j�I�����擾
	/// @return 
	const Quaternion& GetRotation() const
	{
		return m_rot;
	}
	
	/// @brief �ړ����x�x�N�g�����擾
	/// @return 
	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	/// @brief �_���[�W�l���擾
	/// @return 
	const float GetPower() const
	{
		return m_power;
	}

	/// @brief �_���[�W���a���擾
	/// @return 
	const float GetDamageArea() const
	{
		return m_damageArea;
	}

	/// @brief �_���[�W�Ԋu���擾
	/// @return 
	const float GetDamageInterval() const
	{
		return m_damageInterval;
	}

	const bool GetIsExist() const
	{
		return m_isExist;
	}

	/// @brief �G�t�F�N�g�ɍ��W�A��]�N�H�[�^�j�I���A�g�嗦��K�p���čX�V
	void EffectUpdate();

	/// @brief �������Ԃ�����
	void DecLifeTime();

	/// @brief �G�t�F�N�g���Đ�
	void PlayEffect()
	{
		if (m_effect.IsPlay() == false) {
			m_effect.Play();
		}
	}

	/// @brief �����t���O�𒲂ׂč폜���鏈��
	void Destroy();

protected:
	Effect m_effect;							//�G�t�F�N�g
	const char16_t* m_effectFilePath = u"";		//�G�t�F�N�g�t�@�C���p�X
	Vector3 m_position = Vector3::Zero;			//���W
	Quaternion m_rot = Quaternion::Identity;	//��]�N�H�[�^�j�I��
	Vector3 m_scale = Vector3::One;				//�g�嗦
	Vector3 m_moveSpeed = Vector3::Zero;		//�ړ����x

	bool m_isExist = true;						//���݃t���O

	float m_speed = 0.0f;						//����
	float m_lifeTime = 0.0f;					//��������
	float m_scaleRate = 1.0f;					//�g�嗦(float��)
	float m_power = 0.0f;						//�_���[�W
	float m_damageArea = 0.0f;					//�_���[�W�̔������锼�a
	float m_damageInterval = 0.0f;				//�_���[�W�̔����Ԋu�B���̐��l�����G�l�~�[�ɖ��G���Ԃ�ݒ肷��B

};

