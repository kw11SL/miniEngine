#include "stdafx.h"
#include "Bullet.h"

namespace{
	//モデル毎のファイルパス
	const char* MODELPATH_PLAYER_NORMAL = "Assets/modelData/bullet/bullet.tkm";
	const char* MODELPATH_ENEMY_NORMAL = "Assets/modelData/bullet/bullet.tkm";

	//弾ごとの弾速
	const float MOVE_SPEED_PLAYER_NORMAL = 20.0f;
	const float MOVE_SPEED_ENEMY_NORMAL = 3.0f;

	//エネミーのタイプ毎の耐久値
	const float LIFE_PLAYER_NORMAL = 1.0f;
	const float LIFE_ENEMY_NORMAL = 1.0f;

	//弾の残存時間
	const float LIFETIME_PLAYER_NORMAL = 1.0f;
	const float LIFETIME_ENEMY_NORMAL = 6.0f;

	//シェーダーのファイルパス
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	//シェーダーのエントリーポイント名
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	//スキンモデルのエントリーポイント名
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	
	//初期座標
	const Vector3 INIT_POINT = { 0.0f,700.0f,0.0f };

	const float CHARACON_RADIUS = 50.0f;
	const float CHARACON_HEIGHT = 120.0f;
}


Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
}

bool Bullet::Start()
{
	return true;
}

void Bullet::Init(RenderingEngine& renderingEngine, const Vector3& initPoint,const Vector3& direction)
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

	const char* modelPath = "hoge";

	switch (m_enBulletType) {
	case enPlayerNormal:
		modelPath = MODELPATH_PLAYER_NORMAL;
		m_life = LIFE_PLAYER_NORMAL;
		m_speed = MOVE_SPEED_PLAYER_NORMAL;
		break;
	case enEnemyNormal:
		modelPath = MODELPATH_PLAYER_NORMAL;
		m_life = LIFETIME_ENEMY_NORMAL;
		m_speed = MOVE_SPEED_ENEMY_NORMAL;
		break;

	default:
		break;
	}

	m_skinModelRender->Init(
		modelPath,
		enModelUpAxisZ,
		renderingEngine,
		true,
		false
	);

	//初期位置を決定
	m_position = initPoint;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale * 3.0f);

	//ライトを検索して受け取り
	m_directionLight = FindGO<DirectionLight>("directionlight");
	m_pointLight = FindGO<PointLight>("pointlight");
	m_spotLight = FindGO<SpotLight>("spotlight");
	
	RecieveDirectionLight(m_directionLight);
	RecievePointLight(m_pointLight);
	RecieveSpotLight(m_spotLight);

	//モデルを更新
	InitModelFromInitData();

	//自作キャラコンの初期化
	m_myCharaCon.Init(
		CHARACON_RADIUS,
		CHARACON_HEIGHT,
		m_position
	);

	//下方向ベクトルを正規化
	m_downVector.Normalize();

	//前方、右、上の各ベクトルを各軸で初期化
	m_sphericalMove.Init(m_forward, m_right, m_up);

	//進行方向を指定
	m_direction = direction;
	m_direction.Normalize();
}

void Bullet::Move()
{
	//右ベクトルは発射方向と上方向の外積
	m_right = Cross(m_up, m_direction);
	
	//正面を発射方向で更新(初回のみ)
	if (m_isDecideDirection == false) {
		m_forward += m_direction;
		//発射方向が決まったのでフラグをtrue
		m_isDecideDirection = true;
	}

	//以降、前方を更新しながら進み続ける
	m_moveSpeed = m_forward * m_speed;

	//キャラコンによる座標更新
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector);
	
	//更新前の前方ベクトルを記録
	m_oldForward = m_forward;

	//上方向を球面の法線で更新し、右と前方を更新
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//モデルの座標を更新
	m_skinModelRender->SetPosition(m_position);
}

void Bullet::Rotation()
{
	//モデルの向きを変える処理
	//回転クォータニオンを作成
	Quaternion rot;
	//記録しておいた更新前の前方から更新後の前方に回転するクォータニオンを計算
	rot.SetRotation(m_oldForward, m_forward);
	
	Quaternion mulRot;
	//クォータニオンを乗算
	mulRot.Multiply(m_rot, rot);
	//乗算したクォータニオンでモデルを回転
	m_skinModelRender->SetRotation(mulRot);


	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
}

void Bullet::DecLifeTime()
{
	m_life -= g_gameTime->GetFrameDeltaTime();
	
	if (m_life < 0.0f) {
		DeleteGO(this);
	}
}

void Bullet::Update()
{
	Move();
	Rotation();
	DecLifeTime();

	m_skinModelRender->SetRotation(m_rot);

}