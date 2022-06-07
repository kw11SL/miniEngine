#pragma once

/// @brief 弾クラスの基本になるクラス
class BulletBase : public IGameObject
{
public:
	BulletBase(){}
	virtual ~BulletBase();

	/// @brief 初期化処理
	/// @param initPoint 初期位置
	/// @param initUp 初期位置の上ベクトル
	/// @param direction 方向
	void Init(
		const Vector3& initPoint,
		const Vector3& initUp,
		const Vector3& direction
		);

	/// @brief 初期化処理に付随する処理
	virtual void InitSub();

	/// @brief 更新処理に付随する処理
	virtual void UpdateSub();

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

	//セッター
	/// @brief	座標を設定 
	/// @param pos 座標
	void SetPostion(const Vector3& pos)
	{
		m_position = pos;
	}

	/// @brief 拡大率を設定
	/// @param scale 拡大率
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
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

	/// @brief 存在フラグをオン
	void SetIsExist()
	{
		m_isExist = true;
	}

	/// @brief 存在フラグを取得
	/// @return 
	bool GetIsExist()
	{
		return m_isExist;
	}

	/// @brief 弾のダメージを取得
	/// @return 
	float GetPower()
	{
		return m_power;
	}

	/// @brief 残り存在時間を取得
	/// @return 
	float GetLifeTime()
	{
		return m_lifeTime;
	}

	/// @brief ダメージ間隔を取得
	/// @return 
	float GetDamageInterval()
	{
		return m_damageInterval;
	}

	/// @brief 耐久値を減少
	/// @param decVal 減少させる値
	void DecLife(float decVal)
	{
		m_life -= decVal;
	}

	//内部で使う関数
	bool Start() override
	{
		return true;
	}

	/// @brief 更新処理
	void Update() override;

	/// @brief 移動処理
	void Move();
	/// @brief 移動に付随する処理
	virtual void MoveSub();

	/// @brief 回転処理
	void Rotation();
	/// @brief 回転に付随する処理
	virtual void RotationSub();

	/// @brief 生存時間を減少し、0以下で破棄
	void DecLifeTime();

	/// @brief 当たり判定処理
	void Hit();

	/// @brief 破棄処理
	void Destroy();
	/// @brief 破棄処理に付随する処理
	virtual void DestroySub();

	/// @brief エフェクトの初期化処理
	void InitEffect();
	/// @brief エフェクトの初期化に付随する処理
	virtual void InitEffectSub();

	/// @brief エフェクトの更新処理
	void EffectUpdate();
	/// @brief エフェクトの更新処理に付随する処理
	virtual void EffectUpdateSub();

protected:
	MyCharacterController m_myCharaCon;					//自作のキャラクターコントローラ
	SphericalMove m_sphericalMove;						//球面移動用クラス

	float m_life = 0.0f;								//耐久値
	float m_lifeTime = 0.0f;							//時間寿命
	float m_speed = 0.0f;								//移動速度
	float m_power = 0.0f;								//弾が与えるダメージ
	bool m_isExist = true;								//存在フラグ
	float m_damageInterval = 0.5f;						//ダメージを与える間隔。この数値だけエネミーに無敵時間を設定する。

	Vector3 m_position = Vector3::Zero;					//座標
	Vector3 m_moveSpeed = Vector3::Zero;				//速度ベクトル
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//レイを飛ばす方向ベクトル
	Vector3 m_forward = Vector3::Zero;					//前方
	Vector3 m_oldForward = Vector3::Zero;				//球面移動クラスによる更新前の記録用(回転に使用する)
	Vector3 m_right = Vector3::Zero;					//右
	Vector3 m_up = Vector3::Zero;						//上
	Vector3 m_scale = Vector3::One;						//拡大率
	Quaternion m_rot = Quaternion::Identity;			//回転
	float m_angle = 0.0f;								//角度

	//ライトのポインタ
	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	Vector3 m_direction = Vector3::Zero;				//最初の発射方向
	bool m_isDecideDirection = false;					//発射方向を前方ベクトルにしたかどうか

	Effect m_shotEffect;								//弾のエフェクト
	Effect m_banishEffect;								//弾消滅時のエフェクト
		
};

