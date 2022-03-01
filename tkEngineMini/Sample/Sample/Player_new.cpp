#include "stdafx.h"
#include "Player_new.h"
#include "Enemy.h"


namespace{
	//const char* MODELPATH_UTC = "Assets/modelData/unityChan.tkm";
	//const char* MODELPATH_UTC = "Assets/modelData/player/player_object.tkm";
	const char* MODELPATH_UTC = "Assets/modelData/player/player_object_c.tkm";
	//const char* MODELPATH_UTC = "Assets/modelData/player_a.tkm";
	const char* SKELETON_PATH_UTC = "Assets/modelData/unityChan.tks";
	const float UTC_RADIUS = 40.0f;
	const float UTC_HEIHGT = 100.0f;
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	
	const Vector3 INIT_POINT = {0.0f,700.0f,0.0f};			//初期位置
	const float UPPER_OFFSET = 10.0f;						//地形から浮かせる距離

	const float PL_MOVE_SPEED = -12.0f;			//移動速度
	const float FIRECOUNTER_NORMAL = 0.075f;	//通常弾の発射間隔
	const float FIRECOUNTER_SPREAD = 0.8f;		//スプレッドボムの発射間隔
	const int INIT_LIFE = 3;					//初期残機
	const float INVINCIBLE_TIME_REVIVE = 7.0f;	//復活時に設定される無敵時間
	const float REVIVE_TIME = 3.0f;				//被弾から復活までの時間

	
	const float CAMERA_ROTATE_FRACTION_ADD_RATE = 0.005f;			//カメラの回転に使う補間係数に加算する定数
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MIN = 0.003f;		//カメラの回転に使う補間係数に加算する定数
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MAX = 0.03f;		//カメラの回転に使う補間係数に加算する定数
	const float CAMERA_MOVESPEED_MAX = 1000.0f;						//カメラ、注視点の追従最高速度 

	//エフェクト関連
	const char16_t* EFFECT_FILEPATH_START = u"Assets/effect/player_born.efk";		//開始エフェクトファイルパス
	const Vector3 EFFECT_SCALE_START = { 50.0f,50.0f,50.0f };

	const char16_t* EFFECT_FILEPATH_EXPLOSION = u"Assets/effect/justguard.efk";		//被弾エフェクトファイルパス
	const Vector3 EFFECT_SCALE_EXPLOSION = { 30.0f,30.0f,30.0f };					//被弾エフェクトの拡大率

	const char16_t* EFFECT_FILEPATH_REVIVE = u"Assets/effect/revive_a.efk";			//復活エフェクトのファイルパス
	const Vector3 EFFECT_SCALE_REVIVE = { 20.0f,20.0f,20.0f };						//復活エフェクトの拡大率

	const char16_t* EFFECT_FILEPATH_TRACK = u"Assets/effect/moveTrack.efk";			//軌跡エフェクトのファイルパス
	const Vector3 EFFECT_SCALE_TRACK = { 10.0f,10.0f,10.0f };						//軌跡エフェクトの拡大率

	const char16_t* EFFECT_FILEPATH_MARKER = u"Assets/effect/positionMarker.efk";	//当たり判定エフェクトのファイルパス
	const Vector3 EFFECT_SCALE_MARKER = { 30.0f,30.0f,30.0f };						//当たり判定エフェクトの拡大率
	const float MARKER_PLAY_INTERVAL = 0.02f;										//当たり判定エフェクトの発生間隔
}

Player_new::Player_new()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
}

Player_new::~Player_new()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_missSe);
	DeleteGO(m_normalShotSe);
}

void Player_new::Init()
{
	//エフェクトを初期化
	InitEffect();

	//SEを初期化
	m_missSe = NewGO<CSoundSource>(0);
	m_missSe->Init(L"Assets/wav/missSe.wav");

	m_normalShotSe = NewGO<CSoundSource>(0);
	m_normalShotSe->Init(L"Assets/wav/normalShotSe_1.wav");

	////ライトを検索
	//m_directionLight = FindGO<DirectionLight>("directionlight");
	//m_pointLight = FindGO<PointLight>("pointlight");
	//m_spotLight = FindGO<SpotLight>("spotlight");

	//弾の管理クラスのポインタを取得
	m_bulletManager = BulletManager::GetInstance();
	
	//モデルを初期化
	m_skinModelRender->Init(
		MODELPATH_UTC, 
		enModelUpAxisZ,
		true,
		false,
		SKELETON_PATH_UTC
	);

	m_position = INIT_POINT;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);

	//////////////////////////////
	//自機ライフの初期化
	m_life = INIT_LIFE;
	//無敵フラグの初期化
	m_isInvincible = false;
	//生存フラグの初期化
	m_isExist = true;
	//////////////////////////////

	//自作キャラコンの初期化
	m_myCharaCon.Init(
		m_position
	);

	//下方向は上方向の反対
	m_downVector = m_up * -1.0f;
	//下方向ベクトルを正規化
	m_downVector.Normalize();

	//前方、右、上の各ベクトルを各軸で初期化
	m_sphericalMove.Init(m_forward, m_right, m_up);
	
	//カメラ注視点から視点へのベクトルを設定
	Vector3 toCamera;
	toCamera.x = 0.0f;
	toCamera.y = 700.0f;
	toCamera.z = 1000.0f;
	/*toCamera.x = 0.0f;
	toCamera.y = 1600.0f;
	toCamera.z = 2400.0f;*/
	toCamera *= 1.5f;

	//カメラの初期化
	m_gameCamera.Init(CAMERA_MOVESPEED_MAX);
	//注視点をプレイヤーの位置に設定
	m_gameCamera.SetTargetPosition(m_position);
	//注視点からの相対位置で視点を設定
	m_gameCamera.SetCameraPosition(m_position + toCamera);

	//注視点目標を設定
	m_gameCamera.SetTargetPositionTarget(m_position);
	//カメラ目標を設定
	m_gameCamera.SetCameraPositionTarget(m_position + toCamera);

	//カメラの上方向を自身の上にしておく
	m_cameraUp = m_up;

	//発射方向を前方にしておく
	//m_shotDirection = m_forward;

	//初期化時に開始演出用エフェクトを発生させる
	m_startEffect.SetScale(EFFECT_SCALE_START);
	m_startEffect.SetPosition(m_position/* + m_up * 50.0f*/);
	m_startEffect.SetRotation(m_rot);
	m_startEffect.Play();
	
}

void Player_new::InitEffect()
{
	m_startEffect.Init(EFFECT_FILEPATH_START);
	m_explosionEffect.Init(EFFECT_FILEPATH_EXPLOSION);
	m_reviveEffect.Init(EFFECT_FILEPATH_REVIVE);
	m_moveTrackEffect.Init(EFFECT_FILEPATH_TRACK);
	m_markerEffect.Init(EFFECT_FILEPATH_MARKER);
}

bool Player_new::Start()
{

	return true;
}

void Player_new::Move()
{
	//m_markerCounter += g_gameTime->GetFrameDeltaTime();

	//パッドのスティックからx成分とy成分を受け取る
	float x = g_pad[0]->GetLStickXF();
	float y = g_pad[0]->GetLStickYF();

	//前方ベクトルを作成
	Vector3 forward;
	//上ベクトルとカメラの右ベクトルの外積を前方ベクトルにする
	forward.Cross(m_up,g_camera3D->GetRight());
	forward.Normalize();

	//プレイヤーの左右方向への移動
	m_moveSpeed = g_camera3D->GetRight() * -x * PL_MOVE_SPEED;
	//m_moveSpeed = m_right * x * PL_MOVE_SPEED;
	//プレイヤーの前後(奥、手前)方向への移動
	m_moveSpeed += forward * y * PL_MOVE_SPEED;

	//ゲーム中以外,ライフが0、存在フラグがオフなら移動速度を0に
	if (m_isExist == false 
		|| GameDirector::GetInstance()->GetPlayerLife() <= 0
		|| GameDirector::GetInstance()->GetGameState() != enGame) {
		
		m_moveSpeed *= 0.0f;
	}

	//軌跡エフェクトの再生
	if (m_moveSpeed.Length() > 5.0f) {
		m_moveTrackEffect.SetPosition(m_position + m_up*50.0f);
		m_moveTrackEffect.SetRotation(m_rot);
		m_moveTrackEffect.SetScale(EFFECT_SCALE_TRACK);
		m_moveTrackEffect.Play(false);

		/*if (m_markerCounter >= MARKER_PLAY_INTERVAL) {
			m_markerEffect.SetPosition(m_position + m_up * 50.0f);
			m_markerEffect.SetRotation(m_rot);
			m_markerEffect.SetScale(EFFECT_SCALE_MARKER);
			m_markerEffect.Play(true);

			m_markerCounter = 0.0f;
		}*/
	}

	//自作キャラコンに移動速度を渡す
	m_position = m_myCharaCon.Execute(m_moveSpeed,m_downVector,m_upperOffset);

	//{
	//	// 上ベクトルを更新
	//	//下向きベクトル(=レイを飛ばす方向)* -1.0　= プレイヤーの上ベクトル
	//	Vector3 newUp = m_downVector * -1.0f;
	//	// 現在の上ベクトルから、新しい上ベクトルに向けるための回転クォータニオンを計算
	//	//		→　カメラの計算で使う。
	//	m_rotUpToGroundNormal.SetRotation(m_up, newUp);

	//	//自身の上ベクトルを更新
	//	m_up = newUp;

	//	//更新した上ベクトルと前方ベクトルの外積　=　右ベクトル
	//	//m_right = g_camera3D->GetRight();
	//	m_right.Cross(m_up, m_forward);
	//	//求めた右ベクトルと更新した上ベクトルの外積　=　前方ベクトル
	//	m_forward.Cross(m_right, m_up);
	//}
	//※上記まとめ
	Vector3 oldUp = m_up;
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);
	m_rotUpToGroundNormal.SetRotation(oldUp, m_up);
	
	//モデルの座標更新
	m_skinModelRender->SetPosition(m_position);
}

void Player_new::Rotation()
{
	//前方、右、上から回転を計算する
	m_sphericalMove.Rotation(m_forward, m_right, m_up, m_rot);

	//スティックによる回転処理
	float stickX, stickY = 0.0f;
	stickX = g_pad[0]->GetLStickXF();
	stickY = g_pad[0]->GetLStickYF();

	//入力があったら回転する
	if (fabs(stickX) >= 0.001f || fabsf(stickY) >= 0.001f){
		m_angleAxisUp = atan2f(stickX, stickY);
		m_rotAxisUp.SetRotation(m_up, m_angleAxisUp);
	}

	//クォータニオンを合成し、前方ベクトルを回転
	m_rot.Multiply(m_rotAxisUp);
	m_rotAxisUp.Apply(m_forward);

	//モデルに回転を適用する
	m_skinModelRender->SetRotation(m_rot);
}

void Player_new::RotateShotDirection()
{
	//ショットの方向はプレイヤーの前方
	//m_shotDirection = m_forward;
	//発射方向のベース
	m_shotDirection = g_camera3D->GetForward();

	//回転軸は上ベクトル
	Vector3 axis = m_up;
	
	//軸周りの回転クォータニオンを作成
	Quaternion rot;

	//右スティックの入力を受け取り
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//入力値から角度を求める
	float angle = atan2f(x, y);

	//軸周りの回転を求める
	rot.SetRotation(axis, angle);

	//ベクトルを回転
	rot.Apply(m_shotDirection);
}

void Player_new::FireBullet()
{
	//ゲーム中以外、ライフが0、存在フラグがオフならreturn
	if (m_isExist == false 
		|| GameDirector::GetInstance()->GetPlayerLife() <= 0
		|| GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//R1ボタンを押すと発射、押しっぱなしで連射
	if (g_pad[0]->IsPress(enButtonRB1)) {
		
		//右スティック方向にショットを撃つ
		//カウンターが0のときとカウンターが一定値を超えると発射
		if (m_fireCounter > FIRECOUNTER_NORMAL || m_fireCounter == 0.0f) {
			
			//ショットSEの再生
			if (m_enBulletType == enNormalShot) {
				CSoundSource* ssNormalSe = NewGO<CSoundSource>(0);
				ssNormalSe->Init(L"Assets/wav/normalShotSe_1.wav");
				ssNormalSe->SetVolume(0.4f);
				ssNormalSe->Play(false);

				/*m_normalShotSe->SetVolume(0.4f);
				m_normalShotSe->Play(false);*/

			}

			//弾管理クラスの関数を使用して出現させる
			m_bulletManager->InitBullets(
				m_position,
				m_up,
				m_shotDirection,
				m_enBulletType
			);
			
			/*m_bullet.push_back(NewGO<Bullet>(0, "bullet"));
			
			m_bullet[m_bullet.size()-1]->Init(
				*RenderingEngine::GetInstance(),
				m_position,
				m_up,
				m_shotDirection,
				m_enBulletType
			);*/

			/*m_bullet = NewGO<Bullet>(0, "bullet");

			m_bullet->Init(
				*RenderingEngine::GetInstance(),
				m_position,
				m_up,
				m_shotDirection,
				m_enBulletType
			); */

			//発射後、カウンターを0にリセット
			m_fireCounter = 0.0f;

		}
	}

	//R1ボタン押下中、カウンターを加算、押していないときはカウンターを0にする。
	if (g_pad[0]->IsPress(enButtonRB1)) {
		m_fireCounter += g_gameTime->GetFrameDeltaTime();
	}
	else{
		m_fireCounter = 0.0f;
	}

}

void Player_new::ChangeWeapon()
{
	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		switch (m_enUseWeapon)
		{
		case enNormalShot:
			m_enUseWeapon = enSpreadBomb;
			m_enBulletType = enPlayerSpreadBomb;
			break;
		case enSpreadBomb:
			m_enUseWeapon = enNormalShot;
			m_enBulletType = enPlayerNormal;
		default:
			break;
		}
	}
}

void Player_new::Hit()
{
	//プレイヤーのライフが0のときは処理しない
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		return;
	}

	//被弾判定
	//エネミーとの判定
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		//距離を計算
		Vector3 diff = enemy->GetPosition() - m_position;
		float length = diff.Length();

		//エネミーに接触したとき
		if (length < 60.0f) {
			
			//自身が無敵状態でなく、敵の当たり判定が有効であれば
			if (m_isInvincible == false && enemy->GetIsActive() == true) {
				//1機減らす
				m_life -= 1;
				//ゲームディレクターの保持するライフを減らす
				GameDirector::GetInstance()->DecPlayerLife();
				
				//描画フラグをオフにする
				if (m_skinModelRender->GetIsDraw() == true) {
					m_skinModelRender->SetIsDraw(false);
				}

				//生存フラグをオフ
				SetIsExist(false);

				//無敵状態にする
				SetInvincibleTime(INVINCIBLE_TIME_REVIVE);
				SetIsInvFlag(true);

				//ミス時のseを再生
				CSoundSource* ssMissSe = NewGO<CSoundSource>(0);
				ssMissSe->Init(L"Assets/wav/missSe.wav");
				ssMissSe->SetVolume(1.0f);
				ssMissSe->Play(false);
				/*m_missSe->SetVolume(0.8f);
				m_missSe->Play(false);*/


				//爆散エフェクトを発生
				m_explosionEffect.Init(EFFECT_FILEPATH_EXPLOSION);
				m_explosionEffect.SetPosition(m_position);
				m_explosionEffect.SetRotation(m_rot);
				m_explosionEffect.SetScale(EFFECT_SCALE_EXPLOSION);
				m_explosionEffect.Play();

				return false;
			}
		}
		return true;
	});

	//敵の爆発との判定
	QueryGOs<Explosion>("enemyExplosion", [&](Explosion* enemyExplosion) {
		//距離を計算
		Vector3 diff = enemyExplosion->GetPosition() - m_position;
		float length = diff.Length();

		//爆風に接触したとき
		if (length < enemyExplosion->GetDamageArea()) {

			//自身が無敵状態でなければ
			if (m_isInvincible == false) {
				//1機減らす
				m_life -= 1;
				//ゲームディレクターの保持するライフを減らす
				GameDirector::GetInstance()->DecPlayerLife();

				//描画フラグをオフにする
				if (m_skinModelRender->GetIsDraw() == true) {
					m_skinModelRender->SetIsDraw(false);
				}

				//生存フラグをオフ
				SetIsExist(false);

				//無敵状態にする
				SetInvincibleTime(INVINCIBLE_TIME_REVIVE);
				SetIsInvFlag(true);

				//被弾エフェクトを発生
				m_explosionEffect.SetPosition(m_position);
				m_explosionEffect.SetRotation(m_rot);
				m_explosionEffect.SetScale(EFFECT_SCALE_EXPLOSION);
				m_explosionEffect.Play();

				return false;
			}
		}
		return true;
	});

}

void Player_new::DecInvTime()
{
	//無敵時間を減少
	m_invincebleTime -= g_gameTime->GetFrameDeltaTime();
	//無敵時間が切れたら無敵フラグをオフにする
	if (m_invincebleTime <= 0.0f) {
		m_invincebleTime = 0.0f;
		m_isInvincible = false;
	}
}

void Player_new::AddReviveCouter()
{
	//被弾していたら復活までのカウンターを加算
	if (m_isExist == false) {
		m_reviveCounter += g_gameTime->GetFrameDeltaTime();
	}
	
	if (m_reviveCounter >= REVIVE_TIME) {
		//存在フラグをオン
		m_isExist = true;
		//カウンターをリセット
		m_reviveCounter = 0.0f;
	}

}

void Player_new::Revive()
{
	//ゲームディレクター側のライフが0なら復活しない
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		return;
	}

	if (m_isExist == true
		&& m_isExistPrev == false) {

		//復活準備はすでに完了しているのでフラグをオフ
		m_isReviveReady = false;

		//モデルの描画フラグをオン
		if (m_skinModelRender->GetIsDraw() == false) {
			m_skinModelRender->SetIsDraw(true);
		}

	}
}

void Player_new::ReviveReady()
{
	//ゲームディレクター側のライフが0なら処理しない
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		return;
	}

	if (m_invincebleTime < 5.0f
		&& m_isExist == false) {
		//復活準備フラグをオン
		m_isReviveReady = true;
	}

	if (m_isReviveReadyPrev == false
		&& m_isReviveReady == true) {
		
		//復活エフェクトを再生
		m_reviveEffect.SetPosition(m_position);
		m_reviveEffect.SetRotation(m_rot);
		m_reviveEffect.SetScale(EFFECT_SCALE_REVIVE);
		m_reviveEffect.Play();
	}
}

void Player_new::CalcCameraUpFractionAddRate()
{
	//プレイヤーの上方向とカメラの上方向の内積から、カメラの上方向を回転させる補間率に足す値を調整する処理
	//内積が0に近いほど足す値は大きくなり(速く回転する)、内積が1に近いほど足す値は小さくなる(ゆっくり回転する)

	//足す値の最大値と最小値を作る
	float addRate = 0.0f;
	float maxAddRate = CAMERA_ROTATE_FRACTION_ADD_RATE_MAX;
	float minAddRate = CAMERA_ROTATE_FRACTION_ADD_RATE_MIN;
	//一度ベクトルに格納する
	Vector3 addRateVec = { 0.0f,0.0f,0.0f, };
	Vector3 maxAddRateVec = { maxAddRate,0.0f,0.0f };
	Vector3 minAddRateVec = { minAddRate,0.0f,0.0f };

	//プレイヤーの上方向とカメラの上方向で内積をとる
	float dotVec = Dot(m_cameraUp, m_up);
	//1を足し2で割ることで-1.0f～1.0fを0.0f～1.0fに変換する
	dotVec += 1.0f;
	dotVec /= 2.0f;
	//内積の結果が1に近いほど足す値が小さくなる
	addRateVec.Lerp(dotVec, maxAddRateVec * 0.9f, minAddRateVec * 0.5f);
	//ベクトルからfloat値に変換
	addRate = addRateVec.Length();

	//カメラの上を補完する係数を加算
	m_cameraUpFraction += addRate;
	//1を超えたら1に補正
	if (m_cameraUpFraction > 1.0f) {
		m_cameraUpFraction = 1.0f;
	}

}

void Player_new::Update()
{
	//ゲーム中以外なら処理しない
	if (GameDirector::GetInstance()->GetGameState() != enGame &&
		GameDirector::GetInstance()->GetGameState() != enStart) {
		return;
	}

	CalcCameraUpFractionAddRate();
	Move();
	Rotation();
	RotateShotDirection();
	ChangeWeapon();
	FireBullet();
	Hit();
	AddReviveCouter();
	ReviveReady();
	Revive();
	DecInvTime();

	//前フレームの上方向が現フレームの上と変わっていたら補間係数を0にする
	if (m_upPrev.x != m_up.x
		|| m_upPrev.y != m_up.y
		|| m_upPrev.z != m_up.z) {
		m_cameraUpFraction = 0.0f;
	}

	//カメラ追従
	////カメラ注視点から視点へのベクトルを作成
	//Vector3 toCamera = m_gameCamera.GetCameraPosition() - m_gameCamera.GetTargetPosition();
	//注視点目標からカメラ目標へのベクトルを作成
	Vector3 toCamera = m_gameCamera.GetCameraPositionTarget() - m_gameCamera.GetTargetPositionTarget();

	//ベクトルにクォータニオンを適用
	m_rotUpToGroundNormal.Apply(toCamera);
	/*m_cameraRot.Apply(toCamera);
	m_cameraRot.Apply(toCamera);*/
	//mulRot.Apply(toCamera);

	////注視点を自身に設定
	//m_gameCamera.SetTargetPosition(m_position);
	//注視点目標を自身に設定
	m_gameCamera.SetTargetPositionTarget(m_position);

	////視点を設定
	//m_gameCamera.SetCameraPosition(m_position + toCamera);
	//カメラ目標を設定
	m_gameCamera.SetCameraPositionTarget(m_position + toCamera);

	// カメラの上方向目標をプレイヤーの上方向に設定。
	m_gameCamera.SetUpVectorTarget(m_up);

	//カメラの上を少しずつ補間していく
	m_gameCamera.LerpUpVector(m_cameraUpFraction, m_cameraUp);
	//補間したカメラの上でカメラの上を更新
	m_gameCamera.SetUp(m_cameraUp);
	//カメラの更新
	m_gameCamera.UpdateCamera();


	//各種フラグの記録
	//////////////////////////////////
	//現フレームの上を記録
	m_upPrev = m_up;
	//現フレームの無敵フラグを記録
	m_isInvinciblePrev = m_isInvincible;
	//現フレームの存在フラグを記録
	m_isExistPrev = m_isExist;
	m_isReviveReadyPrev = m_isReviveReady;

	//エフェクトの更新
	//m_startEffect.SetScale(EFFECT_SCALE_START);
	m_startEffect.SetPosition(m_position + m_up * 50.0f);
	//m_startEffect.SetRotation(m_rot);

	m_startEffect.Update();
	m_explosionEffect.Update();
	m_reviveEffect.Update();
	m_moveTrackEffect.Update();
	m_markerEffect.Update();

}