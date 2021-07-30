#pragma once

struct SPointLight
{
	Vector3 position = Vector3::Zero;		//座標
	float pad0 = 0.0f;						//パディング
	Vector3 color = Vector3::One;			//カラー
	float ptRange = 0.0f;					//影響範囲
};

struct PtLight
{
	SPointLight pointLight;					//ポイントライトの構造体
};

class PointLight : public IGameObject
{
public:
	PointLight() {}
	~PointLight() {}
	bool Start() override;
	void Update() override;

	void Init(const Vector3& position,const Vector3& color, float range);
	void Move();

	//セッター
	void SetPosition(const Vector3& position);
	void SetColor(const Vector3& color);
	void SetRange(float range);

	//ゲッター
	const Vector3& GetPosition() { return m_position; }
	const Vector3& GetColor() { return m_color; }
	const float& GetRange() { return m_range; }
	const PtLight& GetPointLight() { return m_ptLight; }
	PtLight* GetPointLightAddress() { return &m_ptLight; }
	
private:
	PtLight m_ptLight;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_color = Vector3::One;
	float m_range = 0.0f;
	
};

