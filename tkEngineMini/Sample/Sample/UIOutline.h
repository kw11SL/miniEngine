#pragma once
class UIOutline
{
public:
	UIOutline(){}
	~UIOutline();
	
	/// @brief ����������
	void Init();

	/// @brief �X�V����
	void Update();

private:
	SpriteRender* m_outlineSprite = nullptr;
	Vector2 m_position = Vector2::Zero;
};

