#include "stdafx.h"
#include "Player_new.h"
//#include "MyCharacterController.h"

namespace{
	const char* MODELPATH_UTC = "Assets/modelData/unityChan.tkm";
	const float UTC_RADIUS = 40.0f;
	const float UTC_HEIHGT = 100.0f;
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	const Vector3 INIT_POINT = {0.0f,700.0f,0.0f};

	const float CHARACON_RADIUS = 50.0f;
	const float CHARACON_HEIGHT = 120.0f;
}

Player_new::~Player_new()
{
	DeleteGO(m_skinModelRender);
}

void Player_new::Init(RenderingEngine& renderingEngine)
{

	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init(MODELPATH_UTC, enModelUpAxisZ,renderingEngine,true,false);

	//m_skinModelRender->InitShader(MODEL_SHADER_PATH, VS_ENTRYPOINT_NAME);

	m_position = INIT_POINT;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	////キャラコンの初期化
	//m_charaCon.Init(
	//	CHARACON_RADIUS,
	//	CHARACON_HEIGHT,
	//	m_position
	//);

	//自作キャラコンの初期化
	m_myCharaCon.Init(
		CHARACON_RADIUS,
		CHARACON_HEIGHT,
		m_position
	);

}

bool Player_new::Start()
{

	return true;
}

void Player_new::SetPostion(const Vector3& pos)
{
	m_position.x = pos.x;
	m_position.y = pos.y;
	m_position.z = pos.z;

	m_skinModelRender->SetPosition(m_position);
}

void Player_new::SetScale(const Vector3& scale)
{
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;

	m_skinModelRender->SetScale(m_scale);
}

void Player_new::SetRotation(const Quaternion rot)
{
	m_rot = rot;
}
void Player_new::SetAngle(const float& angle)
{
	m_angle = angle;
}

void Player_new::Move()
{
	//テスト：移動
	float x = g_pad[0]->GetLStickXF();
	float y = g_pad[0]->GetLStickYF();


	m_moveSpeed.x = x * -10.0f;
	m_moveSpeed.z = y * -10.0f;

	//下方向ベクトルの座標更新
	m_downVector.x = m_position.x;
	m_downVector.z = m_position.z;

	//重力
	//m_moveSpeed.y += g_gameTime->GetFrameDeltaTime() * -10.0f;

	////キャラコンに移動速度を渡す
	//m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//自作キャラコンに移動速度を渡す
	m_position = m_myCharaCon.Execute(m_moveSpeed,m_downVector);


	m_skinModelRender->SetPosition(m_position);
	
}

void Player_new::Rotation()
{
	m_angle += g_pad[0]->GetLStickXF() * 2.0f;

	if (m_angle > 360.0f) {
		m_angle = 0.0f;
	}

	m_rot.SetRotationDeg(Vector3::AxisY, m_angle);
}

void Player_new::RecieveDirectionLight(DirectionLight* dirLight)
{
	m_skinModelRender->InitDirectionLight(dirLight);
}

void Player_new::RecievePointLight(PointLight* ptLight)
{
	m_skinModelRender->InitPointLight(ptLight);
}

void Player_new::RecieveSpotLight(SpotLight* spLight)
{
	m_skinModelRender->InitSpotLight(spLight);
}

SkinModelRender* Player_new::GetSkinModelRender()
{
	return m_skinModelRender;
}

void Player_new::InitModelFromInitData()
{
	m_skinModelRender->InitModel();
}

void Player_new::Update()
{
	Move();
	//Rotation();
	if (m_skinModelRender != nullptr) {
		m_skinModelRender->SetRotation(m_rot);
	}

	//テスト：削除処理
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		 DeleteGO(m_skinModelRender);
	}

}