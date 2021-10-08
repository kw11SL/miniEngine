#pragma once
class Player_new;
class SkinModelRender;
class SpriteRender;
class BG;
class Direction;
class DirectionLight;
class PointLight;
class SpotLight;

class Game : public IGameObject
{
public:
	Game(){}
	~Game();
	bool Start() override;
	void Update() override;

	void Init(RenderingEngine& renderingEngine);

private:
	Player_new* m_player = nullptr;
	BG* m_bg = nullptr;
	//Direction* m_direction = nullptr;
	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;
	
	////�e�����p�̃��C�g�J����
	//Camera m_lightCamera;

	//�e�X�g
	SpriteRender* m_sprite = nullptr;
};

