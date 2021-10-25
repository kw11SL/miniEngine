#pragma once
class GameCamera : public IGameObject
{
public:
	GameCamera(){}
	~GameCamera(){}

	bool Start()override;
	void Update()override;

	/// @brief 初期化処理
	void Init();
	
	/// @brief 視点を更新
	/// @param pos 視点
	void SetCameraPosition(const Vector3& pos);

	void SetCameraPositionTarget(const Vector3& pos);
	
	/// @brief 注視点を更新
	/// @param pos 注視点
	void SetTargetPosition(const Vector3& pos);

	/// @brief 視点の座標を取得
	/// @return 座標
	const Vector3& GetCameraPosition() { return m_cameraPosition; }

	const Vector3& GetCameraPositionTarget() { return m_cameraPositionTarget; }
	
	/// @brief 注視点の座標を取得
	/// @return 座標
	const Vector3& GetTargetPosition() { return m_targetPosition; }
	
	/// @brief カメラの上ベクトルを設定。
	/// @param up 
	void SetUp(const Vector3& up);

	void Chase();

private:
	Vector3 m_cameraPosition = Vector3::Zero;				//視点
	Vector3 m_targetPosition = Vector3::Zero;				//注視点
	Vector3 m_cameraPositionTarget = Vector3::Zero;			//始点が追いかける座標

	Quaternion m_rotH = Quaternion::Identity;				//水平回転
	Quaternion m_rotV = Quaternion::Identity;				//垂直回転

	float m_angleH = 0.0f;									//水平回転角度
	float m_angleV = 0.0f;									//垂直回転角度

	float m_moveSpeed = 2.0f;

};

