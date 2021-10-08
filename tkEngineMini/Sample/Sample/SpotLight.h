#pragma once

class SkinModelRender;

struct SSpotLight
{
	Vector3 position = Vector3::Zero;			//座標
	float pad0 = 0.0f;							//パディング
	Vector3 color = Vector3::One;				//カラー
	float spRange = 0.0f;						//影響範囲
	Vector3 spDirection = Vector3::Zero;		//射出方向
	float spAngle = 0.0f;						//射出角度
};

struct SpLight 
{
	SSpotLight spotLight;						//スポットライトの構造体
};

class SpotLight : public IGameObject
{
public:
	SpotLight() {}
	~SpotLight() {}
	bool Start() override;
	void Update() override;

	void Move();
	void Rotation();

	void Init(const Vector3& position, const Vector3& color, const float& range, const Vector3& direction, const float& angle);

	//todo 組み込み型の引数を値渡しにする
	//→組み込み型の引数は値渡しにした方が速い
	//→組み込み型以外はconst参照にした方がよい
	
	//セッター
	void SetPosition(const Vector3& position);
	void SetRotation(const Quaternion& qRot);
	void SetColor(const Vector3& color);
	void SetRange(float range);
	void SetDirection(const Vector3& direction);
	void SetAngle(float angle);

	//ゲッター
	const Vector3& GetPosition() { return m_position; }
	const Quaternion& GetRotation() { return m_qRot; }
	const Vector3& GetColor() { return m_color; }
	const float& GetRange() { return m_range; }
	const Vector3& GetDirection() { return m_direction; }
	const float& GetAngle() { return m_angle; }
	const SpLight& GetSpotLight() { return m_spLight; }
	SpLight* GetSpotLightAddress() { return &m_spLight; }

private:
	SpLight m_spLight;								//スポットライト構造体
	
	Vector3 m_position = Vector3::Zero;
	Quaternion m_qRot = Quaternion::Identity;
	Vector3 m_color = Vector3::One;
	float m_range = 0.0f;
	Vector3 m_direction = Vector3::Zero;
	float m_angle = 0.0f;

	Quaternion m_rotationH = Quaternion::Identity;
	Quaternion m_rotationV = Quaternion::Identity;
};


