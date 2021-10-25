#pragma once

class Player_new : public IGameObject
{
public:
	Player_new(){}
	~Player_new();

	//ゲッター
	/// @brief	座標を取得 
	/// @return 座標
	Vector3 GetPosition() 
	{ 
		return m_position; 
	}
	
	/// @brief 拡大率を取得
	/// @return 拡大率
	Vector3 GetScale() 
	{ 
		return m_scale; 
	}
	
	/// @brief 回転を取得
	/// @return 回転
	Quaternion GetRotation() 
	{ 
		return m_rot; 
	}
	
	/// @brief 回転角度を取得
	/// @return 回転角度
	float GetAngle() 
	{ 
		return m_angle; 
	}

	/// @brief スキンモデルレンダーを取得
	/// @return スキンモデルレンダー
	SkinModelRender* GetSkinModelRender()
	{
		return m_skinModelRender;
	}

	//セッター
	/// @brief	座標を設定 
	/// @param pos 座標
	void SetPostion(const Vector3& pos)
	{
		m_position = pos;
		m_skinModelRender->SetPosition(m_position);
	}

	/// @brief 拡大率を設定
	/// @param scale 拡大率
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_skinModelRender->SetScale(m_scale);
	}

	/// @brief 回転を設定
	/// @param rot	回転
	void SetRotation(const Quaternion rot)
	{
		m_rot = rot;
	}
	
	/// @brief 角度を設定
	/// @param angle 角度
	void SetAngle(const float& angle)
	{
		m_angle = angle;
	}
	
	/// @brief 初期化処理
	/// @param renderingEngine　レンダリングエンジン 
	void Init(RenderingEngine& renderingEngine);

	//ライトを渡すための関数
	
	/// @brief ディレクションライトを受けとる
	/// @param dirLight ディレクションライト
	void RecieveDirectionLight(DirectionLight* dirLight)
	{
		m_skinModelRender->InitDirectionLight(dirLight);
	}
	
	/// @brief ポイントライトを受け取る
	/// @param ptLight ポイントライト
	void RecievePointLight(PointLight* ptLight)
	{
		m_skinModelRender->InitPointLight(ptLight);
	}
	
	/// @brief スポットライトを受け取る
	/// @param spLight スポットライト
	void RecieveSpotLight(SpotLight* spLight)
	{
		m_skinModelRender->InitSpotLight(spLight);
	}

	/// @brief モデルの初期化
	void InitModelFromInitData()
	{
		m_skinModelRender->InitModel();
	}

private:
	//内部で使う処理

	bool Start() override;

	void Update() override;

	/// @brief 移動処理
	void Move();

	/// @brief 回転処理
	void Rotation();

	void RotateShotDirection();

	/// @brief 弾を発射
	void FireBullet();

private:

	SkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダー
	MyCharacterController m_myCharaCon;					//自作のキャラクターコントローラ
	SphericalMove m_sphericalMove;						//球面移動用クラス
	GameCamera m_gameCamera;							//ゲームカメラ
	//Vector3 m_cameraPoint = Vector3::Zero;				//カメラが追いかけるポイント
	Bullet* m_bullet = nullptr;							//弾クラス

	Vector3 m_position = Vector3::Zero;					//座標
	Vector3 m_moveSpeed = Vector3::Zero;				//速度ベクトル
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//レイを飛ばす方向ベクトル
	Vector3 m_forward = Vector3::Zero;					//前方
	Vector3 m_right	= Vector3::Zero;					//右
	Vector3 m_up = Vector3::Zero;						//上
	Vector3 m_scale = Vector3::One;						//拡大率
	Quaternion m_rot = Quaternion::Identity;			//回転
	Quaternion m_rotUpToGroundNormal = Quaternion::Identity;					//プレイヤーの上ベクトルを地面の法線に向かせる回転クォータニオン
	float m_angle = 0.0f;								//回転角度

	float m_fireCounter = 0.0f;							//発射間隔カウンタ
	Vector3 m_shotDirection = Vector3::Zero;			//ショットを撃つ方向

};

