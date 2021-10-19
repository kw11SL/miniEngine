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
	void Init(RenderingEngine& renderingEngine);

	void RecieveDirectionLight(DirectionLight* dirLight);
	void RecievePointLight(PointLight* ptLight);
	void RecieveSpotLight(SpotLight* spLight);


	void InitModelFromInitData();

	SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }

private:
	SkinModelRender* m_skinModelRender = nullptr;		//���f�������_�[
	PhysicsStaticObject m_physicsStaticObject;			//�ÓI�����I�u�W�F�N�g

	Vector3 m_position = Vector3::Zero;					//���W
	Quaternion m_rotation = Quaternion::Identity;		//��]
	Vector3 m_scale = Vector3::One;						//�g�嗦
	float m_angle = 0.0f;								//�p�x
};

