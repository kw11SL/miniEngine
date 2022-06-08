#include "stdafx.h"
#include "BulletBase.h"

namespace{
	const float UPPER_OFFSET = 50.0f;			//地形から浮かせる距離
}

BulletBase::~BulletBase()
{
	//エフェクトを停止
	m_shotEffect.Stop();
}

void BulletBase::Init(
	const Vector3& initPoint,
	const Vector3& initUp,
	const Vector3& direction
)
{
	/////////////////////////////////////////////////////////
	//共通処理
	 
	//初期位置を決定
	m_position = initPoint;

	//キャラコンの初期化
	m_myCharaCon.Init(m_position);

	//球面移動用クラスの初期化
	m_sphericalMove.Init(m_forward, m_right, m_up);

	//上方向を決定
	m_up = initUp;

	//決定した上方向の逆ベクトルが下方向になる
	m_downVector = m_up * -1.0f;
	m_downVector.Normalize();
	
	//前方を決定
	m_direction = direction;
	m_direction.Normalize();
	
	//右ベクトルを発射方向と上方向の外積にする
	m_right = Cross(m_up, m_direction);

	/////////////////////////////////////////////////////////

	//エフェクト初期化処理
	InitEffect();
	//継承先で定義した処理
	InitSub();

}

void BulletBase::InitEffect()
{
	//継承先で決めた処理(初期化、拡大率の設定など)
	InitEffectSub();

	//弾エフェクトの位置、回転を設定
	m_shotEffect.SetPosition(m_position);
	m_shotEffect.SetRotation(m_rot);
	//消滅エフェクトの位置、回転を設定
	m_banishEffect.SetPosition(m_position);
	m_banishEffect.SetRotation(m_rot);
}

void BulletBase::InitEffectSub()
{

}

void BulletBase::InitSub()
{

}

void BulletBase::Move()
{
	////////////////////////////////////////////////////////////////////////////////
	//共通処理

	//正面を発射方向で更新(初回のみ)
	if (m_isDecideDirection == false) {
		m_forward = m_direction;
		m_forward.Normalize();
		//発射方向が決まったのでフラグをtrue
		m_isDecideDirection = true;
	}

	//以降、前方を更新しながら進み続ける
	m_moveSpeed = m_forward * m_speed;

	//キャラコンによる座標更新
	m_position = m_myCharaCon.Execute(m_moveSpeed, m_downVector, UPPER_OFFSET);

	//この時点での前方ベクトルを記録
	m_oldForward = m_forward;

	//ベクトルの向きを変える処理
	//上方向を更新し、右と前方を更新
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);

	////////////////////////////////////////////////////////////////////////////////

	//継承先で決めた移動処理
	MoveSub();

}

void BulletBase::MoveSub()
{

}

void BulletBase::Rotation()
{
	////////////////////////////////////////////////////////////////////////////////
	//共通処理
	
	//エフェクトの向きを変える処理
	//回転クォータニオンを作成
	Quaternion rot;
	//記録しておいた更新前の前方から更新後の前方に回転するクォータニオンを計算
	rot.SetRotation(m_oldForward, m_forward);

	Quaternion mulRot;
	//クォータニオンを乗算
	mulRot.Multiply(m_rot, rot);

	//m_rot.Multiply(m_rot, rot);

	//前方、右、上の各ベクトルを渡し、向きを変える
	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);
	////////////////////////////////////////////////////////////////////////////////

	//継承先で決めた回転処理
	RotationSub();
}

void BulletBase::RotationSub()
{

}

void BulletBase::Hit()
{
	//自身の名前が敵弾のとき、プレイヤーの弾と爆発を検索
	if (m_name == BULLET_ENEMY_NAME) {

		QueryGOs<Bullet>(BULLET_PLAYER_NAME, [&](Bullet* bullet) {
			Vector3 diff = bullet->GetPosition() - m_position;
			float length = diff.Length();

			if (length < 60.0f) {
				//相手側の耐久値を減少
				bullet->DecLife(m_power);
				//問い合わせ終了
				return false;
			}

			//問い合わせ続行
			return true;
		});

		QueryGOs<Explosion>(EXPLOSION_PLAYER_NAME, [&](Explosion* explosion) {
			Vector3 diff = explosion->GetPosition() - m_position;
			float length = diff.Length();

			if (length < explosion->GetDamageArea()) {
				//こちらの耐久値を減少
				DecLife(explosion->GetPower());
				//問い合わせ終了
				return false;
			}

			//問い合わせ続行
			return true;
		});

	}
	//自身の名前が自機弾のとき、敵弾を検索
	else if (m_name == BULLET_PLAYER_NAME) {

		QueryGOs<Bullet>(BULLET_ENEMY_NAME, [&](Bullet* bullet) {
			Vector3 diff = bullet->GetPosition() - m_position;
			float length = diff.Length();

			if (length < 60.0f) {
				//相手側の耐久値を減少
				bullet->DecLife(m_power);
				//問い合わせ終了
				return false;
			}

			//問い合わせ続行
			return true;
		});
	}
}

void BulletBase::DecLifeTime()
{
	//時間寿命を減少
	m_lifeTime -= g_gameTime->GetFrameDeltaTime();

	//弾の耐久値、時間寿命、速さのいずれかが0以下になったとき
	if (m_life <= 0.0f ||
		m_lifeTime <= 0.0f ||
		m_speed <= 0.0f) {

		//存在フラグをオフ
		m_isExist = false;
	}
}

void BulletBase::Destroy()
{
	//存在フラグがオフになったとき
	if (m_isExist == false) {
		//継承先で決めた処理
		DestroySub();
	}
}

void BulletBase::DestroySub()
{

}

void BulletBase::EffectUpdate()
{
	//弾のエフェクトの位置と回転を更新
	m_shotEffect.SetPosition(m_position);
	m_shotEffect.SetRotation(m_rot);

	//再生中でなければ
	if (m_shotEffect.IsPlay() != true) {
		//再生を再開
		m_shotEffect.Play(false);
	}

	//弾エフェクトの更新
	m_shotEffect.Update();
	//消滅エフェクトの更新
	m_banishEffect.Update();

	//継承先で決めた処理
	EffectUpdateSub();
}

void BulletBase::EffectUpdateSub()
{

}


void BulletBase::Update()
{
	//ゲーム中以外なら処理しない
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//移動処理
	Move();
	//回転処理
	Rotation();
	//時間寿命減少&存在フラグオフ処理
	DecLifeTime();
	//当たり判定処理
	Hit();
	//破棄時の処理と消滅エフェクト再生
	Destroy();
	//継承先で決めた処理
	UpdateSub();
	//エフェクトの更新処理
	EffectUpdate();
}

void BulletBase::UpdateSub()
{

}


