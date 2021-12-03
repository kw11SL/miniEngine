#pragma once
#include "Level_New.h"

class Player_new;
class Enemy;
class SkinModelRender;
class SpriteRender;
class BG;
class DirectionLight;
class PointLight;
class SpotLight;
class UI;
class EnemyGenerator;
class Bullet;
class StageBackGround;

class Game : public IGameObject
{
public:
	Game(){}
	~Game();
	
	/// @brief 
	/// @return 
	bool Start() override;

	/// @brief 毎フレーム呼ばれる処理
	void Update() override;

	/// @brief 初期化処理
	/// @param renderingEngine レンダリングエンジン
	void Init(RenderingEngine& renderingEngine);

private:
	RenderingEngine* m_renderingEngine = nullptr;
	
	Player_new* m_player = nullptr;					//プレイヤー
	Enemy* m_enemy[3] = { nullptr };						//エネミー
	BG* m_bg = nullptr;								//ステージ
	StageBackGround* m_stageBackGround = nullptr;				//背景
	DirectionLight* m_directionLight = nullptr;		//ディレクションライト
	PointLight* m_pointLight = nullptr;				//ポイントライト
	SpotLight* m_spotLight = nullptr;				//スポットライト
	UI* m_ui = nullptr;								//UI
	EnemyGenerator* m_enemyGenerator[10] = {nullptr};		//エネミー生成器
	//テスト
	SpriteRender* m_sprite = nullptr;				//テスト表示スプライト
	
	
	Level_New m_level;

	float m_counter = 10.0f;
};

