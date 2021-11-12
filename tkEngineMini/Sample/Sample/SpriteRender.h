#pragma once

class Sprite;


class SpriteRender : public IGameObject
{
public:
	SpriteRender() {};
	~SpriteRender();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const char* spriteFilePath,const UINT& width,const UINT& height,AlphaBlendMode alphaBrendMode);
	void InitShader(const char* fxFilePath, const char* vsEntryPoint,const char* psEntryPoint);

	//セッター
	void SetPosition(const Vector3& pos);
	void SetRotation(const Quaternion& rot);
	void SetScale(const Vector3& scale);
	void SetPivot(const Vector2& pivot);
	void SetColor(const Vector4& color);

	//ゲッター
	const Vector3& GetPosition() { return m_position; }
	const Quaternion& GetRotation() { return m_rotation; }
	const Vector3& GetScale() { return m_scale; }
	const Vector2& GetPivot() { return m_pivot; }
	const Vector4& GetColor() { return m_color; }

private:
	RenderingEngine* m_renderingEngine = nullptr;
	Sprite m_sprite;
	SpriteInitData m_spriteInitData;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
	Vector2 m_pivot = { 0.5f,0.5f };
	Vector4 m_color = {1.0f,1.0f,1.0f,1.0f};

};

