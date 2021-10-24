#pragma once

class Player_new;

class Enemy : public IGameObject
{
public:
	Enemy(){}
	~Enemy();
	
	/// @brief 初期化処理
	/// @param renderingEngine　レンダリングエンジン 
	void Init(RenderingEngine& renderingEngine,  const Vector3& initPoint);

	//ゲッター
	/// @brief	座標を取得 
	/// @return 座標
	Vector3 GetPosition() { return m_position; }

	/// @brief 拡大率を取得
	/// @return 拡大率
	Vector3 GetScale() { return m_scale; }

	/// @brief 回転を取得
	/// @return 回転
	Quaternion GetRotation() { return m_rot; }

	/// @brief 回転角度を取得
	/// @return 回転角度
	float GetAngle() { return m_angle; }

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
		m_skinModelRender->SetRotation(m_rot);
	}

	/// @brief 角度を設定
	/// @param angle 角度
	void SetAngle(const float& angle) 
	{
		m_angle = angle;
	}

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
	bool Start() override;

	void Update() override;

	/// @brief 移動処理
	void Move();

	/// @brief 回転処理
	void Rotation();


private:
	
	/// @brief エネミーのタイプ
	enum EnEnemyType
	{
		enCommon,
		enPowered,
		enChaser,
		enBomb,
		enTypeNum
	};

	SkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダー
	MyCharacterController m_myCharaCon;					//自作のキャラクターコントローラ
	SphericalMove m_sphericalMove;						//球面移動用クラス
	EnEnemyType m_enEnemyType = enCommon;				//エネミーのタイプ

	Player_new* m_player = nullptr;

	float m_life = 0.0f;								//耐久値
	float m_speed = 1.0f;							//移動速度

	Vector3 m_position = Vector3::Zero;					//座標
	Vector3 m_moveSpeed = Vector3::Zero;				//速度ベクトル
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//レイを飛ばす方向ベクトル
	Vector3 m_forward = Vector3::Zero;					//前方
	Vector3 m_right = Vector3::Zero;					//右
	Vector3 m_up = Vector3::Zero;						//上
	Vector3 m_scale = Vector3::One;						//拡大率
	Quaternion m_rot = Quaternion::Identity;			//回転
	float m_angle = 0.0f;								//角度
};

