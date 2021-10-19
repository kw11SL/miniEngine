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
	
	/// @brief 注視点を更新
	/// @param pos 注視点
	void SetTargetPosition(const Vector3& pos);

	/// @brief 視点の座標を取得
	/// @return 座標
	const Vector3& GetCameraPosition() { return m_cameraPosition; }
	
	/// @brief 注視点の座標を取得
	/// @return 座標
	const Vector3& GetTargetPosition() { return m_targetPosition; }

	

private:
	Vector3 m_cameraPosition = Vector3::Zero;				//視点
	Vector3 m_targetPosition = Vector3::Zero;				//注視点

	Quaternion m_rotH = Quaternion::Identity;				//水平回転
	Quaternion m_rotV = Quaternion::Identity;				//垂直回転

	float m_angleH = 0.0f;									//水平回転角度
	float m_angleV = 0.0f;									//垂直回転角度


};

