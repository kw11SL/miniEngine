#pragma once
struct SSpotLight
{
	Vector3 position = Vector3::Zero;			//���W
	float pad0 = 0.0f;							//�p�f�B���O
	Vector3 color = Vector3::One;				//�J���[
	float spRange = 0.0f;						//�e���͈�
	Vector3 spDirection = Vector3::Zero;		//�ˏo����
	float spAngle = 0.0f;						//�ˏo�p�x
};

struct SpLight 
{
	SSpotLight spotLight;						//�X�|�b�g���C�g�̍\����
};

class SpotLight : public IGameObject
{
public:
	SpotLight() {};
	~SpotLight() {};
	bool Start() override;
	void Update() override;

	void Move();
	void Rotation();

	void Init(const Vector3& position, const Vector3& color, const float& range, const Vector3& direction, const float& angle);

	//todo �g�ݍ��݌^�̈�����l�n���ɂ���
	//���g�ݍ��݌^�̈����͒l�n���ɂ�����������
	//���g�ݍ��݌^�ȊO��const�Q�Ƃɂ��������悢
	
	//�Z�b�^�[
	void SetPosition(const Vector3& position);
	void SetColor(const Vector3& color);
	void SetRange(float range);
	void SetDirection(const Vector3& direction);
	void SetAngle(float angle);

	//�Q�b�^�[
	const Vector3& GetPosition() { return m_position; }
	const Vector3& GetColor() { return m_color; }
	const float& GetRange() { return m_range; }
	const Vector3& GetDirection() { return m_direction; }
	const float& GetAngle() { return m_angle; }
	const SpLight& GetSpotLight() { return m_spLight; }
	SpLight* GetSpotLightAddress() { return &m_spLight; }

private:
	SpLight m_spLight;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_color = Vector3::One;
	float m_range = 0.0f;
	Vector3 m_direction = Vector3::Zero;
	float m_angle = 0.0f;
};
