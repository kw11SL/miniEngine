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

	const float PL_MOVE_SPEED = -10.0f;
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

	//下方向ベクトルを正規化
	m_downVector.Normalize();

	//前方、右、上の各ベクトルを各軸で初期化
	//前方向をz軸
	m_forward = g_vec3AxisZ;
	//右方向をx軸
	m_right = g_vec3AxisX;
	//上方向をy軸
	m_up = g_vec3AxisY;

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
	//パッドのスティックからx成分とy成分を受け取る
	float x = g_pad[0]->GetLStickXF();
	float y = g_pad[0]->GetLStickYF();

	//プレイヤーの左右方向への移動
	m_moveSpeed = m_right * x * PL_MOVE_SPEED;
	//プレイヤーの前後(奥、手前)方向への移動
	m_moveSpeed += m_forward * y * PL_MOVE_SPEED;

	//下方向ベクトルの座標更新
	//m_downVector.x = m_position.x;
	//m_downVector.z = m_position.z;

	//重力
	//m_moveSpeed.y += g_gameTime->GetFrameDeltaTime() * -10.0f;

	////キャラコンに移動速度を渡す
	//m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//自作キャラコンに移動速度を渡す
	m_position = m_myCharaCon.Execute(m_moveSpeed,m_downVector);

	// 上ベクトルを更新
	//下向きベクトル(=レイを飛ばす方向)* -1.0　= プレイヤーの上ベクトル
	m_up = m_downVector * -1.0f;
	//更新した上ベクトルと前方ベクトルの外積　=　右ベクトル
	m_right.Cross(m_up, m_forward);
	//求めた右ベクトルと更新した上ベクトルの外積　=　前方ベクトル
	m_forward.Cross(m_right, m_up);
	
	
	//モデルレンダーの座標更新
	m_skinModelRender->SetPosition(m_position);
}

void Player_new::Rotation()
{
	// キャラクターの前方、右、上から回転クォータニオンを決める。
	Matrix mRot;
	// 回転行列の1行目は、その座標系のexになる
	mRot.m[0][0] = m_right.x;
	mRot.m[0][1] = m_right.y;
	mRot.m[0][2] = m_right.z;
	
	// 回転行列の1行目は、その座標系のeyになる
	mRot.m[1][0] = m_up.x;
	mRot.m[1][1] = m_up.y;
	mRot.m[1][2] = m_up.z;

	// 回転行列の1行目は、その座標系のezになる
	mRot.m[2][0] = m_forward.x;
	mRot.m[2][1] = m_forward.y;
	mRot.m[2][2] = m_forward.z;

	// 回転行列からクォータニオンを計算する
	m_rot.SetRotation(mRot);
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
	Rotation();
	
	if (m_skinModelRender != nullptr) {
		m_skinModelRender->SetRotation(m_rot);
	}

	//テスト：削除処理
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		 DeleteGO(m_skinModelRender);
	}

}