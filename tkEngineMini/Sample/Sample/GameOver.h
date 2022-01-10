#pragma once
class GameOver
{
public:
	GameOver();
	~GameOver();

	void Init();
	void Update();

private:
	SpriteRender* m_gameOverSprite = nullptr;
	SpriteRender* m_fadeOutSprite = nullptr;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Vector4 m_color = { 1.0f,1.0f,1.0f,0.0f };
	Vector2 m_pivot = { 0.5f,0.5f };

};

