#pragma once

enum EnUseWeapon {
	enNormalShot,
	enSpreadBomb,
	enUseWeaponNum
};

class Player_new : public IGameObject
{
public:
	Player_new(){}
	~Player_new();

	//ゲッター
	/// @brief	座標を取得 
	/// @return 座標
	const Vector3& GetPosition() const 
	{ 
		return m_position; 
	}

	/// @brief 上ベクトルを取得
	/// @return 上ベクトル
	const Vector3& GetUp() const
	{
		return m_up;
	}
	
	/// @brief 拡大率を取得
	/// @return 拡大率
	const Vector3& GetScale() const 
	{ 
		return m_scale; 
	}
	
	/// @brief 回転を取得
	/// @return 回転
	const Quaternion& GetRotation() const 
	{ 
		return m_rot; 
	}
	
	/// @brief 回転角度を取得
	/// @return 回転角度
	const float GetAngle() const 
	{ 
		return m_angle; 
	}

	/// @brief スキンモデルレンダーを取得
	/// @return スキンモデルレンダー
	const SkinModelRender* GetSkinModelRender()
	{
		return m_skinModelRender;
	}

	/// @brief 無敵状態を取得
	/// @return 
	bool GetIsInvincible()
	{
		return m_isInvincible;
	}

	/// @brief 存在フラグを取得
	/// @return 
	bool GetIsExist()
	{
		return m_isExist;
	}

	//セッター
	/// @brief	座標を設定 
	/// @param pos 座標
	void SetPostion(const Vector3& pos)
	{
		m_position = pos;
		//m_skinModelRender->SetPosition(m_position);
	}

	/// @brief 拡大率を設定
	/// @param scale 拡大率
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		//m_skinModelRender->SetScale(m_scale);
	}

	/// @brief 回転を設定
	/// @param rot	回転
	void SetRotation(const Quaternion& rot)
	{
		m_rot = rot;
	}

	/// @brief 行列から回転を設定
	/// @param mat 行列
	void SetRotationFromMatrix(const Matrix& mat)
	{
		m_rot.SetRotation(mat);
	}
	
	/// @brief 角度を設定
	/// @param angle 角度
	void SetAngle(const float angle)
	{
		m_angle = angle;
	}

	/// @brief 無敵状態フラグをセット
	/// @param flag 
	void SetIsInvFlag(const bool invFlag)
	{
		m_isInvincible = invFlag;
	}

	/// @brief 生存フラグをセット
	/// @param existFlag 
	void SetIsExist(const bool existFlag)
	{
		m_isExist = existFlag;
	}

	/// @brief 無敵時間を設定
	/// @param invTime 
	void SetInvincibleTime(const float invTime)
	{
		m_invincebleTime = invTime;
	}

	/// @brief 下方向ベクトルを設定
	/// @param up 上方向ベクトル
	void SetDownVector(const Vector3& up)
	{
		m_downVector = up * -1.0f;
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

	/// @brief キャラコンの初期化
	void InitCharaCon()
	{
		m_myCharaCon.Init(m_position);
	}

	/// @brief モデル復活処理
	void Revive();

	/// @brief 復活準備処理とエフェクト再生処理
	void ReviveReady();

	/// @brief クォータニオンで右、上、前を回転
	/// @param rot 適用するクォータニオン
	void SetVectorFromQuaternion(const Quaternion& rot)
	{
		//各ベクトルにクォータニオンを適用
		rot.Apply(m_right);
		rot.Apply(m_up);
		rot.Apply(m_forward);

		//各ベクトルを正規化
		m_right.Normalize();
		m_up.Normalize();
		m_forward.Normalize();

		//変更した上ベクトルで下方向ベクトルを設定
		SetDownVector(m_up);
	}

private:
	//内部で使う処理

	bool Start() override;

	void Update() override;

	/// @brief 移動処理
	void Move();

	/// @brief 回転処理
	void Rotation();

	/// @brief 弾の発射方向を回転
	void RotateShotDirection();

	/// @brief 弾を発射
	void FireBullet();

	/// @brief 無敵時間を減少し、0以下なら無敵フラグを解除
	void DecInvTime();

	/// @brief 被弾から復活までのカウンターを増加
	void AddReviveCouter();

	/*void AddBullet(Bullet& bullet)
	{
		m_bullet.push_back(&bullet);
	}*/

	/// @brief 武器の切り替え機能
	void ChangeWeapon();

	/// @brief 敵や敵弾との当たり判定処理
	void Hit();

	/// @brief エフェクトの初期化
	void InitEffect();

private:

	SkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダー
	MyCharacterController m_myCharaCon;					//自作のキャラクターコントローラ
	SphericalMove m_sphericalMove;						//球面移動用クラス
	GameCamera m_gameCamera;							//ゲームカメラ
	
	//Bullet* m_bullet = nullptr;
	std::vector<Bullet*> m_bullet;							//弾クラスの可変長配列
	
	Explosion* m_explosion = nullptr;

	Vector3 m_position = Vector3::Zero;					//座標
	Vector3 m_moveSpeed = Vector3::Zero;				//速度ベクトル
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//レイを飛ばす方向ベクトル
	Vector3 m_forward = Vector3::Front;					//前方
	Vector3 m_right	= Vector3::Right;					//右
	Vector3 m_up = Vector3::Up;							//上
	Vector3 m_upPrev = Vector3::Zero;					//前フレームの上
	Vector3 m_cameraUp = Vector3::Zero;					//カメラの上(カメラに反映する上方向)
	Vector3 m_scale = Vector3::One;						//拡大率
	Quaternion m_rot = Quaternion::Identity;			//回転
	Quaternion m_rotUpToGroundNormal = Quaternion::Identity;
	
	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	float m_rotFraction = 1.0f;
	float m_cameraUpFraction = 0.0f;					//カメラ上方向を回転させる補間率
	float m_angle = 0.0f;								//回転角度

	int m_life = 0;										//プレイヤーのライフ
	bool m_isInvincible = false;						//無敵状態フラグ
	bool m_isInvinciblePrev = false;					//前フレームの無敵状態フラグ
	float m_reviveCounter = 0.0f;						//被弾から復活までのカウンター
	bool m_isExist = true;								//生存フラグ
	bool m_isExistPrev = true;							//前フレームの生存フラグ
	bool m_isReviveReady = false;						//復活準備フラグ(エフェクト再生用)
	bool m_isReviveReadyPrev = false;					//前フレームの復活準備フラグ
	float m_markerCounter = 0.0f;						//自機位置表示のエフェクト再生カウンター

	float m_invincebleTime = 0.0f;						//無敵時間
	float m_fireCounter = 0.0f;							//発射間隔カウンタ
	Vector3 m_shotDirection = Vector3::Zero;			//ショットを撃つ方向

	EnUseWeapon m_enUseWeapon = enNormalShot;			//現在使用しているショット
	EnBulletType m_enBulletType = enPlayerNormal;		//弾に渡す弾のタイプ情報

	BulletManager* m_bulletManager = nullptr;			//弾の管理クラスのオブジェクト

	Effect m_explosionEffect;							//被弾時のエフェクト
	Effect m_reviveEffect;								//復活時のエフェクト
	Effect m_moveTrackEffect;							//移動時の軌跡エフェクト
	Effect m_markerEffect;								//自機位置の表示エフェクト
};

