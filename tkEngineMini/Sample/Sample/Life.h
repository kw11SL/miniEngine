#pragma once
class Life
{
public:
	Life();
	~Life();
	
	/// @brief 初期化処理 
	void Init();

	/// @brief ライフアイコンの表示、非表示
	void LifeDisp();

	/// @brief 更新処理
	void Update();

private:
	SpriteRender* m_lifeTextSprite = nullptr;
	Vector3 m_lifeTextSpritePos = Vector3::Zero;

	SpriteRender* m_lifeIconSprite[3] = { nullptr };
	Vector3 m_lifeIconSpritePos[3];


};

