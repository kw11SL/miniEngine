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

	SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }

private:
	SkinModelRender* m_skinModelRender = nullptr;
	PhysicsStaticObject m_physicsStaticObject;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
	float m_angle = 0.0f;
};

