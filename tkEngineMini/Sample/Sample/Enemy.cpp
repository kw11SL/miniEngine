#include "stdafx.h"
#include "Enemy.h"
#include "Player_new.h"

namespace {
	//モデル毎のファイルパス
	const char* MODELPATH_COMMON = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_POWERED = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_CHASER = "Assets/modelData/enemy/enemy_common.tkm";
	const char* MODELPATH_SHOT = "Assets/modelData/enemy/enemy_shot.tkm";
	const char* MODELPATH_BOMB = "Assets/modelData/enemy/enemy_bomb.tkm";

	const float UPPER_OFFSET = 50.0f;
	
	//エネミーのタイプ毎の移動速度
	const float MOVE_SPEED_COMMON = 3.0f;
	const float MOVE_SPEED_POWERED = 5.0f;
	const float MOVE_SPEED_CHASER = 10.0f;
	const float MOVE_SPEED_SHOT = 2.0f;
	const float MOVE_SPEED_BOMB = 2.0f;

	//エネミーのタイプ毎の耐久値
	const float LIFE_COMMON = 1.0f;
	const float LIFE_POWERED = 1.0f;
	const float LIFE_CHASER = 1.0f;
	const float LIFE_SHOT = 1.0f;
	const float LIFE_BOMB = 10.0f;

	//エネミーのタイプ毎の弾への影響値
	const float DURABILITY_COMMON = 1.0f;
	const float DURABILITY_POWERED = 1.0f;
	const float DURABILITY_CHASER = 1.0f;
	const float DURABILITY_SHOT = 1.0f;
	const float DURABILITY_BOMB = 1.0f;

	//エネミーのタイプ別スコア
	const int SCORE_COMMON = 100;
	const int SCORE_POWERED = 500;
	const int SCORE_CHASER = 300;
	const int SCORE_SHOT = 300;
	const int SCORE_BOMB = 200;

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

Enemy::Enemy()
{
	GameDirector::GetInstance()->AddEnemyCount();
}

Enemy::~Enemy()
{
	GameDirector::GetInstance()->DecEnemyCount();
	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	return true;
}

void Enemy::Init(
	RenderingEngine& renderingEngine,
	const Vector3& initPoint,
	const Vector3& initUp,
	const EnEnemyType& enemyType)
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

	const char* modelPath = "hoge";
	
	m_enEnemyType = enemyType;

	switch (m_enEnemyType) {
	case enCommon:
		modelPath = MODELPATH_COMMON;
		m_life = LIFE_COMMON;
		m_speed = MOVE_SPEED_COMMON;
		m_score = SCORE_COMMON;
		m_durability = DURABILITY_COMMON;

		break;
	case enPowered:
		modelPath = MODELPATH_POWERED;
		m_life = LIFE_POWERED;
		m_speed = MOVE_SPEED_POWERED;
		m_score = SCORE_POWERED;
		m_durability = DURABILITY_POWERED;
		break;
	case enChaser:
		modelPath = MODELPATH_CHASER;
		m_life = LIFE_CHASER;
		m_speed = MOVE_SPEED_CHASER;
		m_score = SCORE_CHASER;
		m_durability = DURABILITY_CHASER;
		break;
	case enShot:
		modelPath = MODELPATH_SHOT;
		m_life = LIFE_SHOT;
		m_speed = MOVE_SPEED_SHOT;
		m_score = SCORE_SHOT;
		m_durability = DURABILITY_SHOT;
		break;
	case enBomb:
		modelPath = MODELPATH_BOMB;
		m_life = LIFE_BOMB;
		m_speed = MOVE_SPEED_BOMB;
		m_score = SCORE_BOMB;
		m_durability = DURABILITY_COMMON;
		m_durability = DURABILITY_BOMB;
		break;
	default:
		break;
	}

	m_skinModelRender->Init(modelPath, enModelUpAxisZ, renderingEngine, true, false);

	m_position = initPoint;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	//各種ライトの受け取り
	//ライトを検索して受け取り
	m_directionLight = FindGO<DirectionLight>("directionlight");
	m_pointLight = FindGO<PointLight>("pointlight");
	m_spotLight = FindGO<SpotLight>("spotlight");
	
	RecieveDirectionLight(m_directionLight);
	RecievePointLight(m_pointLight);
	RecieveSpotLight(m_spotLight);

	//モデルを更新
	InitModelFromInitData();

	//上方向を設定
	m_up = initUp;

	//自作キャラコンの初期化
	m_myCharaCon.Init(
		m_position
	);

	//下方向ベクトルは上方向の反対
	m_downVector = m_up * -1.0f;
	//下方向ベクトルを正規化
	m_downVector.Normalize();


	//前方、右、上の各ベクトルを各軸で初期化
	m_sphericalMove.Init(m_forward, m_right, m_up);
	
	//生存フラグをオン
	m_exist = true;

	//無敵状態フラグをオフ
	m_isInvincible = false;

	//エフェクトの初期化
	m_destroyEffect.Init(u"Assets/effect/destroy.efk");


}

void Enemy::Move()
{
	//プレイヤー追跡処理
	Vector3 toPlayer = Vector3::Zero;
	float toPlayerLength = 0.0f;

	//プレイヤーを検索
	if (m_player == nullptr) {
		m_player = FindGO<Player_new>("player");
	}

	//プレイヤーへのベクトルを取り、その方向を移動方向にする
	if(m_player != nullptr) {

		toPlayer = m_player->GetPosition() - m_position;
		toPlayerLength = toPlayer.Length();
		toPlayer.Normalize();

		m_moveSpeed = toPlayer * m_speed;
	}

	Vector3 playerToMe = m_position - m_player->GetPosition();
	
	//プレイヤーの位置までのベクトルを取り、一定距離まで近づくと速度を0にする
	if (toPlayerLength < 300.0f){
		m_speed = 0.0f;
	}
	else if (toPlayerLength >= 300.0f && m_enEnemyType == enCommon) {
		m_speed = MOVE_SPEED_COMMON;
	}

	//if (toPlayer.Dot(playerToMe) < 0.0f) {
	//	//m_position = m_player->GetPosition();
	//	OutputDebugStringA("hoge");
	//}
	//else {
	//	OutputDebugStringA("true");
	//}

	/*m_moveSpeed = m_right * m_speed * 0.0f;
	m_moveSpeed += m_forward * m_speed;*/

	//キャラコンによる座標更新
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector,UPPER_OFFSET);
	//上方向を球面の法線で更新し、右と前方を更新
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//モデルの座標を更新
	m_skinModelRender->SetPosition(m_position);
}

void Enemy::Rotation()
{
	m_sphericalMove.Rotation(m_forward,m_right,m_up,m_rot);
}

void Enemy::Hit()
{
	//弾を検索
	QueryGOs<Bullet>("bullet", [&](Bullet* bullet) {
		Vector3 diff = bullet->GetPosition() - m_position;
		float length = diff.Length();
		
		//距離が一定値以下のとき
		if (length < 50.0f) {
			
			//無敵状態でなければ
			if (m_isInvincible == false) {
				//耐久力を減らす
				m_life -= bullet->GetPower();
				//エネミーに無敵時間を設定
				SetInvincibleTime(bullet->GetDamageInterval());
				m_isInvincible = true;
			}
			
			//弾の耐久値を減らす
			bullet->DecLife(m_durability);
			
			//問い合わせ終了
			return false;
		}
		
		//問い合わせ続行
		return true;
	});

	//爆発を検索
	QueryGOs<Explosion>("explosion", [&](Explosion* explosion) {
		Vector3 diff = explosion->GetPosition() - m_position;
		float length = diff.Length();

		//爆発の範囲内のとき
		if (length < explosion->GetDamageArea()) {

			//無敵状態でなければ
			if (m_isInvincible == false) {
				//耐久力を減らす
				m_life -= explosion->GetPower();
				//エネミーに無敵時間を設定
				SetInvincibleTime(explosion->GetDamageInterval());
				m_isInvincible = true;
			}

			//問い合わせ終了
			return false;
		}

		//問い合わせ続行
		return true;
	});

}

void Enemy::Destroy()
{
	if (m_life <= 0.0f) {
		m_life = 0.0f;
		
		//生存フラグをオフ
		m_exist = false;
	}
	//生存フラグがオフならエフェクトを出して削除処理
	if (m_exist == false) {
		m_destroyEffect.SetPosition(m_position);
		m_destroyEffect.SetRotation(m_rot);
		m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });
		m_destroyEffect.Play(false);
		
		DeleteGO(this);

		//点数を加点
		GameDirector::GetInstance()->AddScore(m_score);
	}

}

void Enemy::DecInvTime()
{
	//無敵時間を減少
	m_invTime -= g_gameTime->GetFrameDeltaTime();

	
	//0以下なら無敵状態をオフ
	if(m_invTime <= 0.0f){
		m_invTime = 0.0f;
		m_isInvincible = false;
	}

}

void Enemy::Update()
{
	Move();
	Rotation();
	Hit();
	DecInvTime();
	Destroy();

	m_destroyEffect.Update();

	m_skinModelRender->SetRotation(m_rot);

}