#pragma once
#include "ExplosionManager.h"
#include "BulletBase.h"
#include "BulletPlayerNormal.h"
#include "BulletPlayerSpread.h"
#include "BulletEnemyNormal.h"

class ExplosionManager;

/// @brief 弾の種類
enum EnBulletType
{
	enPlayerNormal,
	enPlayerSpreadBomb,
	enEnemyNormal,
	enBulletTypeNum
};

/// @brief 弾クラス。弾の基底クラスを内部に持って使う。
class Bullet : public IGameObject
{
public:
	Bullet() {}
	~Bullet();

	/// @brief 初期化処理
	/// @param initPoint 初期位置
	/// @param initUp 初期位置の上ベクトル
	/// @param direction 方向
	/// @param bulletType 弾の種類
	void Init(
		const Vector3& initPoint,
		const Vector3& initUp,
		const Vector3& direction,
		const EnBulletType& bulletType);

	//ゲッター
	/// @brief	座標を取得 
	/// @return 座標
	Vector3 GetPosition() 
	{ 
		return m_bulletBase->GetPosition();
	}

	/// @brief 拡大率を取得
	/// @return 拡大率
	Vector3 GetScale() 
	{ 
		return m_bulletBase->GetScale();
	}

	/// @brief 回転を取得
	/// @return 回転
	Quaternion GetRotation() 
	{
		return m_bulletBase->GetRotation();
	}

	/// @brief 回転角度を取得
	/// @return 回転角度
	float GetAngle() 
	{ 
		return m_bulletBase->GetAngle();
	}

	/// @brief 存在フラグを取得
	/// @return 
	bool GetIsExist()
	{
		return m_bulletBase->GetIsExist();
	}

	/// @brief 弾のダメージを取得
	/// @return 
	float GetPower()
	{
		return m_bulletBase->GetPower();
	}

	/// @brief 弾の時間寿命を取得
	/// @return 
	float GetLifeTime()
	{
		return m_bulletBase->GetLifeTime();
	}

	/// @brief ダメージ間隔を取得
	/// @return 
	float GetDamageInterval()
	{
		return m_bulletBase->GetDamageInterval();
	}
		
	//セッター
	/// @brief	座標を設定 
	/// @param pos 座標
	void SetPostion(const Vector3& pos)
	{
		m_bulletBase->SetPostion(pos);
	}

	/// @brief 拡大率を設定
	/// @param scale 拡大率
	void SetScale(const Vector3& scale)
	{
		m_bulletBase->SetScale(scale);
	}

	/// @brief 回転を設定
	/// @param rot	回転
	void SetRotation(const Quaternion rot)
	{
		m_bulletBase->SetRotation(rot);
	}

	/// @brief 角度を設定
	/// @param angle 角度
	void SetAngle(const float& angle)
	{
		m_bulletBase->SetAngle(angle);
	}

	/// @brief 存在フラグをオン(true)にする
	void SetIsExist()
	{
		m_bulletBase->SetIsExist();
	}

	/// @brief 耐久値を減少
	/// @param decVal 減少させる値
	void DecLife(float decVal)
	{
		m_bulletBase->DecLife(decVal);
	}

private:
	//内部で使う関数
	
	/// @brief 生成時に1度だけ呼ばれる処理。trueで完了 
	/// @return 
	bool Start() override;

	/// @brief 更新処理
	void Update() override;

private:	
	
	//弾の基底クラス
	BulletBase* m_bulletBase = nullptr;			
	
	//弾の派生クラス
	BulletPlayerNormal m_bulletPlayerNormal;		//プレイヤー通常弾
	BulletPlayerSpread m_bulletPlayerSpread;		//プレイヤースプレッドボム
	BulletEnemyNormal m_bulletEnemyNormal;			//エネミー通常弾
};

