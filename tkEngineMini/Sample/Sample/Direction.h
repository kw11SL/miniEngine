#pragma once

class SkinModelRender;
class DirectionLight;
class PointLight;
class SpotLight;

class Direction : public IGameObject
{
public:
	Direction();
	~Direction();

	bool Start() override;
	void Update() override;

	void Move();
	void Rotation();

	//セッター
	void SetPosition(const Vector3& pos);
	void SetRotation(const Quaternion& qRot);
	void SetAngle(const float angle);
	void SetScale(const Vector3& scale);

	//ゲッター
	const Vector3& GetPosition() { return m_position; }
	const Quaternion& GetRotation() { return m_rotation; }
	const float& GetAngle() { return m_angle; }
	const Vector3& GetScale() { return m_scale; }
	SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }

	//ライト受け取り
	void RecieveDirectionLight(DirectionLight* dirLight);
	void RecievePointLight(PointLight* ptLight);
	void RecieveSpotLight(SpotLight* spLight);

private:
	Vector3 m_position = Vector3::Zero;
	float m_angle = 0.0f;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	SkinModelRender* m_skinModelRender = nullptr;
};

