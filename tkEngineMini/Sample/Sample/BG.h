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
	SkinModelRender* m_skinModelRender = nullptr;		//モデルレンダー
	PhysicsStaticObject m_physicsStaticObject;			//静的物理オブジェクト

	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	Vector3 m_position = Vector3::Zero;					//座標
	Quaternion m_rotation = Quaternion::Identity;		//回転
	Vector3 m_scale = Vector3::One;						//拡大率
	float m_angle = 0.0f;								//角度
};

