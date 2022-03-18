#include "stdafx.h"
#include "EnemyBase.h"

namespace {
	float UPPER_OFFSET = 50.0f;
	const float ACTIVATE_COUNT = 0.7f;		//当たり判定が有効になるまでのカウンター
}

EnemyBase::EnemyBase()
{
	//発生したらエネミー数に+1
	GameDirector::GetInstance()->AddEnemyCount();
	//エネミー総数に+1
	GameDirector::GetInstance()->AddEnemyTotalCount();
}

void EnemyBase::Init(const Vector3& initPoint, const Vector3& initUp)
{
	//モデルをNewGO
	m_skinModelRender = NewGO<SkinModelRender>(0);

	const char* modelPath = "hoge";

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
	//キャラコンの処理
	m_myCharaCon.Execute(m_moveSpeed, m_downVector, UPPER_OFFSET);

	//前方、右、上の各ベクトルを各軸で初期化
	m_sphericalMove.Init(m_forward, m_right, m_up);

	//生存フラグをオン
	m_isExist = true;
	//無敵状態フラグをオフ
	m_isInvincible = false;

	//出現から当たり判定が有効になるまでの時間をセット
	m_toActivateCounter = ACTIVATE_COUNT;

	//継承先で決めた処理
	InitSub();
	
}

void EnemyBase::Update()
{
	//ゲーム中以外なら処理しない
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	Move();
	Rotation();
	Hit();
	DecInvTime();
	DecToActivateTime();
	Destroy();
	DestroyTimeUp();

	//継承先で決めた処理
	UpdateSub();
}
