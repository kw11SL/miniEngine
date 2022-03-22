#pragma once
#include"Explosion.h"
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
	
	/// @brief 初期化処理
	/// @param renderingEngine 
	/// @param initPoint 
	/// @param initUp 
	/// @param enemyType 
	void Init(
		const Vector3& initPoint,
		const Vector3& initUp,
		const EnEnemyType& enemyType);

	//ゲッター
	/// @brief	座標を取得 
	/// @return 座標
	const Vector3& GetPosition() const { return m_position; }

	/// @brief 拡大率を取得
	/// @return 拡大率
	const Vector3& GetScale() const { return m_scale; }

	/// @brief 回転を取得
	/// @return 回転
	const Quaternion& GetRotation() const { return m_rot; }

	/// @brief 回転角度を取得
	/// @return 回転角度
	const float GetAngle() const { return m_angle; }

	/// @brief スキンモデルレンダーを取得
	/// @return スキンモデルレンダー
	SkinModelRender* GetSkinModelRender()
	{
		return m_skinModelRender;
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

	/// @brief プレイヤーに対する当たり判定が有効かどうか
	/// @return アクティブフラグ
	const bool GetIsActive()
	{
		return m_isActive;
	}

	/// @brief 無敵状態かどうか？
	/// @return 無敵フラグ
	bool GetIsInvincible()
	{
		return m_isInvincible;
	}

	/// @brief 存在しているかどうか？
	/// @return 存在フラグ
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
	void SetAngle(const float angle) { m_angle = angle;}

	/// @brief プレイヤーに対する当たり判定を有効にするかどうか
	/// @param isActive 
	void SetActive(const bool isActive) { m_isActive = isActive; }

	/// @brief エネミーの種類を設定
	/// @param type 
	void SetType(const EnEnemyType& type)
	{
		m_enEnemyType = type;
	}

	/// @brief 無敵時間を設定
	/// @param invTime 無敵時間
	void SetInvincibleTime(const float invTime)
	{
		m_invTime = invTime;
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

	/// @brief プレイヤーに点数を加点せず即座に自滅させる
	void SelfDestroy();

private:
	//内部で行う処理
	
	/// @brief 生成時に1度だけ呼ばれる処理
	/// @return 初期化終了フラグ 
	bool Start() override;

	/// @brief 更新処理
	void Update() override;

	/// @brief 移動処理
	void Move();

	/// @brief 回転処理
	void Rotation();

	/// @brief ヒット処理
	void Hit();

	/// @brief 撃破処理
	void Destroy();

	/// @brief タイムアップ時に消滅させる処理
	void DestroyTimeUp();

	/// @brief 無敵時間を減らし、無敵時間が切れたら無敵状態をオフにする
	void DecInvTime();

	/// @brief エネミーの生存時間を減らす
	void DecLifeTime();

	/// @brief 当たり判定が有効になるまでのカウンターを減らす
	void DecToActivateTime();

private:
	SkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダー
	MyCharacterController m_myCharaCon;					//自作のキャラクターコントローラ
	SphericalMove m_sphericalMove;						//球面移動用クラス
	EnEnemyType m_enEnemyType = enCommon;				//エネミーのタイプ

	Player_new* m_player = nullptr;						//プレイヤーへのポインタ
	Explosion* m_explosion = nullptr;

	float m_life = 0.0f;								//耐久値
	float m_lifeTime = 1.0f;							//時間寿命
	float m_speed = 0.0f;								//移動速度
	int m_score = 0;									//エネミーの撃破スコア
	float m_durability = 0.0f;							//弾への影響値
	bool m_isExist = false;								//存在フラグ
	bool m_isInvincible = false;						//無敵状態かどうか
	bool m_isActive = false;							//プレイヤーに対する当たり判定が有効かどうか

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
	float m_toActivateCounter = 0.0f;					//出現してから当たり判定が有効になるまでのカウンター

	//撃破エフェクト
	Effect m_destroyEffect;
	//ヒットエフェクト
	Effect m_hitEffect;

};

