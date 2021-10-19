#pragma once

//#include "CapsuleCollider.h"
//#include "RigidBody.h"

class MyCharacterController
{
public:
	MyCharacterController() {}
	
	~MyCharacterController() 
	{
		RemoveRigidBody();
	}

	/// @brief 初期化処理
	/// @param radius カプセルコライダーの半径
	/// @param height カプセルコライダーの高さ
	/// @param pos カプセルコライダーの位置
	void Init(float radius, float height, Vector3& pos);

	/// @brief 座標の処理を行う
	/// @param moveSpeed 移動速度
	/// @param downVector 下方向ベクトル
	/// @return 処理後の座標
	const Vector3& Execute(Vector3& moveSpeed,Vector3& downVector);

private:
	//内部で使う関数

	/// @brief 座標を設定
	/// @param pos 座標
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// @brief 剛体の削除
	void RemoveRigidBody();

private:
	bool m_isInit = false;							//初期化フラグ
	Vector3 m_position = Vector3::Zero;				//座標

	CCapsuleCollider m_cupsuleCollider;				//カプセルコライダー
	
	float m_radius = 0.0f;							//カプセルコライダーの半径
	float m_height = 0.0f;							//カプセルコライダーの高さ

	RigidBody m_rigidBody;							//剛体

};

