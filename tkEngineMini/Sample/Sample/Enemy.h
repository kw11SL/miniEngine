#pragma once
#include "EnemyBase.h"
#include "EnemyNormal.h"
#include "EnemyShot.h"
#include "EnemyBomb.h"


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

/// @brief エネミークラス。エネミーの基底クラスを内部に持って使う。
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
	const Vector3& GetPosition() const 
	{
		return m_enemyBase->GetPosition();
	}

	/// @brief 拡大率を取得
	/// @return 拡大率
	const Vector3& GetScale() const 
	{ 
		return m_enemyBase->GetScale();
	}

	/// @brief 回転を取得
	/// @return 回転
	const Quaternion& GetRotation() const 
	{ 
		return m_enemyBase->GetRotation();
	}

	/// @brief 回転角度を取得
	/// @return 回転角度
	const float GetAngle() const 
	{ 
		return m_enemyBase->GetAngle();
	}

	/// @brief スキンモデルレンダーを取得
	/// @return スキンモデルレンダー
	SkinModelRender* GetSkinModelRender()
	{
		return m_enemyBase->GetSkinModelRender();
	}

	/// @brief エネミーの種類を取得
	/// @return 
	EnEnemyType& GetType()
	{
		return m_enEnemyType;
	}

	/// @brief エネミーの撃破スコアを取得
	/// @return 
	const int GetScore()
	{
		return m_enemyBase->GetScore();
	}

	/// @brief プレイヤーに対する当たり判定が有効かどうか
	/// @return アクティブフラグ
	const bool GetIsActive()
	{
		return m_enemyBase->GetIsActive();
	}

	/// @brief 無敵状態かどうか？
	/// @return 無敵フラグ
	const bool GetIsInvincible()
	{
		return m_enemyBase->GetIsInvincible();
	}

	/// @brief 存在しているかどうか？
	/// @return 存在フラグ
	const bool GetIsExist()
	{
		return m_enemyBase->GetIsExist();
	}

	//セッター
	/// @brief	座標を設定 
	/// @param pos 座標
	void SetPostion(const Vector3& pos) 
	{ 
		m_enemyBase->SetPostion(pos);
	}

	/// @brief 拡大率を設定
	/// @param scale 拡大率
	void SetScale(const Vector3& scale) 
	{ 
		m_enemyBase->SetScale(scale);
	}

	/// @brief 回転を設定
	/// @param rot	回転
	void SetRotation(const Quaternion rot) 
	{ 
		m_enemyBase->SetRotation(rot);
	}

	/// @brief 角度を設定
	/// @param angle 角度
	void SetAngle(const float angle) 
	{
		m_enemyBase->SetAngle(angle);
	}

	/// @brief プレイヤーに対する当たり判定を有効にするかどうか
	/// @param isActive 
	void SetActive(const bool isActive) 
	{ 
		m_enemyBase->SetActive(isActive);
	}

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
		m_enemyBase->SetInvincibleTime(invTime);
	}

	//ライトを渡すための関数

	/// @brief ディレクションライトを受けとる
	/// @param dirLight ディレクションライト
	void RecieveDirectionLight(DirectionLight* dirLight) 
	{
		m_enemyBase->RecieveDirectionLight(dirLight);
	}

	/// @brief ポイントライトを受け取る
	/// @param ptLight ポイントライト
	void RecievePointLight(PointLight* ptLight)
	{
		m_enemyBase->RecievePointLight(ptLight);
	}

	/// @brief スポットライトを受け取る
	/// @param spLight スポットライト
	void RecieveSpotLight(SpotLight* spLight)
	{
		m_enemyBase->RecieveSpotLight(spLight);
	}

	/// @brief モデルの初期化
	void InitModelFromInitData() 
	{
		m_enemyBase->InitModelFromInitData();
	}

	/// @brief プレイヤーに点数を加点せず即座に自滅させる
	void SelfDestroy();

private:
	//内部で行う処理
	
	/// @brief 生成時に1度だけ呼ばれる処理
	/// @return 初期化終了フラグ 
	bool Start() override;
//
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

	/// @brief プレイヤー方向基準で一定時間おきに弾を均等な全方位に撃つ。
	/// @param wayNum 同時発射数。奇数なら止まっていると当たり、偶数なら当たらない。1を指定すれば単発自機狙い弾。
	void FireBulletEqually(const int wayNum,const EnBulletType& bulletType);

	/// @brief 発射までのカウンターの増加
	void AddShotCounter();

	/// @brief エフェクトの更新
	void UpdateEffect();

	/// @brief 時間寿命エフェクトのスケーリング処理
	void LifeRingScaling();
	
	/// @brief 発射直前に発生する予兆エフェクトのスケーリング処理
	/// @param activateTime 発生させるタイミング
	void ShotNoticeScaling(const float activateTime);

private:
	
	//エネミーの基底クラス
	EnemyBase* m_enemyBase = nullptr;			
	
	//エネミーの派生クラス
	EnemyNormal m_enemyNormal;					//通常タイプ				
	EnemyShot m_enemyShot;						//射撃タイプ
	EnemyBomb m_enemyBomb;						//自爆タイプ

	EnEnemyType m_enEnemyType = enCommon;		//エネミーのタイプ

};

