#pragma once
class UIOutline
{
public:
	UIOutline(){}
	~UIOutline();
	
	/// @brief 初期化処理
	void Init();

	/// @brief 更新処理
	void Update();

private:
	SpriteRender* m_outlineSprite = nullptr;
	Vector2 m_position = Vector2::Zero;
};

