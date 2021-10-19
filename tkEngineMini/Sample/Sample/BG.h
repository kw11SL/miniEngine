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
	SkinModelRender* m_skinModelRender = nullptr;		//モデルレンダー
	PhysicsStaticObject m_physicsStaticObject;			//静的物理オブジェクト

	Vector3 m_position = Vector3::Zero;					//座標
	Quaternion m_rotation = Quaternion::Identity;		//回転
	Vector3 m_scale = Vector3::One;						//拡大率
	float m_angle = 0.0f;								//角度
};

