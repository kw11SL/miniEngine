#include "stdafx.h"
#include "Enemy.h"
#include "Player_new.h"

namespace {
	//モデル毎のファイルパス
	const char* MODELPATH_COMMON = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_POWERED = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_CHASER = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_BOMB = "Assets/modelData/enemy/enemy_common.tkm";
	
	//エネミーのタイプ毎の移動速度
	const float MOVE_SPEED_COMMON = 3.0f;
	const float MOVE_SPEED_POWERED = 5.0f;
	const float MOVE_SPEED_CHASER = 10.0f;
	const float MOVE_SPEED_BOMB = 2.0f;

	//エネミーのタイプ毎の耐久値
	const float LIFE_COMMON = 1.0f;
	const float LIFE_POWERED = 1.0f;
	const float LIFE_CHASER = 1.0f;
	const float LIFE_BOMB = 1.0f;


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

Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	return true;
}

void Enemy::Init(RenderingEngine& renderingEngine,const Vector3& initPoint)
{
	m_skinModelRender = NewGO<SkinModelRender>(0,"enemy");

	const char* modelPath = "hoge";
	
	switch (m_enEnemyType) {
	case enCommon:
		modelPath = MODELPATH_COMMON;
		m_life = LIFE_COMMON;
		m_speed = MOVE_SPEED_COMMON;
		break;
	case enPowered:
		modelPath = MODELPATH_POWERED;
		m_life = LIFE_POWERED;
		m_speed = MOVE_SPEED_POWERED;
		break;
	case enChaser:
		modelPath = MODELPATH_CHASER;
		m_life = LIFE_CHASER;
		m_speed = MOVE_SPEED_CHASER;
		break;
	case enBomb:
		modelPath = MODELPATH_BOMB;
		m_life = LIFE_BOMB;
		m_speed = MOVE_SPEED_BOMB;
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

void Enemy::Move()
{
	Vector3 toPlayer;

	/*QueryGOs<Player_new>("player", [&](Player_new* player) {
		toPlayer = player->GetPosition() - m_position;
		
		return false;
	});*/

	//const char* plName = "player";

	if (m_player == nullptr) {
		m_player = FindGO<Player_new>("player");
	}
	
	if(m_player != nullptr) {
		toPlayer = m_player->GetPosition() - m_position;
		toPlayer.Normalize();

		m_moveSpeed = toPlayer * m_speed;
	}

	Vector3 playerToMe = m_position - m_player->GetPosition();

	if (toPlayer.Dot(playerToMe) < 0.0f) {
		//m_position = m_player->GetPosition();
		OutputDebugStringA("hoge");
	}
	else {
		OutputDebugStringA("true");
	}

	/*m_moveSpeed = m_right * m_speed * 0.0f;
	m_moveSpeed += m_forward * m_speed;*/

	//キャラコンによる座標更新
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector);

	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	m_skinModelRender->SetPosition(m_position);
}

void Enemy::Rotation()
{
	m_sphericalMove.Rotation(m_forward,m_right,m_up,m_rot);
}

void Enemy::Update()
{
	Move();
	Rotation();

	m_skinModelRender->SetRotation(m_rot);

}