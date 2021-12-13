#pragma once
class Life
{
public:
	Life();
	~Life();
	
	/// @brief ‰Šú‰»ˆ— 
	void Init();

	/// @brief XVˆ—
	void Update();

private:
	SpriteRender* m_lifeTextSprite = nullptr;
	Vector3 m_lifeTextSpritePos = Vector3::Zero;

	SpriteRender* m_lifeIconSprite[3] = { nullptr };
	Vector3 m_lifeIconSpritePos[3];


};

