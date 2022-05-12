#pragma once
#include "Explosion.h"

class ExplosionManager;

/// @brief 弾の種類
enum EnBulletType
{
	enPlayerNormal,
	enPlayerSpreadBomb,
	enEnemyNormal,
	enBulletTypeNum
};

class Bullet : public IGameObject
{
public:
	Bullet() {}
	~Bullet();

	/// @brief 初期化処理
	/// @param renderingEngine レンダリングエンジン
	/// @param initPoint 初期位置
	/// @param initUp 初期位置の上ベクトル
	/// @param direction 方向
	/// @param bulletType 弾の種類
	void Init(
		RenderingEngine& renderingEngine,
		const Vector3& initPoint,
		const Vector3& initUp,
		const Vector3& direction,
		const EnBulletType& bulletType);

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

	/// @brief モデルが削除されたかどうか
	/// @return 
	bool GetIsModelDeleted()
	{
		return m_isModelDeleted;
	}

	/// @brief 弾のタイプを設定
	/// @param type 
	void SetType(const EnBulletType& type)
	{
		m_enBulletType = type;
	}

	/// @brief 弾のタイプを取得
	/// @return 
	EnBulletType GetType()
	{
		return m_enBulletType;
	}

	/// @brief 弾のダメージを取得
	/// @return 
	float GetPower()
	{
		return m_power;
	}

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

	/// @brief スキンモデルを削除
	void DeleteSkinModel()
	{
		DeleteGO(m_skinModelRender);
		m_isModelDeleted = true;
	}

	/// @brief 耐久値を減少
	/// @param decVal 減少させる値
	void DecLife(float decVal)
	{
		m_life -= decVal;
	}

private:
	//内部で使う関数
	
	/// @brief 生成時に1度だけ呼ばれる処理。trueで完了 
	/// @return 
	bool Start() override;


	void Update() override;

	/// @brief 移動処理
	void Move();

	/// @brief 回転処理
	void Rotation();

	/// @brief 生存時間を減少し、0以下で破棄
	void DecLifeTime();

	/// @brief 削除処理
	void Destroy();

	void Hit();

	/// @brief エフェクトの初期化
	/// @param bulletType 弾のタイプ
	void InitEffect(const EnBulletType& bulletType);

private:	
	SkinModelRender* m_skinModelRender = nullptr;		//スキンモデルレンダー
	MyCharacterController m_myCharaCon;					//自作のキャラクターコントローラ
	SphericalMove m_sphericalMove;						//球面移動用クラス
	EnBulletType m_enBulletType = enPlayerNormal;		//弾のタイプ

	Player_new* m_player = nullptr;						//プレイヤーへのポインタ
	ExplosionManager* m_explosionManager = nullptr;		//爆発マネージャへのポインタ
	DirectionLight* m_directionLight = nullptr;			//ディレクションライトへのポインタ
	PointLight* m_pointLight = nullptr;					//ポイントライトへのポインタ
	SpotLight* m_spotLight = nullptr;					//スポットライトへのポインタ

	float m_life = 0.0f;								//耐久値
	float m_speed = 0.0f;								//速さ
	float m_lifeTime = 0.0f;							//時間寿命
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

	Vector3 m_direction = Vector3::Zero;				//最初の発射方向
	bool m_isDecideDirection = false;					//発射方向を前方ベクトルにしたかどうか

	//ショットエフェクト
	Effect m_shotEffect;								//通常ショットのエフェクト
	Effect m_spreadBurstEffect;							//スプレッドボムのエフェクト
	Effect m_normalBanishEffect;						//通常ショットの消滅エフェクト

	//スキンモデルレンダーの削除フラグ
	bool m_isModelDeleted = false;

	
};

