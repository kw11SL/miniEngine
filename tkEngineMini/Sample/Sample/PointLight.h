#pragma once

struct SPointLight
{
	Vector3 position = Vector3::Zero;		//���W
	float pad0 = 0.0f;						//�p�f�B���O
	Vector3 color = Vector3::One;			//�J���[
	float ptRange = 0.0f;					//�e���͈�
};

struct PtLight
{
	SPointLight pointLight;					//�|�C���g���C�g�̍\����
};

class PointLight : public IGameObject
{
public:
	PointLight() {}
	~PointLight() {}
	bool Start() override;
	void Update() override;

	void Init(const Vector3& position,const Vector3& color, float range);
	void Move();

	//�Z�b�^�[
	void SetPosition(const Vector3& position);
	void SetColor(const Vector3& color);
	void SetRange(float range);

	//�Q�b�^�[
	const Vector3& GetPosition() { return m_position; }
	const Vector3& GetColor() { return m_color; }
	const float& GetRange() { return m_range; }
	const PtLight& GetPointLight() { return m_ptLight; }
	PtLight* GetPointLightAddress() { return &m_ptLight; }
	
private:
	PtLight m_ptLight;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_color = Vector3::One;
	float m_range = 0.0f;
	
};

