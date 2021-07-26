#pragma once

struct SDirectionLight
{
	Vector3 direction = Vector3::Zero;		//12				ライトの方向
	float pad0 = 0.0f;						//4(12 + 4 = 16)	パディング
	Vector3 color = Vector3::Zero;			//12				ライトカラー
	float pad1 = 0.0f;						//4(12 + 4 = 16)	パディング
};

struct DirLight
{
	SDirectionLight directionLight;			//ディレクションライトの構造体
	Vector3 eyePos = Vector3::Zero;			//視点のベクトル
	float pad0 = 0.0f;
	Vector3 ambientLight = Vector3::Zero;	//アンビエントライト
	float pad1 = 0.0f;
};

class DirectionLight : public IGameObject
{
public:
	DirectionLight();
	~DirectionLight();
	bool Start() override;
	void Update() override;

	void Init(const Vector3& direction,const Vector3& color,const Vector3& ambLig);
	void Rotation();


	void SetDirection(const Vector3& direction);
	Vector3 GetDirection() { return m_light.directionLight.direction; }
	
	void SetRotationH(const Quaternion& rotH);
	Quaternion GetRotationH() { return m_rotationH; }
	
	void SetRotationV(const Quaternion& rotV);
	Quaternion GetRotationV() { return m_rotationV; }
	
	void SetAngleH(const float& angleH) { m_angleH = angleH; }
	float GetAngleH() { return m_angleH; }
	
	void SetAngleV(const float& angleV) { m_angleV = angleV; }
	float GetAngleV() { return m_angleV; }

	void SetColor(const Vector3& color);
	Vector3 GetColor() { return m_light.directionLight.color; }
	
	void SetEyePos(const Vector3& pos);
	
	/// @brief ライト構造体を返す関数
	/// @return m_light
	DirLight GetDirLight() { return m_light; }
	
	/// @brief ライトの構造体のアドレスを返す関数
	/// @return m_lightのアドレス値
	DirLight* GetDirLightAddress() { return &m_light; }

private:
	DirLight m_light;
	Vector3 m_direction = Vector3::Zero;
	Quaternion m_rotationH = Quaternion::Identity;
	Quaternion m_rotationV = Quaternion::Identity;
	Vector3 m_color = Vector3::One;
	Vector3 m_eyePos = Vector3::Zero;
	Vector3 m_ambientLig = Vector3::Zero;
	float m_angleH = 0.0f;
	float m_angleV = 0.0f;
};