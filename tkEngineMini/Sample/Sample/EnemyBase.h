#pragma once
class EnemyBase : public IGameObject
{
public:
	EnemyBase();
	virtual ~EnemyBase(){}

	bool Start() override
	{
		return true;
	}

	/// @brief ����������
	/// @param initPoint 
	/// @param initUp 
	void Init(const Vector3& initPoint, const Vector3& initUp);
	/// @brief �����������ɕt�����鏈��
	virtual void InitSub() {};


	//���ʂ��čs������

	/// @brief �X�V����
	void Update() override;
	/// @brief �X�V�����ɕt�����鏈��
	virtual void UpdateSub() {}

	/// @brief �ړ�����
	void Move();
	/// @brief �ړ������ɕt�����鏈��
	virtual void MoveSub() {}

	/// @brief ��]����
	void Rotation();
	/// @brief ��]�����ɕt�����鏈��
	virtual void RotationSub(){}

	/// @brief �����蔻�菈��
	void Hit();

	/// @brief ���G���Ԃ̌���
	void DecInvTime();

	/// @brief ���@�Ƃ̓����蔻�肪�L���ɂȂ�܂ł̃J�E���^�[������
	void DecToActivateTime();

	/// @brief ���j����
	void Destroy();

	/// @brief ���ŏ���
	void SelfDestroy();

	/// @brief �^�C���A�b�v���̎���
	void DestroyTimeUp();

	//�Q�b�^�[
	/// @brief	���W���擾 
	/// @return ���W
	const Vector3& GetPosition() const { return m_position; }

	/// @brief �g�嗦���擾
	/// @return �g�嗦
	const Vector3& GetScale() const { return m_scale; }

	/// @brief ��]�N�H�[�^�j�I�����擾
	/// @return ��]�N�H�[�^�j�I��
	const Quaternion& GetRotation() const { return m_rot; }

	/// @brief ��]�p�x���擾
	/// @return ��]�p�x
	const float GetAngle() const { return m_angle; }

	/// @brief �X�L�����f�������_�[���擾
	/// @return �X�L�����f�������_�[
	SkinModelRender* GetSkinModelRender(){	return m_skinModelRender; }

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
	void SetAngle(const float angle) { m_angle = angle; }

	/// @brief �v���C���[�ɑ΂��铖���蔻���L���ɂ��邩�ǂ���
	/// @param isActive 
	void SetActive(const bool isActive) { m_isActive = isActive; }

	/// @brief ���G���Ԃ�ݒ�
	/// @param invTime ���G����
	void SetInvincibleTime(const float invTime) { m_invTime = invTime; }

	//���C�g��n�����߂̊֐�
	/// @brief �f�B���N�V�������C�g���󂯂Ƃ�
	/// @param dirLight �f�B���N�V�������C�g
	void RecieveDirectionLight(DirectionLight* dirLight) { m_skinModelRender->InitDirectionLight(dirLight); }

	/// @brief �|�C���g���C�g���󂯎��
	/// @param ptLight �|�C���g���C�g
	void RecievePointLight(PointLight* ptLight) { m_skinModelRender->InitPointLight(ptLight); }

	/// @brief �X�|�b�g���C�g���󂯎��
	/// @param spLight �X�|�b�g���C�g
	void RecieveSpotLight(SpotLight* spLight) { m_skinModelRender->InitSpotLight(spLight); }

	/// @brief ���f���̏�����
	void InitModelFromInitData() { m_skinModelRender->InitModel(); }

protected:
	SkinModelRender* m_skinModelRender = nullptr;		//���f�������_�[
	MyCharacterController m_myCharaCon;					//���ʈړ��p�L�����R��
	SphericalMove m_sphericalMove;						//���ʈړ��p�N���X

	Player_new* m_player = nullptr;						//�v���C���[�ւ̃|�C���^

	//�p�����[�^�e��
	float m_life = 0.0f;								//�ϋv�l
	float m_lifeTime = 1.0f;							//���Ԏ���
	float m_speed = 0.0f;								//�ړ����x
	int m_score = 0;									//�G�l�~�[�̌��j�X�R�A
	float m_durability = 0.0f;							//�e�ւ̉e���l
	bool m_isExist = false;								//���݃t���O
	bool m_isInvincible = false;						//���G��Ԃ��ǂ���
	bool m_isActive = false;							//�v���C���[�ɑ΂��铖���蔻�肪�L�����ǂ���
	float m_angle = 0.0f;								//�p�x
	float m_invTime = 0.0f;								//���G����
	float m_toActivateCounter = 0.0f;					//�o�����Ă��瓖���蔻�肪�L���ɂȂ�܂ł̃J�E���^�[

	//���W�Ȃ�
	Vector3 m_position = Vector3::Zero;					//���W
	Vector3 m_moveSpeed = Vector3::Zero;				//���x�x�N�g��
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//���C���΂������x�N�g��
	Vector3 m_forward = Vector3::Zero;					//�O��
	Vector3 m_right = Vector3::Zero;					//�E
	Vector3 m_up = Vector3::Zero;						//��
	Vector3 m_scale = Vector3::One;						//�g�嗦
	Quaternion m_rot = Quaternion::Identity;			//��]
	
	//���C�g�e��ւ̃|�C���^
	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	//�G�t�F�N�g�e��
	//���j�G�t�F�N�g
	Effect m_destroyEffect;
	//�q�b�g�G�t�F�N�g
	Effect m_hitEffect;

};
