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
	DirectionLight(){}
	~DirectionLight(){}
	bool Start() override;
	void Update() override;

	void Init(const Vector3& direction,const Vector3& color,const Vector3& ambLig);
	void Rotation();

	//セッター
	void SetDirection(const Vector3& direction);
	void SetRotationH(const Quaternion& rotH);
	void SetRotationV(const Quaternion& rotV);
	void SetAngleH(float angleH) { m_angleH = angleH; }
	void SetAngleV(float angleV) { m_angleV = angleV; }
	void SetColor(const Vector3& color);
	void SetAmbColor(const Vector3& ambColor);
	void SetEyePos(const Vector3& pos);

	//ゲッター
	const Vector3& GetDirection() { return m_dirLight.directionLight.direction; }
	const Quaternion& GetRotationH() { return m_rotationH; }
	const Quaternion& GetRotationV() { return m_rotationV; }
	const float& GetAngleH() { return m_angleH; }
	const float& GetAngleV() { return m_angleV; }
	const Vector3& GetColor() { return m_dirLight.directionLight.color; }
	
	/// @brief 線形補完で明るさを上げていく処理
	/// @param addRate 上昇補間率
	void FadeIn(const float addRate);

	/// @brief 線形補完で明るさを下げていく処理
	/// @param decRate 下降補間率
	void FadeOut(const float decRate);

	/// @brief ライト構造体を返す関数
	/// @return m_light
	const DirLight& GetDirLight() { return m_dirLight; }
	
	/// @brief ライトの構造体のアドレスを返す関数
	/// @return m_lightのアドレス値
	DirLight* GetDirLightAddress() { return &m_dirLight; }

private:
	DirLight m_dirLight;
	Vector3 m_direction = Vector3::Zero;
	Quaternion m_rotationH = Quaternion::Identity;
	Quaternion m_rotationV = Quaternion::Identity;
	Vector3 m_color = Vector3::One;
	Vector3 m_colorAmb = Vector3::One;
	Vector3 m_eyePos = Vector3::Zero;
	Vector3 m_ambientLig = Vector3::Zero;

	float m_colorRate = 0.0f;
	float m_colorAmbRate = 0.0f;

	float m_angleH = 0.0f;
	float m_angleV = 0.0f;
};