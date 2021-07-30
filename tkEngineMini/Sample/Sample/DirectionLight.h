#pragma once

struct SDirectionLight
{
	Vector3 direction = Vector3::Zero;		//12				���C�g�̕���
	float pad0 = 0.0f;						//4(12 + 4 = 16)	�p�f�B���O
	Vector3 color = Vector3::Zero;			//12				���C�g�J���[
	float pad1 = 0.0f;						//4(12 + 4 = 16)	�p�f�B���O
};

struct DirLight
{
	SDirectionLight directionLight;			//�f�B���N�V�������C�g�̍\����
	Vector3 eyePos = Vector3::Zero;			//���_�̃x�N�g��
	float pad0 = 0.0f;
	Vector3 ambientLight = Vector3::Zero;	//�A���r�G���g���C�g
	float pad1 = 0.0f;
};

class DirectionLight : public IGameObject
{
public:
	DirectionLight();
	~DirectionLight();
	bool Start() override;
	void Update() override;

	void Init(const Vector3& direction,const Vector3& color,const Vector3& ambLig);
	void Rotation();

	//�Z�b�^�[
	void SetDirection(const Vector3& direction);
	void SetRotationH(const Quaternion& rotH);
	void SetRotationV(const Quaternion& rotV);
	void SetAngleH(float angleH) { m_angleH = angleH; }
	void SetAngleV(float angleV) { m_angleV = angleV; }
	void SetColor(const Vector3& color);
	void SetEyePos(const Vector3& pos);

	//�Q�b�^�[
	const Vector3& GetDirection() { return m_light.directionLight.direction; }
	const Quaternion& GetRotationH() { return m_rotationH; }
	const Quaternion& GetRotationV() { return m_rotationV; }
	const float& GetAngleH() { return m_angleH; }
	const float& GetAngleV() { return m_angleV; }
	const Vector3& GetColor() { return m_light.directionLight.color; }
	
	/// @brief ���C�g�\���̂�Ԃ��֐�
	/// @return m_light
	const DirLight& GetDirLight() { return m_light; }
	
	/// @brief ���C�g�̍\���̂̃A�h���X��Ԃ��֐�
	/// @return m_light�̃A�h���X�l
	DirLight* GetDirLightAddress() { return &m_light; }

private:
	DirLight m_light;
	Vector3 m_direction = Vector3::Zero;
	Quaternion m_rotationH = Quaternion::Identity;
	Quaternion m_rotationV = Quaternion::Identity;
	Vector3 m_color = Vector3::One;
	Vector3 m_eyePos = Vector3::Zero;
	Vector3 m_ambientLig = Vector3::Zero;
	float m_angleH = 0.0f;
	float m_angleV = 0.0f;
};