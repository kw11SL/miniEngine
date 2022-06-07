#include "stdafx.h"
#include "EnemyBase.h"
#include "Player_new.h"

namespace {
	float UPPER_OFFSET = 50.0f;				//浮かす高さ
	const float ACTIVATE_COUNT = 0.7f;		//当たり判定が有効になるまでのカウンター
}

EnemyBase::EnemyBase()
{
}

void EnemyBase::Init(const Vector3& initPoint, const Vector3& initUp)
{	
	///////////////////////////////////////////////////////////////////
	// 共通処理
	//モデルをNewGO
	m_skinModelRender = NewGO<SkinModelRender>(0);

	//バレットマネージャのポインタを取得
	m_bulletManager = BulletManager::GetInstance();

	m_position = initPoint;
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	//ライトを検索,受け取り
	m_directionLight = FindGO<DirectionLight>(DIRECTION_LIGHT_NAME);
	m_pointLight = FindGO<PointLight>(POINT_LIGHT_NAME);
	m_spotLight = FindGO<SpotLight>(SPOT_LIGHT_NAME);
	if (m_directionLight != nullptr) { RecieveDirectionLight(m_directionLight); }
	if (m_pointLight != nullptr) { RecievePointLight(m_pointLight); }
	if (m_pointLight != nullptr) { RecieveSpotLight(m_spotLight); }

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

	///////////////////////////////////////////////////////////////////
	//継承先で決めたエフェクト初期化処理
	InitEffect();
	//継承先で決めた初期化処理(モデルの初期化や固有パラメータ設定など)
	InitSub();
	
}

void EnemyBase::InitSub()
{
}

void EnemyBase::Move()
{
	//プレイヤーを検索
	if (m_player == nullptr) {
		m_player = FindGO<Player_new>(PLAYER_NAME);
	}

	//継承先で決めた処理
	MoveSub();

	//キャラコンによる座標更新
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector, UPPER_OFFSET);
	//上方向を球面の法線で更新し、右と前方を更新
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	//モデルの座標を更新
	m_skinModelRender->SetPosition(m_position);
}

void EnemyBase::MoveSub()
{
}

void EnemyBase::Rotation()
{
	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
	//継承先で決めた処理
	RotationSub();
}

void EnemyBase::RotationSub()
{
}

void EnemyBase::Hit()
{
	//自機弾を検索
	QueryGOs<Bullet>("bullet", [&](Bullet* bullet) {
		Vector3 diff = bullet->GetPosition() - m_position;
		float length = diff.Length();

		//距離が一定値以下のとき
		if (length < m_hitRange) {

			//無敵状態でなければ
			if (m_isInvincible == false) {
				//耐久力を減らす
				m_life -= bullet->GetPower();

				//ヒットエフェクトの再生
				m_hitEffect.SetPosition(m_position + m_up * 50.0f);
				m_hitEffect.SetRotation(m_rot);
				m_hitEffect.SetScale({ 10.0f,10.0f,10.0f });
				m_hitEffect.Play(false);

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

void EnemyBase::DecInvTime()
{
	//無敵時間を減少
	m_invTime -= g_gameTime->GetFrameDeltaTime();

	//0以下なら無敵状態をオフ
	if (m_invTime <= 0.0f) {
		m_invTime = 0.0f;
		m_isInvincible = false;
	}
}

void EnemyBase::DecToActivateTime()
{
	m_toActivateCounter -= g_gameTime->GetFrameDeltaTime();

	//自機との接触判定を有効にする
	if (m_toActivateCounter <= 0.0f) {
		m_toActivateCounter = 0.0f;
		m_isActive = true;
	}
}

void EnemyBase::DecLifeTime()
{
	m_lifeTime -= g_gameTime->GetFrameDeltaTime();

	//時間寿命を減少,寿命が0以下だったら自滅処理
	if (m_lifeTime <= 0.0f) {
		m_lifeTime = 0.0f;
		SelfDestroy();
	}
}

void EnemyBase::Destroy()
{
	//ライフが0以下
	if (m_life <= 0.0f) {
		m_life = 0.0f;

		//生存フラグをオフ
		m_isExist = false;
	}

	//生存フラグがオフならエフェクトを出して削除処理
	if (m_isExist == false) {
		m_destroyEffect.SetPosition(m_position);
		m_destroyEffect.SetRotation(m_rot);
		m_destroyEffect.SetScale({ 20.0f,20.0f,20.0f });
		m_destroyEffect.Play(false);

		//爆破seの再生
		CSoundSource* ssDestroy = NewGO<CSoundSource>(0);
		ssDestroy->Init(L"Assets/wav/destroySe.wav");
		ssDestroy->SetVolume(0.6f);
		ssDestroy->Play(false);

		//DeleteGO(this);

		//点数を加点
		GameDirector::GetInstance()->AddScore(m_score);
		//プレイヤーの撃破総数に+1
		GameDirector::GetInstance()->AddDestroyedEnemyCount();
	}
}

void EnemyBase::SelfDestroy()
{
	//継承先で決めた処理
	SelfDestroySub();

	//DeleteGO(this);
	m_isExist = false;
}

void EnemyBase::SelfDestroySub()
{
}

void EnemyBase::DestroyTimeUp()
{
	//タイムアップだったら自滅
	if (GameDirector::GetInstance()->GetTime() <= 0.0f) {
		SelfDestroy();
	}
}

void EnemyBase::FireBulletEqually(const int wayNum, const EnBulletType bulletType)
{
	//数が0だったら実行しない
	if (wayNum == 0) {
		return;
	}

	//プレイヤー方向を格納するベクトルを作成
	Vector3 toPlayer = Vector3::Zero;
	
	
	if (m_player == nullptr) {
		m_player = FindGO<Player_new>(PLAYER_NAME);
	}
	//プレイヤー方向へのベクトルを計算、正規化
	if (m_player != nullptr) {
		toPlayer = m_player->GetPosition() - m_position;
		toPlayer.Normalize();
	}

	Vector3 dir = toPlayer;
	//角度を決めるための回転クォータニオンを作成
	Quaternion rot;

	//指定した分割数で弾と弾の間の角度を計算(ラジアン)
	//π*2(=2π = 1周)を等分している
	float angle = Math::PI * 2.0f / (float)wayNum;

	//for文に足し込む用の角度
	float angleTmp = 0.0f;

	//発射処理
	//弾の角度決定,射撃
	for (int i = 0; i < wayNum; i++) {
		//奇数弾の1発目
		if (i == 0 && wayNum % 2 == 1) {
			//プレイヤー方向に1発目
			rot.SetRotation(m_up, 0.0f);
			rot.Apply(dir);
		}
		//偶数弾の1発目(プレイヤー方向には出さない)
		else if (i == 0 && wayNum % 2 == 0) {
			//プレイヤー方向準拠なので1発目が出る方向は、
			//(分割した角度 / 2)ラジアン回転させた向きとなる
			angleTmp += angle / 2.0f;
			rot.SetRotation(m_up, angleTmp);
			rot.Apply(dir);
		}
		//2発目以降は等分した角度分発射方向を回す
		else {
			angleTmp += angle;
			rot.SetRotation(m_up, angleTmp);
			rot.Apply(dir);
		}

		//弾を生成
		m_bulletManager->InitBullets(
			m_position,
			m_up,
			dir,
			bulletType
		);

		//生成後、向きをプレイヤー方向に戻す(正面を基準に方向を決定しているため)
		dir = toPlayer;
		dir.Normalize();
	}

	//発射カウンターを戻す
	m_shotCounter = 0.0f;
}

void EnemyBase::Update()
{
	//ゲーム中以外なら処理しない
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}
	
	//共通処理
	///////////////////////////////////////////////////
	Move();
	Rotation();
	Hit();
	DecInvTime();
	DecToActivateTime();
	Destroy();
	DestroyTimeUp();
	///////////////////////////////////////////////////

	//継承先で決めた処理
	UpdateSub();
	//継承先で決めたエフェクトの更新処理
	UpdateEffect();

	//モデルの回転を更新
	m_skinModelRender->SetRotation(m_rot);
}

void EnemyBase::UpdateSub()
{

}