#pragma once
//#include "DirectionLight.h"
class Player_new;
//class Player;
class SkinModelRender;
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
	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	//SkinModelRender* m_skinModelRender = nullptr;
};

