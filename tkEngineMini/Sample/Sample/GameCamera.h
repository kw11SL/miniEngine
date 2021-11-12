#pragma once
class GameCamera
{
public:
	GameCamera(){}
	~GameCamera(){}

	/// @brief 初期化処理
	/// @param maxMoveSpeed カメラと注視点が移動する最高速度 
	void Init(const float maxMoveSpeed);
	
	/// @brief 視点を更新
	/// @param pos 視点
	void SetCameraPosition(const Vector3& pos)
	{
		m_cameraPosition = pos;
		g_camera3D->SetPosition(m_cameraPosition);

		/*m_cameraPositionTarget = pos;
		g_camera3D->SetPosition(m_cameraPosition);*/

	}

	/// @brief 視点が追いかける座標を更新
	/// @param pos 
	void SetCameraPositionTarget(const Vector3& pos)
	{
		m_cameraPositionTarget = pos;
	}
	
	/// @brief 注視点を更新
	/// @param pos 注視点
	void SetTargetPosition(const Vector3& pos)
	{
		m_targetPosition = pos;
		g_camera3D->SetTarget(m_targetPosition);
	}

	/// @brief 注視点が追いかける座標を更新
	/// @param pos 
	void SetTargetPositionTarget(const Vector3& pos)
	{
		m_targetPositionTarget = pos;
	}

	void SetUpVectorTarget(const Vector3& upTarget)
	{
		m_upTarget = upTarget;
	}

	/// @brief カメラの座標を取得
	/// @return 座標
	const Vector3& GetCameraPosition() { return m_cameraPosition; }

	/// @brief カメラ目標の座標を取得
	/// @return 
	const Vector3& GetCameraPositionTarget() { return m_cameraPositionTarget; }
	
	/// @brief 注視点の座標を取得
	/// @return 座標
	const Vector3& GetTargetPosition() { return m_targetPosition; }

	/// @brief 注視点目標の座標を取得
	/// @return 
	const Vector3& GetTargetPositionTarget() { return m_targetPositionTarget; }
	
	/// @brief 上方向の目標値を取得
	/// @return 
	const Vector3& GetUpVectorTarget() { return m_upTarget; }

	/// @brief カメラの上ベクトルを設定。
	/// @param up 
	void SetUp(const Vector3& up);

	/// @brief カメラ座標がカメラ目標を追いかける処理
	void ChaseCameraPosition();

	/// @brief 注視点が注視点目標を追いかける処理
	void ChaseTargetPosition();

	/// @brief 受け取ったベクトルをカメラの上の目標ベクトルで線形補完する処理
	/// @param fracton 補間率
	void LerpUpVector(float fracton , Vector3& up);


	/// @brief ばねの減衰率をばねっぽく変化させる処理
	/// @param dampingRate			//減衰率
	/// @param dampingRateTarget	//減衰率目標値
	/// @param moveSpeed			//ばねの移動速度
	/// @return 
	float& CalcDampingRate(
		float dampingRateCurrent,
		float dampingRateTarget,
		float& moveSpeed
	);

	/// @brief ばねの減衰率を使ってばねっぽく座標を更新する処理
	/// @param pos			//実際の座標
	/// @param targetPos	//目標の座標
	/// @param moveSpeed	//速度ベクトル
	/// @param maxMoveSpeed //最高速度
	/// @param dampingRate	//ばねの減衰率
	/// @return				//座標
	Vector3& CalcSpringVector(
		const Vector3& currentPos,
		const Vector3& targetPos,
		Vector3& moveSpeed,
		float maxMoveSpeed,
		float dampingRate
	);

	/// @brief ばねカメラの更新
	void UpdateSpringCamera();

	/// @brief カメラの更新
	void UpdateCamera();

private:
	Vector3 m_cameraPosition = Vector3::Zero;		//カメラ座標
	Vector3 m_targetPosition = Vector3::Zero;		//注視点
	
	Vector3 m_cameraPositionTarget = Vector3::Zero;	//カメラ目標
	Vector3 m_targetPositionTarget = Vector3::Zero;	//注視点目標

	Vector3 m_cameraMoveSpeedVel = Vector3::Zero;	//カメラの移動速度
	Vector3 m_targetMoveSpeedVel = Vector3::Zero;	//注視点の移動速度

	Vector3 m_upTarget = Vector3::Zero;				//カメラの上の目標

	Quaternion m_rotH = Quaternion::Identity;		//水平回転
	Quaternion m_rotV = Quaternion::Identity;		//垂直回転

	float m_angleH = 0.0f;							//水平回転角度
	float m_angleV = 0.0f;							//垂直回転角度

	float m_maxMoveSpeed = 0.0f;					//カメラ、注視点の移動速度の最大値

	float m_cameraMoveSpeed = 0.0f;					//カメラがカメラ目標を追いかける速度
	float m_targetMoveSpeed = 0.0f;					//注視点が注視点目標を追いかける速度

	float m_dampingRate = 1.0f;						//ばねの減衰率。座標が移動する速度を決定するために使用
	float m_dampingRateTarget = 10.0f;				//ばねの減衰率の目標値
	float m_dampingRateVel = 0.0f;					//ばね減衰率が目標値に近づく速度
};

