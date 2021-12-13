#pragma once
class Life
{
public:
	Life();
	~Life();
	
	/// @brief ���������� 
	void Init();

	/// @brief �X�V����
	void Update();

private:
	SpriteRender* m_lifeTextSprite = nullptr;
	Vector3 m_lifeTextSpritePos = Vector3::Zero;

	SpriteRender* m_lifeIconSprite[3] = { nullptr };
	Vector3 m_lifeIconSpritePos[3];


};

