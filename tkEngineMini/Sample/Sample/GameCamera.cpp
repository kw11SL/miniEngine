#include "stdafx.h"
#include "GameCamera.h"

bool GameCamera::Start()
{
	return true;
}


void GameCamera::Init()
{
	//カメラの設定
	g_camera3D->SetPosition({ 0.0f,500.0f,200.0f });
	g_camera3D->SetTarget({ 0.0f,500.0f,0.0f });

	//ビュー行列、プロジェクション行列を更新
	g_camera3D->Update();
}

void GameCamera::SetCameraPosition(const Vector3& pos)
{
	m_cameraPosition = pos;
	g_camera3D->SetPosition(m_cameraPosition);

	/*m_cameraPositionTarget = pos;
	g_camera3D->SetPosition(m_cameraPosition);*/

}

void GameCamera::SetCameraPositionTarget(const Vector3& pos)
{
	m_cameraPositionTarget = pos;
}

void GameCamera::SetTargetPosition(const Vector3& pos)
{
	m_targetPosition = pos;
	g_camera3D->SetTarget(m_targetPosition);
}
void GameCamera::SetUp(const Vector3& up)
{
	g_camera3D->SetUp(up);
}

void GameCamera::Chase()
{
	Vector3 posToTarget = m_cameraPositionTarget - m_cameraPosition;
	posToTarget.Normalize();

	m_cameraPosition += posToTarget * m_moveSpeed;

	//g_camera3D->SetPosition(m_cameraPosition);
}

void GameCamera::Update()
{
	//ビュー行列、プロジェクション行列を更新
	g_camera3D->Update();
}