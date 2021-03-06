#pragma once

class Sprite;


class SpriteRender : public IGameObject
{
public:
	SpriteRender() {};
	~SpriteRender();
	
	/// @brief 開始処理 
	/// @return trueを返すとUpdateが呼ばれるようになる
	bool Start() override;
	
	/// @brief 更新処理
	void Update() override;

	//描画処理
	void Render(RenderContext& rc) override;
	/// @brief 初期化処理
	/// @param spriteFilePath	使用するddsのファイルパス 
	/// @param width 幅
	/// @param height 高さ
	/// @param alphaBrendMode 半透明ブレンドモード
	/// @param isDraw3D 3D空間に描画するかどうか
	void Init(const char* spriteFilePath,const UINT& width,const UINT& height,AlphaBlendMode alphaBrendMode,bool isDraw3D = false);
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

	/// @brief フェードインする処理
	/// @param addRate 加算割合
	void FadeIn(const float addRate);

	/// @brief フェードアウトする処理
	/// @param decRate 減算割合
	void FadeOut(const float decRate);

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