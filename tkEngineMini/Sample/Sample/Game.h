#pragma once
class Player_new;
class Enemy;
class SkinModelRender;
class SpriteRender;
class BG;
class DirectionLight;
class PointLight;
class SpotLight;

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
	Player_new* m_player = nullptr;					//プレイヤー
	
	Enemy* m_enemy = nullptr;						//エネミー

	BG* m_bg = nullptr;								//背景
	DirectionLight* m_directionLight = nullptr;		//ディレクションライト
	PointLight* m_pointLight = nullptr;				//ポイントライト
	SpotLight* m_spotLight = nullptr;				//スポットライト

	//テスト
	SpriteRender* m_sprite = nullptr;				//テスト表示スプライト
};

