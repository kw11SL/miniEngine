#include "stdafx.h"
#include "Player_new.h"

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

	const float PL_MOVE_SPEED = -15.0f;

	const float FIRECOUNTER = 20.0f;
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

	////前方向をz軸
	//m_forward = g_vec3AxisZ;
	////右方向をx軸
	//m_right = g_vec3AxisX;
	////上方向をy軸
	//m_up = g_vec3AxisY;
	
	//※上記処理まとめ
	//前方、右、上の各ベクトルを各軸で初期化
	m_sphericalMove.Init(m_forward, m_right, m_up);
	
	//カメラ注視点から視点へのベクトルを設定
	Vector3 toCamera;
	toCamera.x = 0.0f;
	toCamera.y = 700.0f;
	toCamera.z = 1000.0f;

	//注視点を設定
	m_gameCamera.SetTargetPosition(m_position);
	//視点を設定
	m_gameCamera.SetCameraPosition(m_position + toCamera);

	//発射方向を前方にしておく
	//m_shotDirection = m_forward;
}

bool Player_new::Start()
{

	return true;
}

void Player_new::Move()
{
	//m_shotDirection = m_forward;

	//テスト：移動
	//パッドのスティックからx成分とy成分を受け取る
	float x = g_pad[0]->GetLStickXF();
	float y = g_pad[0]->GetLStickYF();

	//前方ベクトルを作成
	Vector3 forward;
	//上ベクトルとカメラの右ベクトルの外積を前方ベクトルにする
	forward.Cross(m_up, g_camera3D->GetRight());
	forward.Normalize();

	//プレイヤーの左右方向への移動
	m_moveSpeed = g_camera3D->GetRight() * -x * PL_MOVE_SPEED;
	//プレイヤーの前後(奥、手前)方向への移動
	m_moveSpeed += forward * y * PL_MOVE_SPEED;

	//自作キャラコンに移動速度を渡す
	m_position = m_myCharaCon.Execute(m_moveSpeed,m_downVector);

	// 上ベクトルを更新
	//下向きベクトル(=レイを飛ばす方向)* -1.0　= プレイヤーの上ベクトル
	Vector3 newUp = m_downVector * -1.0f;
	// 現在の上ベクトルから、新しい上ベクトルに向けるための回転クォータニオンを計算
	//		→　カメラの計算で使う。
	m_rotUpToGroundNormal.SetRotation(m_up, newUp);
	
	m_up = newUp;

	//更新した上ベクトルと前方ベクトルの外積　=　右ベクトル
	m_right = g_camera3D->GetRight();
	//求めた右ベクトルと更新した上ベクトルの外積　=　前方ベクトル
	m_forward.Cross(m_right, m_up);
	
	
	//モデルレンダーの座標更新
	m_skinModelRender->SetPosition(m_position);
}

void Player_new::Rotation()
{
	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
}

void Player_new::RotateShotDirection()
{
	//発射方向を上方向とカメラの右の外積にしておく
	m_shotDirection = Cross(m_up, g_camera3D->GetRight());
	m_shotDirection.Normalize();

	//回転軸は上ベクトル
	Vector3 axis = m_up;
	//軸周りの回転クォータニオンを作成
	Quaternion rot;

	//右スティックの入力を受け取り
	float x = g_pad[0]->GetRStickXF() * -1.0f;
	float y = g_pad[0]->GetRStickYF() * -1.0f;

	//入力値から角度を求める
	float angle = atan2f(x, y);

	//軸周りの回転を求める
	rot.SetRotation(m_up, angle);

	//ベクトルを回転
	rot.Apply(m_shotDirection);
	
}

void Player_new::FireBullet()
{
	//RB1を押すと弾を発射
	if (g_pad[0]->IsTrigger(enButtonRB1)) {

		m_bullet = NewGO<Bullet>(0, "bullet");
		m_bullet->Init(
			*RenderingEngine::GetInstance(),
			m_position,
			m_shotDirection
		);
	}
}

void Player_new::Update()
{
	Move();
	Rotation();
	RotateShotDirection();
	FireBullet();
	
	if (m_skinModelRender != nullptr) {
		m_skinModelRender->SetRotation(m_rot);
	}

	//テスト：削除処理
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		 DeleteGO(m_skinModelRender);
	}

	//カメラ追従
	//カメラ注視点から視点へのベクトルを作成
	Vector3 toCamera = g_camera3D->GetPosition() - g_camera3D->GetTarget();
	m_rotUpToGroundNormal.Apply(toCamera);

	//注視点を自身に設定
	m_gameCamera.SetTargetPosition(m_position);
	//視点を設定
	m_gameCamera.SetCameraPosition(m_position + toCamera);
	// カメラの上方向はプレイヤーの上方向と同じ。
	m_gameCamera.SetUp(m_up);
	////それぞれ正規化
	//toCameraTmp.Normalize();
	//upVectorTmp.Normalize();

}