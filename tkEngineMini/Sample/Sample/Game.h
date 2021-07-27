#pragma once
class Player_new;
class SkinModelRender;
class BG;
class DirectionLight;
class PointLight;

class Game : public IGameObject
{
public:
	Game(){}
	~Game();
	bool Start() override;
	void Update() override;
private:
	//Player* m_player;
	Player_new* m_player = nullptr;
	BG* m_bg = nullptr;
	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	//SkinModelRender* m_skinModelRender = nullptr;
};

