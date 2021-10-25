#pragma once
class GameCamera : public IGameObject
{
public:
	GameCamera(){}
	~GameCamera(){}

	bool Start()override;
	void Update()override;

	/// @brief ����������
	void Init();
	
	/// @brief ���_���X�V
	/// @param pos ���_
	void SetCameraPosition(const Vector3& pos);

	void SetCameraPositionTarget(const Vector3& pos);
	
	/// @brief �����_���X�V
	/// @param pos �����_
	void SetTargetPosition(const Vector3& pos);

	/// @brief ���_�̍��W���擾
	/// @return ���W
	const Vector3& GetCameraPosition() { return m_cameraPosition; }

	const Vector3& GetCameraPositionTarget() { return m_cameraPositionTarget; }
	
	/// @brief �����_�̍��W���擾
	/// @return ���W
	const Vector3& GetTargetPosition() { return m_targetPosition; }
	
	/// @brief �J�����̏�x�N�g����ݒ�B
	/// @param up 
	void SetUp(const Vector3& up);

	void Chase();

private:
	Vector3 m_cameraPosition = Vector3::Zero;				//���_
	Vector3 m_targetPosition = Vector3::Zero;				//�����_
	Vector3 m_cameraPositionTarget = Vector3::Zero;			//�n�_���ǂ���������W

	Quaternion m_rotH = Quaternion::Identity;				//������]
	Quaternion m_rotV = Quaternion::Identity;				//������]

	float m_angleH = 0.0f;									//������]�p�x
	float m_angleV = 0.0f;									//������]�p�x

	float m_moveSpeed = 2.0f;

};

