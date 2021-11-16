#pragma once
/// @brief 爆発の基底クラス

class ExplosionBase : public IGameObject
{
public:
	ExplosionBase() {}
	virtual ~ExplosionBase() {}

	bool Start() override
	{
		return true;
	}

	/// @brief 初期化処理
	/// @param pos 初期位置
	/// @param scale 拡大率
	/// @param effectFilePath エフェクトのファイルパス 
	void Init(
		const Vector3& pos,
		const float scale
	);

	/// @brief 初期化に付随する処理
	virtual void InitSub();

	/// @brief エフェクトの初期化
	void InitEffect(const char16_t* filePath);

	/// @brief 更新処理
	void Update() override;
	
	/// @brief 更新処理に付随する処理 
	virtual void UpdateSub();

	/// @brief 座標を設定
	/// @param pos 
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// @brief 拡大率を設定
	/// @param scale 
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// @brief 拡大率(float版)を設定
	/// @param scaleRate 
	void SetScaleRate(const float scaleRate)
	{
		m_scaleRate = scaleRate;
	}

	/// @brief 回転クォータニオンを設定
	/// @param rot 
	void SetRotation(const Quaternion& rot)
	{
		m_rot = rot;
	}

	/// @brief 移動速度ベクトルを設定
	/// @param speed 
	void SetMoveSpeed(const Vector3& moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}

	/// @brief 速さを設定
	/// @param speed 
	void SetSpeed(const float speed)
	{
		m_speed = speed;
	}

	/// @brief 座標を取得
	/// @return 
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// @brief 拡大率を取得
	/// @return 
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/// @brief 拡大率(float版)を取得
	/// @return 
	const float GetScaleRate() const
	{
		return m_scaleRate;
	}

	/// @brief 回転クォータニオンを取得
	/// @return 
	const Quaternion& GetRotation() const
	{
		return m_rot;
	}
	
	/// @brief 移動速度ベクトルを取得
	/// @return 
	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	/// @brief ダメージ値を取得
	/// @return 
	const float GetPower() const
	{
		return m_power;
	}

	/// @brief ダメージ半径を取得
	/// @return 
	const float GetDamageArea() const
	{
		return m_damageArea;
	}

	/// @brief ダメージ間隔を取得
	/// @return 
	const float GetDamageInterval() const
	{
		return m_damageInterval;
	}

	const bool GetIsExist() const
	{
		return m_isExist;
	}

	/// @brief エフェクトに座標、回転クォータニオン、拡大率を適用して更新
	void EffectUpdate();

	/// @brief 生存時間を減少
	void DecLifeTime();

	/// @brief エフェクトを再生
	void PlayEffect()
	{
		if (m_effect.IsPlay() == false) {
			m_effect.Play();
		}
	}

	/// @brief 生存フラグを調べて削除する処理
	void Destroy();

protected:
	Effect m_effect;							//エフェクト
	const char16_t* m_effectFilePath = u"";		//エフェクトファイルパス
	Vector3 m_position = Vector3::Zero;			//座標
	Quaternion m_rot = Quaternion::Identity;	//回転クォータニオン
	Vector3 m_scale = Vector3::One;				//拡大率
	Vector3 m_moveSpeed = Vector3::Zero;		//移動速度

	bool m_isExist = true;						//存在フラグ

	float m_speed = 0.0f;						//速さ
	float m_lifeTime = 0.0f;					//生存時間
	float m_scaleRate = 1.0f;					//拡大率(float版)
	float m_power = 0.0f;						//ダメージ
	float m_damageArea = 0.0f;					//ダメージの発生する半径
	float m_damageInterval = 0.0f;				//ダメージの発生間隔。この数値だけエネミーに無敵時間を設定する。

};

