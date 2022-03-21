#pragma once
class StageBackGround : public IGameObject
{
public:
	StageBackGround();
	~StageBackGround();

	bool Start() override;
	void Update() override;
	
	/// @brief ����������
	/// @param renderingEngine	�����_�����O�G���W�� 
	/// @param pos ���W
	/// @param rot ��]
	/// @param scale �g�嗦
	void Init(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

	/// @brief ���W��ݒ�
	/// @param pos 
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		m_skinModelRender->SetPosition(m_position);
	}

	/// @brief ��]��ݒ�
	/// @param qRot 
	void SetRotation(const Quaternion& qRot)
	{
		m_rotation = qRot;
		m_skinModelRender->SetRotation(m_rotation);
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale 
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_skinModelRender->SetScale(m_scale);
	}

	/// @brief �f�B���N�V�������C�g�̎󂯎��
	/// @param dirLight 
	void RecieveDirectionLight(DirectionLight* dirLight)
	{
		m_skinModelRender->InitDirectionLight(dirLight);
	}

	/// @brief �|�C���g���C�g�̎󂯎��
	void RecievePointLight(PointLight* ptLight)
	{
		m_skinModelRender->InitPointLight(ptLight);
	}

	/// @brief �X�|�b�g���C�g�̎󂯎��
	void RecieveSpotLight(SpotLight* spLight)
	{
		m_skinModelRender->InitSpotLight(spLight);
	}

	/// @brief ���f���̏�����
	void InitModelFromInitData()
	{
		m_skinModelRender->InitModel();
	}

	/// @brief ���f�������_�[�̎擾
	SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }

private:
	RenderingEngine* m_renderingEngine = nullptr;
	SkinModelRender* m_skinModelRender = nullptr;		//���f�������_�[
	Vector3 m_position = Vector3::Zero;					//���W
	Quaternion m_rotation = Quaternion::Identity;		//��]
	Vector3 m_scale = Vector3::One;						//�g�嗦
	float m_angle = 0.0f;								//�p�x

	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;
};

