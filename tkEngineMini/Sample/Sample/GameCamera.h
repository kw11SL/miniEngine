#pragma once
class GameCamera
{
public:
	GameCamera(){}
	~GameCamera(){}

	/// @brief ����������
	/// @param maxMoveSpeed �J�����ƒ����_���ړ�����ō����x 
	void Init(const float maxMoveSpeed);
	
	/// @brief ���_���X�V
	/// @param pos ���_
	void SetCameraPosition(const Vector3& pos)
	{
		m_cameraPosition = pos;
		g_camera3D->SetPosition(m_cameraPosition);

		/*m_cameraPositionTarget = pos;
		g_camera3D->SetPosition(m_cameraPosition);*/

	}

	/// @brief ���_���ǂ���������W���X�V
	/// @param pos 
	void SetCameraPositionTarget(const Vector3& pos)
	{
		m_cameraPositionTarget = pos;
	}
	
	/// @brief �����_���X�V
	/// @param pos �����_
	void SetTargetPosition(const Vector3& pos)
	{
		m_targetPosition = pos;
		g_camera3D->SetTarget(m_targetPosition);
	}

	/// @brief �����_���ǂ���������W���X�V
	/// @param pos 
	void SetTargetPositionTarget(const Vector3& pos)
	{
		m_targetPositionTarget = pos;
	}

	void SetUpVectorTarget(const Vector3& upTarget)
	{
		m_upTarget = upTarget;
	}

	/// @brief �J�����̍��W���擾
	/// @return ���W
	const Vector3& GetCameraPosition() { return m_cameraPosition; }

	/// @brief �J�����ڕW�̍��W���擾
	/// @return 
	const Vector3& GetCameraPositionTarget() { return m_cameraPositionTarget; }
	
	/// @brief �����_�̍��W���擾
	/// @return ���W
	const Vector3& GetTargetPosition() { return m_targetPosition; }

	/// @brief �����_�ڕW�̍��W���擾
	/// @return 
	const Vector3& GetTargetPositionTarget() { return m_targetPositionTarget; }
	
	/// @brief ������̖ڕW�l���擾
	/// @return 
	const Vector3& GetUpVectorTarget() { return m_upTarget; }

	/// @brief �J�����̏�x�N�g����ݒ�B
	/// @param up 
	void SetUp(const Vector3& up);

	/// @brief �J�������W���J�����ڕW��ǂ������鏈��
	void ChaseCameraPosition();

	/// @brief �����_�������_�ڕW��ǂ������鏈��
	void ChaseTargetPosition();

	/// @brief �󂯎�����x�N�g�����J�����̏�̖ڕW�x�N�g���Ő��`�⊮���鏈��
	/// @param fracton ��ԗ�
	void LerpUpVector(float fracton , Vector3& up);


	/// @brief �΂˂̌��������΂˂��ۂ��ω������鏈��
	/// @param dampingRate			//������
	/// @param dampingRateTarget	//�������ڕW�l
	/// @param moveSpeed			//�΂˂̈ړ����x
	/// @return 
	float& CalcDampingRate(
		float dampingRateCurrent,
		float dampingRateTarget,
		float& moveSpeed
	);

	/// @brief �΂˂̌��������g���Ă΂˂��ۂ����W���X�V���鏈��
	/// @param pos			//���ۂ̍��W
	/// @param targetPos	//�ڕW�̍��W
	/// @param moveSpeed	//���x�x�N�g��
	/// @param maxMoveSpeed //�ō����x
	/// @param dampingRate	//�΂˂̌�����
	/// @return				//���W
	Vector3& CalcSpringVector(
		const Vector3& currentPos,
		const Vector3& targetPos,
		Vector3& moveSpeed,
		float maxMoveSpeed,
		float dampingRate
	);

	/// @brief �΂˃J�����̍X�V
	void UpdateSpringCamera();

	/// @brief �J�����̍X�V
	void UpdateCamera();

private:
	Vector3 m_cameraPosition = Vector3::Zero;		//�J�������W
	Vector3 m_targetPosition = Vector3::Zero;		//�����_
	
	Vector3 m_cameraPositionTarget = Vector3::Zero;	//�J�����ڕW
	Vector3 m_targetPositionTarget = Vector3::Zero;	//�����_�ڕW

	Vector3 m_cameraMoveSpeedVel = Vector3::Zero;	//�J�����̈ړ����x
	Vector3 m_targetMoveSpeedVel = Vector3::Zero;	//�����_�̈ړ����x

	Vector3 m_upTarget = Vector3::Zero;				//�J�����̏�̖ڕW

	Quaternion m_rotH = Quaternion::Identity;		//������]
	Quaternion m_rotV = Quaternion::Identity;		//������]

	float m_angleH = 0.0f;							//������]�p�x
	float m_angleV = 0.0f;							//������]�p�x

	float m_maxMoveSpeed = 0.0f;					//�J�����A�����_�̈ړ����x�̍ő�l

	float m_cameraMoveSpeed = 0.0f;					//�J�������J�����ڕW��ǂ������鑬�x
	float m_targetMoveSpeed = 0.0f;					//�����_�������_�ڕW��ǂ������鑬�x

	float m_dampingRate = 1.0f;						//�΂˂̌������B���W���ړ����鑬�x�����肷�邽�߂Ɏg�p
	float m_dampingRateTarget = 10.0f;				//�΂˂̌������̖ڕW�l
	float m_dampingRateVel = 0.0f;					//�΂ˌ��������ڕW�l�ɋ߂Â����x
};

