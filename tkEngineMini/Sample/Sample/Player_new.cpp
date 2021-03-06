#include "stdafx.h"
#include "Player_new.h"
#include "Enemy.h"


namespace{
	const char* MODELPATH_UTC = "Assets/modelData/player/player_object_c.tkm";
	const char* SKELETON_PATH_UTC = "Assets/modelData/unityChan.tks";
	const float UTC_RADIUS = 40.0f;
	const float UTC_HEIHGT = 100.0f;
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	
	const Vector3 INIT_POINT = {0.0f,700.0f,0.0f};			//初期位置
	const float UPPER_OFFSET = 20.0f;						//地形から浮かせる距離

	//プレイヤーのスペック
	const float PL_MOVE_SPEED = -12.0f;			//移動速度
	const float PL_HIT_RANGE = 60.0f;			//プレイヤーの被弾判定距離
	const float FIRECOUNTER_NORMAL = 0.075f;	//通常弾の発射間隔
	const float FIRECOUNTER_SPREAD = 0.25f;		//スプレッドボムの発射間隔
	const int INIT_LIFE = 3;					//初期残機
	const float REVIVE_TIME = 2.0f;								//被弾から復活(モデル表示、行動可能)までの時間。復活エフェクトの再生と無敵時間付与の基準となる。
	const float INVINCEBLE_TIME_EXTEND = REVIVE_TIME + 1.5f;	//復活後、移動可能になってからもしばらく付与される無敵時間。REVIVE_TIME + 固定値
	const float REVIVE_EFFECT_START_TIME = REVIVE_TIME - 1.0f;	//被弾から復活エフェクトが再生開始されるまでの時間。REVIVE_TIME - 固定値

	
	const float CAMERA_ROTATE_FRACTION_ADD_RATE = 0.005f;			//カメラの回転に使う補間係数に加算する定数
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MIN = 0.003f;		//カメラの回転に使う補間係数に加算する定数
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MAX = 0.03f;		//カメラの回転に使う補間係数に加算する定数
	const float CAMERA_MOVESPEED_MAX = 1000.0f;						//カメラ、注視点の追従最高速度 

	//エフェクト関連
	const char16_t* EFFECT_FILEPATH_START = u"Assets/effect/player_born.efk";		//開始エフェクトファイルパス
	const Vector3 EFFECT_SCALE_START = { 50.0f,50.0f,50.0f };						//開始エフェクトの拡大率

	const char16_t* EFFECT_FILEPATH_EXPLOSION = u"Assets/effect/justguard.efk";		//被弾エフェクトファイルパス
	const Vector3 EFFECT_SCALE_EXPLOSION = { 30.0f,30.0f,30.0f };					//被弾エフェクトの拡大率

	const char16_t* EFFECT_FILEPATH_REVIVE = u"Assets/effect/revive_a.efk";			//復活エフェクトのファイルパス
	const Vector3 EFFECT_SCALE_REVIVE = { 20.0f,20.0f,20.0f };						//復活エフェクトの拡大率

	const char16_t* EFFECT_FILEPATH_TRACK = u"Assets/effect/moveTrack.efk";			//軌跡エフェクトのファイルパス
	const Vector3 EFFECT_SCALE_TRACK = { 10.0f,10.0f,10.0f };						//軌跡エフェクトの拡大率

	const char16_t* EFFECT_FILEPATH_DIRECTION = u"Assets/effect/shotDirection.efk";	//方向エフェクトのファイルパス
	const Vector3 EFFECT_SCALE_DIRECTION = { 10.0f,10.0f,10.0f };					//方向エフェクトの拡大率
	const float EFFECT_SCALE_RATIO_DIRECTION = 2.0f;								//スティック入力量に対して掛ける拡大率の固定値

	const char16_t* EFFECT_FILEPATH_MARKER = u"Assets/effect/positionMarker.efk";	//当たり判定エフェクトのファイルパス
	const Vector3 EFFECT_SCALE_MARKER = { 30.0f,30.0f,30.0f };						//当たり判定エフェクトの拡大率
	const float MARKER_PLAY_INTERVAL = 0.02f;										//当たり判定エフェクトの発生間隔

	const char16_t* EFFECT_FILEPATH_CHANGE_NORMAL = u"Assets/effect/change_weapon_normal.efk";	//ショット変更エフェクト(ノーマル)のファイルパス
	const Vector3 EFFECT_SCALE_CHANGE_NORMAL = { 15.0f,15.0f,15.0f };							//ショット変更エフェクト(ノーマル)の拡大率

	const char16_t* EFFECT_FILEPATH_CHANGE_SPREAD = u"Assets/effect/change_weapon_spread.efk";	//ショット変更エフェクト(スプレッド)のファイルパス
	const Vector3 EFFECT_SCALE_CHANGE_SPREAD = { 15.0f,15.0f,15.0f };							//ショット変更エフェクト(スプレッド)の拡大率

	//SEのファイルパス,音量
	//ノーマルショットse
	const wchar_t* SHOT_NORMAL_SE_FILEPATH = L"Assets/wav/normalShotSe_1.wav";
	const float SHOT_NORMAL_SE_VOLUME = 0.4f;
	//スプレッドボムse
	const wchar_t* SHOT_SPREAD_SE_FILEPATH = L"Assets/wav/shot_spread.wav";
	const float SHOT_SPREAD_SE_VOLUME = 0.4f;
	//被弾se
	const wchar_t* MISS_SE_FILEPATH = L"Assets/wav/missSe.wav";
	const float MISS_SE_VOLUME = 1.0f;
	//武器切り替えse
	const wchar_t* CHANGE_SE_FILEPATH = L"Assets/wav/weapon_change.wav";
	const float CHANGE_SE_VOLUME = 0.5f;

}

Player_new::Player_new()
{

}

Player_new::~Player_new()
{
	DeleteGO(m_skinModelRender);
}

void Player_new::Init()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

	//エフェクトを初期化
	InitEffect();

	//ライトを検索,受け取り
	m_directionLight = FindGO<DirectionLight>(DIRECTION_LIGHT_NAME);
	m_pointLight = FindGO<PointLight>(POINT_LIGHT_NAME);
	m_spotLight = FindGO<SpotLight>(SPOT_LIGHT_NAME);
	if (m_directionLight != nullptr) { RecieveDirectionLight(m_directionLight); }
	if (m_pointLight != nullptr) { RecievePointLight(m_pointLight); }
	if (m_pointLight != nullptr) { RecieveSpotLight(m_spotLight); }


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

	//初期化時に開始演出用エフェクトを発生させる
	m_startEffect.SetScale(EFFECT_SCALE_START);
	m_startEffect.SetPosition(m_position);
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
	m_shotDirectionEffect.Init(EFFECT_FILEPATH_DIRECTION);
	m_changeEffectNormal.Init(EFFECT_FILEPATH_CHANGE_NORMAL);
	m_changeEffectSpread.Init(EFFECT_FILEPATH_CHANGE_SPREAD);
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
	//前方、上、右を更新
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);
	
	//上、右ベクトルの書き換え防止用
	Vector3 fixedUp = m_up;
	Vector3 fixedRight = m_right;
	//ショット方向の基準となる前方を更新
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_fixedForward, fixedUp, fixedRight);
	

	// 現在の上ベクトルから、新しい上ベクトルに向けるための回転クォータニオンを計算
	// →　カメラの計算で使う。
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
	//射撃方向の基準は固定の上方向
	m_shotDirection = m_fixedForward * -1.0f;

	//回転軸は上ベクトル
	Vector3 axis = m_up;
	//軸周りの回転クォータニオンを作成
	Quaternion rot;

	//右スティックの入力を受け取り
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//左スティックの入力を受け取り
	float xL = g_pad[0]->GetLStickXF();
	float yL = g_pad[0]->GetLStickYF();

	//右スティックの入力値から角度を求める
	float angle = atan2f(x, y);
	//回転軸周りの回転を求める
	rot.SetRotation(axis, angle);

	//射撃方向表示エフェクトの回転を決めるクォータニオンを作成
	Quaternion effRot = Quaternion::Identity;
	//右スティックの入力方向から角度を取得
	float effAngle = angle;
	//エフェクトの拡大率を0にしておく(右スティックの入力があったときだけ拡大率を上げる)
	float effScaleRate = 0.0f;
	//右スティックの入力量を変換
	float length = sqrt(x*x + y*y);

	//ショットの射出方向の決定
	//移動中
	if (fabsf(xL) > 0.001f || fabsf(yL) > 0.001f) {
		//かつ、右スティックの入力があるとき、右スティックの方へ射撃
		if (fabsf(x) > 0.001f || fabsf(y) > 0.001f) {
			rot.Apply(m_shotDirection);
			//射撃方向表示エフェクトの拡大率を設定
			effScaleRate = EFFECT_SCALE_RATIO_DIRECTION * length;
		}
		//右スティックの入力がないとき、自機の向いている方向へ射撃
		else {
			m_shotDirection = m_forward * -1.0f;
		}
	}
	//停止中
	else {
		//かつ右スティックの入力があるとき、右スティックの方へ射撃
		if (fabsf(x) > 0.001f || fabsf(y) > 0.001f) {
			rot.Apply(m_shotDirection);
			//射撃方向表示エフェクトの拡大率を設定
			effScaleRate = EFFECT_SCALE_RATIO_DIRECTION * length;
		}
		//右スティックの入力がないとき、自機の向いている方向へ射撃
		else {
			m_shotDirection = m_forward * -1.0f;
		}
	}

	//射撃方向エフェクトの向きを決めるため基準となるクォータニオンを作成
	//まず自機のクォータニオンで初期化する
	Quaternion baseRot = m_rot;
	
	//次に乗算するための行列を作成
	//基底軸を決定していく。
	Matrix baseMat;
	//exは右方向
	baseMat.m[0][0] = m_right.x;
	baseMat.m[0][1] = m_right.y;
	baseMat.m[0][2] = m_right.z;
	//eyは上方向
	baseMat.m[1][0] = m_up.x;
	baseMat.m[1][1] = m_up.y;
	baseMat.m[1][2] = m_up.z;
	//射撃方向の基準に用いた固定の上方向を指定することで、
	//エフェクトの向きが自機の向きに依存しなくなる。
	baseMat.m[2][0] = m_fixedForward.x;
	baseMat.m[2][1] = m_fixedForward.y;
	baseMat.m[2][2] = m_fixedForward.z;
	//行列を乗算
	baseRot.SetRotation(baseMat);

	//右スティックの入力で方向を決定する
	//上方向周りに回転させる
	effRot.SetRotation(axis,angle);
	//その結果を基準となるクォータニオンと乗算する
	effRot.Multiply(baseRot,effRot);

	//射撃方向エフェクトに値を設定
	m_shotDirectionEffect.SetPosition(m_position);
	m_shotDirectionEffect.SetRotation(effRot);
	m_shotDirectionEffect.SetScale(EFFECT_SCALE_DIRECTION * effScaleRate);

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
		//通常ショット
		if (m_enUseWeapon == enNormalShot) {
			if (m_fireCounter > FIRECOUNTER_NORMAL || m_fireCounter == 0.0f) {

				//ショットSEの再生
				if (m_enBulletType == enNormalShot) {
					m_ssNormalShotSe = NewGO<CSoundSource>(0);
					m_ssNormalShotSe->Init(SHOT_NORMAL_SE_FILEPATH);
					m_ssNormalShotSe->SetVolume(SHOT_NORMAL_SE_VOLUME);
					m_ssNormalShotSe->Play(false);
				}


				//弾管理クラスの関数を使用して出現させる
				m_bulletManager->InitBullets(
					m_position,
					m_up,
					m_shotDirection,
					m_enBulletType
				);

				//発射後、カウンターを0にリセット
				m_fireCounter = 0.0f;

			}
		}
		//スプレッドボム
		if (m_enBulletType == enSpreadBomb) {
			if (m_fireCounter > FIRECOUNTER_SPREAD || m_fireCounter == 0.0f) {
				//ショットSEの再生
				if (m_enBulletType == enSpreadBomb) {
					CSoundSource* ssSpreadSe = NewGO<CSoundSource>(0);
					ssSpreadSe->Init(SHOT_SPREAD_SE_FILEPATH);
					ssSpreadSe->SetVolume(SHOT_SPREAD_SE_VOLUME);
					ssSpreadSe->Play(false);
				}

				//弾管理クラスの関数を使用して出現させる
				m_bulletManager->InitBullets(
					m_position,
					m_up,
					m_shotDirection,
					m_enBulletType
				);

				//発射後、カウンターを0にリセット
				m_fireCounter = 0.0f;
			}
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
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonLB1)) {
		//武器切り替え時のSEを再生
		CSoundSource* ssChangeSe = NewGO<CSoundSource>(0);
		ssChangeSe->Init(CHANGE_SE_FILEPATH);
		ssChangeSe->SetVolume(CHANGE_SE_VOLUME);
		ssChangeSe->Play(false);

		switch (m_enUseWeapon)
		{
		case enNormalShot:
			m_enUseWeapon = enSpreadBomb;
			m_enBulletType = enPlayerSpreadBomb;
			
			//スプレッドボムに変更するエフェクトを再生
			m_changeEffectSpread.SetPosition(m_position);
			m_changeEffectSpread.SetRotation(m_rot);
			m_changeEffectSpread.SetScale(EFFECT_SCALE_CHANGE_SPREAD);
			m_changeEffectSpread.Play(false);
			break;

		case enSpreadBomb:
			m_enUseWeapon = enNormalShot;
			m_enBulletType = enPlayerNormal;
			
			//ノーマルショットに変更するエフェクトを再生
			m_changeEffectNormal.SetPosition(m_position);
			m_changeEffectNormal.SetRotation(m_rot);
			m_changeEffectNormal.SetScale(EFFECT_SCALE_CHANGE_NORMAL);
			m_changeEffectNormal.Play(false);
			break;

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
	QueryGOs<Enemy>(ENEMY_NAME, [&](Enemy* enemy) {
		//距離を計算
		Vector3 diff = enemy->GetPosition() - m_position;
		float length = diff.Length();

		//エネミーに接触したとき
		if (length < PL_HIT_RANGE) {
			
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
				SetInvincibleTime(INVINCEBLE_TIME_EXTEND);
				SetIsInvFlag(true);

				//ミス時のseを再生
				CSoundSource* ssMissSe = NewGO<CSoundSource>(0);
				ssMissSe->Init(MISS_SE_FILEPATH);
				ssMissSe->SetVolume(MISS_SE_VOLUME);
				ssMissSe->Play(false);

				//被弾エフェクトを発生
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

	//敵弾との判定
	QueryGOs<Bullet>(BULLET_ENEMY_NAME, [&](Bullet* enemyBullet) {
		//距離を計算
		Vector3 diff = enemyBullet->GetPosition() - m_position;
		float length = diff.Length();

		//エネミーに接触したとき
		if (length < PL_HIT_RANGE) {

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
				SetInvincibleTime(INVINCEBLE_TIME_EXTEND);
				SetIsInvFlag(true);

				//ミス時のseを再生
				CSoundSource* ssMissSe = NewGO<CSoundSource>(0);
				ssMissSe->Init(MISS_SE_FILEPATH);
				ssMissSe->SetVolume(MISS_SE_VOLUME);
				ssMissSe->Play(false);

				//被弾エフェクトを発生
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
	QueryGOs<Explosion>(EXPLOSION_ENEMY_NAME, [&](Explosion* enemyExplosion) {
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
				SetInvincibleTime(INVINCEBLE_TIME_EXTEND);
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
	
	//復活カウンタが復活時間に達していたら
	if (m_reviveCounter >= REVIVE_TIME) {
		//存在フラグをオン
		m_isExist = true;
		//カウンターをリセット
		m_reviveCounter = 0.0f;
	}

}

void Player_new::ReviveReady()
{
	//ゲームディレクター側のライフが0なら処理しない
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		return;
	}

	//復活カウンタがエフェクト再生開始時間に達していて、存在フラグがオフなら
	if (m_reviveCounter >= REVIVE_EFFECT_START_TIME
		&& m_isExist == false) {
		//復活準備フラグをオン
		m_isReviveReady = true;
	}

	//現フレームで復活準備フラグがオンなら
	if (m_isReviveReadyPrev == false
		&& m_isReviveReady == true) {

		//復活エフェクトを再生
		m_reviveEffect.SetPosition(m_position);
		m_reviveEffect.SetRotation(m_rot);
		m_reviveEffect.SetScale(EFFECT_SCALE_REVIVE);
		m_reviveEffect.Play();
	}
}

void Player_new::Revive()
{
	//ゲームディレクター側のライフが0なら復活しない
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		return;
	}

	//現フレームで生存フラグが立ったら
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
	//1を足し2で割ることで-1.0f〜1.0fを0.0f〜1.0fに変換する
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

void Player_new::EffectUpdate()
{
	//開始時のエフェクトの位置と更新
	m_startEffect.SetPosition(m_position + m_up * 50.0f);
	m_startEffect.Update();
	
	//武器切り替え時のエフェクト(通常)を更新
	m_changeEffectNormal.SetPosition(m_position);
	m_changeEffectNormal.SetScale(EFFECT_SCALE_CHANGE_NORMAL);
	m_changeEffectNormal.Update();
	//武器切り替え時のエフェクト(スプレッド)を更新
	m_changeEffectSpread.SetPosition(m_position);
	m_changeEffectSpread.SetScale(EFFECT_SCALE_CHANGE_SPREAD);
	m_changeEffectSpread.Update();

	//射撃方向表示エフェクトの更新
	//再生停止していたら再度再生する
	if (m_shotDirectionEffect.IsPlay() != true) {
		m_shotDirectionEffect.Play();
	}
	m_shotDirectionEffect.Update();

	//その他諸々更新
	m_explosionEffect.Update();
	m_reviveEffect.Update();
	m_moveTrackEffect.Update();
	m_markerEffect.Update();
}

void Player_new::Update()
{
	//ゲーム中、スタート時以外なら処理しない
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
	////////////////////////////////////////////////////////////
	//注視点目標からカメラ目標へのベクトルを作成
	Vector3 toCamera = m_gameCamera.GetCameraPositionTarget() - m_gameCamera.GetTargetPositionTarget();
	//ベクトルにクォータニオンを適用
	m_rotUpToGroundNormal.Apply(toCamera);
	
	//注視点目標を自身に設定
	m_gameCamera.SetTargetPositionTarget(m_position);
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
	////////////////////////////////////////////////////////////

	//各種フラグの記録
	////////////////////////////////////////////////////////////
	//現フレームの上を記録
	m_upPrev = m_up;
	//現フレームの無敵フラグを記録
	m_isInvinciblePrev = m_isInvincible;
	//現フレームの存在フラグを記録
	m_isExistPrev = m_isExist;
	m_isReviveReadyPrev = m_isReviveReady;
	////////////////////////////////////////////////////////////

	//エフェクトの更新
	EffectUpdate();
}