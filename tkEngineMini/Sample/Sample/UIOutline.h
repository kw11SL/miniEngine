#pragma once
class UIOutline
{
public:
	UIOutline(){}
	~UIOutline();
	
	/// @brief ‰Šú‰»ˆ—
	void Init();

	/// @brief XVˆ—
	void Update();

private:
	SpriteRender* m_outlineSprite = nullptr;
	Vector2 m_position = Vector2::Zero;
};

