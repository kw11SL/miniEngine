#pragma once
//#include "DirectionLight.h"
class SkinModelRender;
class DirectionLight;
class PointLight;

class Player_new : public IGameObject
{
public:
	Player_new();
	~Player_new();
	bool Start() override;
	void Update() override;
	void Move();
	void Rotation();

	//ゲッター
	Vector3 GetPosition() { return m_position; }
	Vector3 GetScale() { return m_scale; }
	Quaternion GetRotation() { return m_rot; }
	float GetAngle() { return m_angle; }
	bool GetIsInitDirLig() { return m_isInitDirLig; }

	//セッター
	void SetPostion(const Vector3& pos);
	void SetScale(const Vector3& scale);
	void SetRotation(const Quaternion rot);
	void SetAngle(const float& angle);
	void SetIsInitDirLig(const bool& isInit) { m_isInitDirLig = isInit; }


	void Init();
	//ライトを渡すための関数
	void InitDirectionLight(DirectionLight* dirLight);
	void InitPointLight(PointLight* ptLight);
	SkinModelRender* GetSkinModelRender();

private:
	SkinModelRender* m_skinModelRender = nullptr;
	CharacterController m_charaCon;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rot = Quaternion::Identity;
	float m_angle = 0.0f;
	bool m_isInitDirLig = false;

};

