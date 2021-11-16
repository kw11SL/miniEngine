#pragma once

class Player_new;

/// @brief エネミーのタイプ
enum EnEnemyType
{
	enCommon,
	enPowered,
	enChaser,
	enBomb,
	enShot,
	enEnemyTypeNum
};

class Enemy : public IGameObject
{
private:
	

public:
	Enemy();
	~Enemy();
	
	/// @brief 
	/// @param renderingEngine 
	/// @param initPoint 
	/// @param initUp 
	/// @param enemyType 
	void Init(
		RenderingEngine& renderingEngine,
		const Vector3& initPoint,
		const Vector3& initUp,
		const EnEnemyType& enemyType);

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

	/// @brief エネミーの種類を設定
	/// @param type 
	void SetType(const EnEnemyType& type)
	{
		m_enEnemyType = type;
	}

	/// @brief エネミーの種類を取得
	/// @return 
	EnEnemyType& GetType()
	{
		return m_enEnemyType;
	}

	/// @brief エネミーの撃破スコアを取得
	/// @return 
	int GetScore()
	{
		return m_score;
	}

	//無敵時間を設定
	void SetInvincibleTime(const float invTime)
	{
		m_invTime = invTime;
	}

private:
	bool Start() override;

	void Update() override;

	/// @brief 移動処理
	void Move();

	/// @brief 回転処理
	void Rotation();

	/// @brief ヒット処理
	void Hit();

	/// @brief 撃破処理
	void Destroy();

private:
	SkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダー
	MyCharacterController m_myCharaCon;					//自作のキャラクターコントローラ
	SphericalMove m_sphericalMove;						//球面移動用クラス
	EnEnemyType m_enEnemyType = enCommon;				//エネミーのタイプ

	Player_new* m_player = nullptr;

	float m_life = 0.0f;								//耐久値
	float m_speed = 0.0f;								//移動速度
	int m_score = 0;									//エネミーの撃破スコア
	bool m_exist = false;								//存在フラグ

	Vector3 m_position = Vector3::Zero;					//座標
	Vector3 m_moveSpeed = Vector3::Zero;				//速度ベクトル
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//レイを飛ばす方向ベクトル
	Vector3 m_forward = Vector3::Zero;					//前方
	Vector3 m_right = Vector3::Zero;					//右
	Vector3 m_up = Vector3::Zero;						//上
	Vector3 m_scale = Vector3::One;						//拡大率
	Quaternion m_rot = Quaternion::Identity;			//回転
	float m_angle = 0.0f;								//角度

	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	float m_invTime = 0.0f;								//無敵時間

	//テスト　撃破エフェクト
	Effect m_destroyEffect;

};

