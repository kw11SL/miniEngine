#pragma once

class SkinModelRender;
class DirectionLight;
class PointLight;
class SpotLight;

class BG : public IGameObject
{
public:
	BG();
	~BG();
	bool Start() override;
	void Update() override;
	void Init(const Vector3& pos,const Quaternion& rot,const Vector3& scale);

	void RecieveDirectionLight(DirectionLight* dirLight);
	void RecievePointLight(PointLight* ptLight);
	void RecieveSpotLight(SpotLight* spLight);
	void InitModelFromInitData();

	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	void SetRotation(const Quaternion& qRot)
	{
		m_rotation = qRot;
	}

	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	SkinModelRender* GetSkinModelRender()
	{ 
		return m_skinModelRender; 
	}

private:
	SkinModelRender* m_skinModelRender = nullptr;		//���f�������_�[
	PhysicsStaticObject m_physicsStaticObject;			//�ÓI�����I�u�W�F�N�g

	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	Vector3 m_position = Vector3::Zero;					//���W
	Quaternion m_rotation = Quaternion::Identity;		//��]
	Vector3 m_scale = Vector3::One;						//�g�嗦
	float m_angle = 0.0f;								//�p�x
};

