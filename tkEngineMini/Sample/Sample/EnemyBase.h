#pragma once
class EnemyBase : public IGameObject
{
public:
	EnemyBase();
	virtual ~EnemyBase(){}

	bool Start() override
	{
		return true;
	}

	/// @brief 初期化処理
	/// @param initPoint 
	/// @param initUp 
	void Init(const Vector3& initPoint, const Vector3& initUp);
	/// @brief 初期化処理に付随する処理
	virtual void InitSub() {};


	//共通して行う処理

	/// @brief 更新処理
	void Update() override;
	/// @brief 更新処理に付随する処理
	virtual void UpdateSub() {}

	/// @brief 移動処理
	void Move();
	/// @brief 移動処理に付随する処理
	virtual void MoveSub() {}

	/// @brief 回転処理
	void Rotation();
	/// @brief 回転処理に付随する処理
	virtual void RotationSub(){}

	/// @brief 当たり判定処理
	void Hit();

	/// @brief 無敵時間の減少
	void DecInvTime();

	/// @brief 自機との当たり判定が有効になるまでのカウンターを減少
	void DecToActivateTime();

	/// @brief 撃破処理
	void Destroy();

	/// @brief 自滅処理
	void SelfDestroy();

	/// @brief タイムアップ時の自滅
	void DestroyTimeUp();

	//ゲッター
	/// @brief	座標を取得 
	/// @return 座標
	const Vector3& GetPosition() const { return m_position; }

	/// @brief 拡大率を取得
	/// @return 拡大率
	const Vector3& GetScale() const { return m_scale; }

	/// @brief 回転クォータニオンを取得
	/// @return 回転クォータニオン
	const Quaternion& GetRotation() const { return m_rot; }

	/// @brief 回転角度を取得
	/// @return 回転角度
	const float GetAngle() const { return m_angle; }

	/// @brief スキンモデルレンダーを取得
	/// @return スキンモデルレンダー
	SkinModelRender* GetSkinModelRender(){	return m_skinModelRender; }

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
	void SetAngle(const float angle) { m_angle = angle; }

	/// @brief プレイヤーに対する当たり判定を有効にするかどうか
	/// @param isActive 
	void SetActive(const bool isActive) { m_isActive = isActive; }

	/// @brief 無敵時間を設定
	/// @param invTime 無敵時間
	void SetInvincibleTime(const float invTime) { m_invTime = invTime; }

	//ライトを渡すための関数
	/// @brief ディレクションライトを受けとる
	/// @param dirLight ディレクションライト
	void RecieveDirectionLight(DirectionLight* dirLight) { m_skinModelRender->InitDirectionLight(dirLight); }

	/// @brief ポイントライトを受け取る
	/// @param ptLight ポイントライト
	void RecievePointLight(PointLight* ptLight) { m_skinModelRender->InitPointLight(ptLight); }

	/// @brief スポットライトを受け取る
	/// @param spLight スポットライト
	void RecieveSpotLight(SpotLight* spLight) { m_skinModelRender->InitSpotLight(spLight); }

	/// @brief モデルの初期化
	void InitModelFromInitData() { m_skinModelRender->InitModel(); }

protected:
	SkinModelRender* m_skinModelRender = nullptr;		//モデルレンダー
	MyCharacterController m_myCharaCon;					//球面移動用キャラコン
	SphericalMove m_sphericalMove;						//球面移動用クラス

	Player_new* m_player = nullptr;						//プレイヤーへのポインタ

	//パラメータ各種
	float m_life = 0.0f;								//耐久値
	float m_lifeTime = 1.0f;							//時間寿命
	float m_speed = 0.0f;								//移動速度
	int m_score = 0;									//エネミーの撃破スコア
	float m_durability = 0.0f;							//弾への影響値
	bool m_isExist = false;								//存在フラグ
	bool m_isInvincible = false;						//無敵状態かどうか
	bool m_isActive = false;							//プレイヤーに対する当たり判定が有効かどうか
	float m_angle = 0.0f;								//角度
	float m_invTime = 0.0f;								//無敵時間
	float m_toActivateCounter = 0.0f;					//出現してから当たり判定が有効になるまでのカウンター

	//座標など
	Vector3 m_position = Vector3::Zero;					//座標
	Vector3 m_moveSpeed = Vector3::Zero;				//速度ベクトル
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//レイを飛ばす方向ベクトル
	Vector3 m_forward = Vector3::Zero;					//前方
	Vector3 m_right = Vector3::Zero;					//右
	Vector3 m_up = Vector3::Zero;						//上
	Vector3 m_scale = Vector3::One;						//拡大率
	Quaternion m_rot = Quaternion::Identity;			//回転
	
	//ライト各種へのポインタ
	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	//エフェクト各種
	//撃破エフェクト
	Effect m_destroyEffect;
	//ヒットエフェクト
	Effect m_hitEffect;

};

