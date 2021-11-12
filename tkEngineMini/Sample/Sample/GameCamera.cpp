#include "stdafx.h"
#include "GameCamera.h"

namespace {
	const float CAMERA_MOOVESPEED = 5.0f;
	const float TARGET_MOVESPEED = 5.0f;

	const float DAMPING_CONST = 35.0f;			//ダンピング定数
	const float DAMPING_RATE_CONST = 100.0f;	//減衰率を計算する際に使う減衰率の固定値
}

void GameCamera::Init(const float maxMoveSpeed)
{
	//カメラ、注視点が目標を追いかけるスピードを設定
	m_cameraMoveSpeed = CAMERA_MOOVESPEED;
	m_targetMoveSpeed = TARGET_MOVESPEED;

	//カメラの設定
	g_camera3D->SetPosition({ 0.0f,500.0f,200.0f });
	g_camera3D->SetTarget({ 0.0f,500.0f,0.0f });

	//移動速度の最大値を設定
	m_maxMoveSpeed = maxMoveSpeed;

	//ビュー行列、プロジェクション行列を更新
	g_camera3D->Update();
}

void GameCamera::SetUp(const Vector3& up)
{
	g_camera3D->SetUp(up);
}

void GameCamera::ChaseCameraPosition()
{
	//return;
	//カメラ座標からカメラ目標までのベクトルを作成
	Vector3 posToTarget = m_cameraPositionTarget - m_cameraPosition;
	//長さを取得
	float posToTargetLength = posToTarget.Length();

	//正規化
	posToTarget.Normalize();
	//targetToPos.Normalize();

	
	m_cameraPosition += posToTarget * m_cameraMoveSpeed;
	
	//カメラ座標を設定
	g_camera3D->SetPosition(m_cameraPosition);
}

void GameCamera::ChaseTargetPosition()
{
	//return;
	//注視点から注視点目標までのベクトルを作成
	Vector3 posToTarget = m_targetPositionTarget - m_targetPosition;
	float posToTargetLength = posToTarget.Length();

	//正規化
	posToTarget.Normalize();
	//targetToPos.Normalize();

	//注視点目標の方向に注視点を移動
	m_targetPosition += posToTarget * m_targetMoveSpeed;

	//注視点を設定
	g_camera3D->SetTarget(m_targetPosition);
}

void GameCamera::LerpUpVector(float fraction, Vector3& up)
{
	//線形補完
	up.Lerp(fraction, up, m_upTarget);
	//正規化
	up.Normalize();
}

float& GameCamera::CalcDampingRate(
	float dampingRateCurrent,
	float dampingRateTarget,
	float& moveSpeed
)
{
	//1フレームあたりの経過時間を取得
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	//計算で使うためのダンピング定数を設定する
	float dampingRate = DAMPING_RATE_CONST;

	//現在の減衰率から目標値までの差分値を計算
	float distance = dampingRateTarget - dampingRateCurrent;
	//差分値の絶対値がほぼ0のとき
	if (fabsf(distance) < FLT_EPSILON) {
		//現在の減衰率の変化量を0にし、目標値を返す
		moveSpeed = 0.0f;
		return dampingRateTarget;
	}

	/////////////////////////////////////////////////////////////////////////
	//減衰率を変化させるための加速度を求める
	//加速度を作成
	float springAcceleration;

	//現在の減衰率が目標値に対して、増加の方向なのか減少の方向なのかを決めるための方向
	//差分値を格納
	float originalDir = distance;
	//差分値の絶対値で割って正規化する
	originalDir /= fabsf(distance);

	//加速度には上で求めた差分値を格納しておく
	springAcceleration = distance;

	//ばね方程式より、 F = ma = -(Ksx) - (Kdv)
	//質量m = 1とすると、加速度 a = -(Ksx) - (Kdv)
	//Ks : ばね定数
	//x  : 目標値までの差分値(ここでは、目標値　→　現在値)
	//Kd : ダンピング定数
	//v  : 現在値の移動速度

	//まず、ダンピング定数と減衰率からばね定数を求める
	float t = DAMPING_CONST / (2.0f * dampingRate);
	//ばね定数を決定
	float springK = t * t;
	

	//上記ばね方程式より加速度を求める
	//-Ksx  
	//計算した差分値は、現在値　→　目標値　なので-1が掛かる
	//現在、加速度には差分値が入っているのでばね定数を掛ける
	springAcceleration *= springK;

	//-Kdv
	//移動速度を格納
	float vt = moveSpeed;
	//移動速度にダンピング定数を掛ける
	vt *= DAMPING_CONST;

	//-Ksx - Kdv
	springAcceleration - vt;

	//加速度に経過時間を掛ける
	springAcceleration *= deltaTime;
	/////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////
	//移動速度の更新
	//移動速度に加速度を足し込んでいく
	moveSpeed += springAcceleration;
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	//更新した移動速度を使って現在値を更新する
	//更新する予定の現在値を作成
	float newDampingRate = dampingRateCurrent;
	//加算する値は移動速度
	float addRate = moveSpeed;
	//addRate *= deltaTime;

	//現在値を更新
	newDampingRate += addRate;
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	//目標値に達したかどうかのチェック
	//ここで、更新後の現在値と目標値との差分値を計算する
	float afterDir;
	afterDir = dampingRateTarget - newDampingRate;

	//差分値がほぼ0のとき
	if (fabsf(afterDir) < FLT_EPSILON) {
		//減衰率を目標値に設定
		newDampingRate = dampingRateTarget;
		//移動速度を0に設定
		moveSpeed = 0.0f;
	}
	else {
		
		//正規化
		afterDir /= fabsf(afterDir);

		//更新前の差分値 * 更新後の差分値　が負の値のとき、更新後の現在値は目標値を超えている
		if (afterDir * originalDir < 0.0f) {
			//目標値を超えているので現在値を目標値に設定
			newDampingRate = dampingRateTarget;
			moveSpeed = 0.0f;
		}

	}
	/////////////////////////////////////////////////////////////////////////


	//更新された減衰率を返す
	return newDampingRate;
}

Vector3& GameCamera::CalcSpringVector(
	const Vector3& currentPos,
	const Vector3& targetPos,
	Vector3& moveSpeed,
	float maxMoveSpeed,
	float dampingRate
)
{
	//1フレームの経過時間を取得
	float deltaTime = g_gameTime->GetFrameDeltaTime();

	//現在位置と目標位置との差分を求める(現在位置　→　目標位置)
	Vector3 distance = targetPos - currentPos;

	//現段階での、現在位置から目標位置への方向を保存しておく
	Vector3 originalDir = distance;
	originalDir.Normalize();

	/////////////////////////////////////////////////////////////////////////
	//目標位置を移動させるための加速度を求める
	Vector3 springAcceleration;
	springAcceleration = distance;

	//F = ma = -Ksx - Kdv
	//m = 1のとき a = -Ksx - Kdv
	// 
	//ダンピング定数と減衰率からばね定数を求める
	float t = DAMPING_CONST / (2.0f * dampingRate);
	float springK = t * t;

	//加速度を求める
	//-Ksx
	//差分値は、現在位置　→　目標位置　なので-1が掛かっている
	//加速度には差分値が入っているのでばね定数を掛ける
	springAcceleration *springK;

	//-Kdv
	Vector3 vt = moveSpeed;
	vt *= DAMPING_CONST;

	//-Ksx - Kdv
	springAcceleration -= vt;

	//加速度に経過時間を掛ける
	springAcceleration *= deltaTime;
	/////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////
	//移動速度を更新
	moveSpeed += springAcceleration;
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	//目標位置を超えたかどうかのチェック
	//更新する予定の座標を作成
	Vector3 newPos = currentPos;
	//座標に加算するベクトルを作成
	Vector3 addPos = moveSpeed;
	//addPos *= deltaTime;

	//座標を更新
	newPos += addPos;

	//更新後の位置と目標位置との差分値を計算
	Vector3 afterDir = targetPos - newPos;
	afterDir.Normalize();

	//最高速度を超えた場合
	if (moveSpeed.LengthSq() > maxMoveSpeed * maxMoveSpeed) {
		//移動速度を最高速度にする
		moveSpeed.Normalize();
		moveSpeed *= maxMoveSpeed;
	}

	//移動速度が負の値になったとき
	if (moveSpeed.Length() < 1.0f) {
		newPos = targetPos;
		moveSpeed *= 0.0f;
	}
	else {
		//更新前の差分値ベクトルと更新後の差分値ベクトルの内積をとり、負の値だったら目標位置を超えている
		if (afterDir.Dot(originalDir) < 0.0f) {
			//更新後の位置を目標位置にする
			newPos = targetPos;
			moveSpeed *= 0.0f;
		}

	}
	/////////////////////////////////////////////////////////////////////////

	//更新後の位置を返す
	return newPos;
}

void GameCamera::UpdateSpringCamera()
{
	//減衰率を計算
	m_dampingRate = CalcDampingRate(m_dampingRate, m_dampingRateTarget, m_dampingRateVel);
	//カメラの座標を計算
	m_cameraPosition = CalcSpringVector(m_cameraPosition, m_cameraPositionTarget, m_cameraMoveSpeedVel, m_maxMoveSpeed, m_dampingRate);
	//注視点の座標を計算
	m_targetPosition = CalcSpringVector(m_targetPosition, m_targetPositionTarget, m_targetMoveSpeedVel, m_maxMoveSpeed, m_dampingRate);

	//カメラの座標と注視点の座標を更新
	SetCameraPosition(m_cameraPosition);
	SetTargetPosition(m_targetPosition);
}

void GameCamera::UpdateCamera()
{
	//ばねカメラの更新
	UpdateSpringCamera();

	//ビュー行列、プロジェクション行列を更新
	g_camera3D->Update();
}