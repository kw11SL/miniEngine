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

	//Player_new* m_player2 = nullptr;					//プレイヤー


	BG* m_bg = nullptr;								//背景
	//Direction* m_direction = nullptr;
	DirectionLight* m_directionLight = nullptr;		//ディレクションライト
	PointLight* m_pointLight = nullptr;				//ポイントライト
	SpotLight* m_spotLight = nullptr;				//スポットライト
	
	////影生成用のライトカメラ
	//Camera m_lightCamera;

	//テスト
	SpriteRender* m_sprite = nullptr;				//テスト表示スプライト
};

