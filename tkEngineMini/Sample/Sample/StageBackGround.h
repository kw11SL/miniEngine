#pragma once
class StageBackGround : public IGameObject
{
public:
	StageBackGround();
	~StageBackGround();

	bool Start() override;
	void Update() override;
	void Init(RenderingEngine& renderingEngine, const Vector3& pos, const Quaternion& rot, const Vector3& scale);

	void RecieveDirectionLight(DirectionLight* dirLight);
	void RecievePointLight(PointLight* ptLight);
	void RecieveSpotLight(SpotLight* spLight);
	void InitModelFromInitData();

	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		m_skinModelRender->SetPosition(m_position);
	}

	void SetRotation(const Quaternion& qRot)
	{
		m_rotation = qRot;
		m_skinModelRender->SetRotation(m_rotation);
	}

	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_skinModelRender->SetScale(m_scale);
	}

	SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }

private:
	SkinModelRender* m_skinModelRender = nullptr;		//モデルレンダー

	Vector3 m_position = Vector3::Zero;					//座標
	Quaternion m_rotation = Quaternion::Identity;		//回転
	Vector3 m_scale = Vector3::One;						//拡大率
	float m_angle = 0.0f;								//角度
};

