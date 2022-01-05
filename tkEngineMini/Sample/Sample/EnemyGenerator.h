#pragma once
#include "Enemy.h"

class EnemyGenerator : public IGameObject
{
public:
	EnemyGenerator() {}
	~EnemyGenerator();

	bool Start()override { return true; }
	void Update()override;

	/// @brief 初期化処理
	/// @param pos 座標
	/// @param rot 回転
	/// @param isActive 最初からアクティブにするかどうか
	/// @param enemyType スポーンさせるエネミーの種類
	void Init(const Vector3& pos,const Quaternion& rot ,const bool isActive, const EnEnemyType& enemyType);

	/// @brief エネミー生成
	/// @param enemyType スポーンさせるエネミーの種類
	void GenerateEnemy(const EnEnemyType& enemyType);

	void Move();

	void Rotation();

	/// @brief スポーン周期用のカウンターを増加
	void AddCounter()
	{
		m_spawnCounter += g_gameTime->GetFrameDeltaTime();
	}

	/// @brief エフェクト発生用のカウンターを増加
	void AddSpawnEffectPlayCounter()
	{
		m_spawnEffectCounter += g_gameTime->GetFrameDeltaTime();
	}

	/// @brief 座標を設定
	/// @param pos 
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// @brief クォータニオンを設定
	/// @param rot 
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// @brief アクティブフラグを設定
	/// @param isActive 
	void SetActive(const bool isActive)
	{
		m_isActive = isActive;
	}

	/// @brief 座標を取得
	/// @return 
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// @brief クォータニオンを取得
	/// @return 
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/// @brief 生成器のアクティブ状態を取得
	/// @return 
	const bool GetIsActive() const
	{
		return m_isActive;
	}

	//生成器のアクティベート処理
	void Activate();

	/// @brief スポーン時のエフェクト再生処理
	void PlaySpawnEffect();

	/// @brief エフェクトの更新
	void UpdateEffect();

	/// @brief 生成したエネミーの削除および配列からの削除処理
	void DeleteEnemy();

	/// @brief 下方向ベクトルを上方向の反対で初期化
	/// @param up 上方向
	void SetDownVector(const Vector3& up)
	{
		m_downVector = up * -1.0f;
	}

	/// @brief クォータニオンで右、上、前を回転
	/// @param rot 適用するクォータニオン
	void SetVectorFromQuaternion(const Quaternion& rot)
	{
		rot.Apply(m_right);
		rot.Apply(m_up);
		rot.Apply(m_forward);

		m_right.Normalize();
		m_up.Normalize();
		m_forward.Normalize();

		//変更した上方向を基に下方向を設定する
		SetDownVector(m_up);
	}

private:
	std::vector<Enemy*> m_enemies;						//エネミーの可変長配列

	MyCharacterController m_myCharaCon;					//球面移動用キャラコン
	SphericalMove m_sphericalMove;						//球面移動用クラス
	Vector3 m_position = Vector3::Zero;					//座標
	Quaternion m_rotation = Quaternion::Identity;		//回転クォータニオン
	Vector3 m_forward = Vector3::Zero;					//前方
	Vector3 m_right = Vector3::Zero;					//右
	Vector3 m_up = Vector3::Zero;						//上
	Vector3 m_moveSpeed = Vector3::Zero;				//速度ベクトル
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//レイを飛ばす下方向ベクトル
	EnEnemyType m_spawnEnemyType = enCommon;			//スポーンさせるエネミータイプ
	float m_spawnCounter = 0.0f;						//スポーン間隔用カウンター
	float m_spawnEffectCounter = 0.0f;					//スポーンエフェクトの再生用カウンター
	bool m_isActive = false;							//生成器がアクティブかどうか

	EnemyManager* m_enemyManager = nullptr;				//エネミー管理クラスのオブジェクト

	Effect m_spawnEffect;								//エネミーをスポーンさせるときのエフェクト
};

