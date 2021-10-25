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
	const float LIFETIME_PLAYER_NORMAL = 2.0f;
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

Bullet::Bullet()
{

}

Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
}

bool Bullet::Start()
{
	return true;
}

void Bullet::Init(RenderingEngine& renderingEngine, const Vector3& initPoint)
{
	m_skinModelRender = NewGO<SkinModelRender>(0, "bullet");

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

	m_skinModelRender->Init(modelPath, enModelUpAxisZ, renderingEngine, true, false);

	m_position = initPoint;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

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

}

void Bullet::Move()
{
	//キャラコンによる座標更新
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector);
	
	//上方向を球面の法線で更新し、右と前方を更新
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//モデルの座標を更新
	m_skinModelRender->SetPosition(m_position);
}

void Bullet::Rotation()
{

	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
}

void Bullet::Update()
{
	Move();
	Rotation();

	m_skinModelRender->SetRotation(m_rot);

}