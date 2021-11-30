#include "stdafx.h"
#include "Player_new.h"
#include "Enemy.h"


namespace{
	const char* MODELPATH_UTC = "Assets/modelData/unityChan.tkm";
	const char* SKELETON_PATH_UTC = "Assets/modelData/unityChan.tks";
	const float UTC_RADIUS = 40.0f;
	const float UTC_HEIHGT = 100.0f;
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
	
	const Vector3 INIT_POINT = {0.0f,700.0f,0.0f};
	const float UPPER_OFFSET = 0.0f;

	const float PL_MOVE_SPEED = -12.0f;			//移動速度
	const float FIRECOUNTER_NORMAL = 0.15f;		//通常弾の発射間隔
	const int INIT_LIFE = 3;					//初期残機
	const float INVINCIBLE_TIME_REVIVE = 7.0f;	//復活時に設定される無敵時間
	const float REVIVE_TIME = 3.0f;				//被弾から復活までの時間

	
	const float CAMERA_ROTATE_FRACTION_ADD_RATE = 0.005f;			//カメラの回転に使う補間係数に加算する定数
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MIN = 0.003f;		//カメラの回転に使う補間係数に加算する定数
	const float CAMERA_ROTATE_FRACTION_ADD_RATE_MAX = 0.03f;		//カメラの回転に使う補間係数に加算する定数
	const float CAMERA_MOVESPEED_MAX = 1000.0f;						//カメラ、注視点の追従最高速度 
}

Player_new::~Player_new()
{
	DeleteGO(m_skinModelRender);
}

void Player_new::Init(RenderingEngine& renderingEngine)
{
	//ライトを検索
	m_directionLight = FindGO<DirectionLight>("directionlight");
	m_pointLight = FindGO<PointLight>("pointlight");
	m_spotLight = FindGO<SpotLight>("spotlight");

	m_bulletManager = BulletManager::GetInstance();

	m_skinModelRender = NewGO<SkinModelRender>(0);
	
	m_skinModelRender->Init(
		MODELPATH_UTC, 
		enModelUpAxisZ,
		renderingEngine,
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

	//カメラの初期化
	m_gameCamera.Init(CAMERA_MOVESPEED_MAX);
	//注視点を設定
	m_gameCamera.SetTargetPosition(m_position);
	//視点を設定
	m_gameCamera.SetCameraPosition(m_position + toCamera);
	//注視点目標を設定
	m_gameCamera.SetTargetPositionTarget(m_position);
	//カメラ目標を設定
	m_gameCamera.SetCameraPositionTarget(m_position + toCamera);

	//カメラの上方向を自身の上にしておく
	m_cameraUp = m_up;

	//発射方向を前方にしておく
	//m_shotDirection = m_forward;
}

bool Player_new::Start()
{

	return true;
}

void Player_new::Move()
{
	//存在フラグがオフならreturn
	if (m_isExist == false) {
		return;
	}

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
	//m_moveSpeed = m_right * x * PL_MOVE_SPEED;
	//プレイヤーの前後(奥、手前)方向への移動
	m_moveSpeed += forward * y * PL_MOVE_SPEED;

	//自作キャラコンに移動速度を渡す
	m_position = m_myCharaCon.Execute(m_moveSpeed,m_downVector,UPPER_OFFSET);


	// 上ベクトルを更新
	//下向きベクトル(=レイを飛ばす方向)* -1.0　= プレイヤーの上ベクトル
	Vector3 newUp = m_downVector * -1.0f;
	// 現在の上ベクトルから、新しい上ベクトルに向けるための回転クォータニオンを計算
	//		→　カメラの計算で使う。
	m_rotUpToGroundNormal.SetRotation(m_up, newUp);

	//自身の上ベクトルを更新
	m_up = newUp;

	//更新した上ベクトルと前方ベクトルの外積　=　右ベクトル
	//m_right = g_camera3D->GetRight();
	m_right.Cross(m_up,m_forward);
	//求めた右ベクトルと更新した上ベクトルの外積　=　前方ベクトル
	m_forward.Cross(m_right, m_up);
	
	
	//モデルの座標更新
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
	//存在フラグがオフならreturn
	if (m_isExist == false) {
		return;
	}

	//R1ボタンを押すと発射、押しっぱなしで連射
	if (g_pad[0]->IsPress(enButtonRB1)) {
		
		//カウンターが0のときとカウンターが一定値を超えると発射
		if (m_fireCounter > FIRECOUNTER_NORMAL || m_fireCounter == 0.0f) {
			
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
	//被弾判定
	//エネミーを検索
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		//距離を計算
		Vector3 diff = enemy->GetPosition() - m_position;
		float length = diff.Length();

		if (length < 60.0f) {
			//自身が無敵状態でなければ
			if (m_isInvincible == false) {
				//1機減らす
				m_life -= 1;
				
				//モデルを消す
				DeleteGO(m_skinModelRender);

				//生存フラグをオフ
				SetIsExist(false);

				//無敵状態にする
				SetInvincibleTime(INVINCIBLE_TIME_REVIVE);
				SetIsInvFlag(true);

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

	if (m_isExist == true
		&& m_isExistPrev == false) {

		m_skinModelRender = NewGO<SkinModelRender>(0);
		
		m_skinModelRender->Init(
					MODELPATH_UTC,
					enModelUpAxisZ,
					*RenderingEngine::GetInstance(),
					true,
					false,
					SKELETON_PATH_UTC
				);

		m_skinModelRender->SetPosition(m_position);
		m_skinModelRender->SetScale(m_scale);

		//ライトの受け取り処理
		RecieveDirectionLight(m_directionLight);
		RecievePointLight(m_pointLight);
		RecieveSpotLight(m_spotLight);

		InitModelFromInitData();

	}
}

void Player_new::Update()
{

	float addRate = 0.0f;
	float maxAddRate = CAMERA_ROTATE_FRACTION_ADD_RATE_MAX;
	float minAddRate = CAMERA_ROTATE_FRACTION_ADD_RATE_MIN;

	Vector3 addRateVec = { 0.0f,0.0f,0.0f, };
	Vector3 maxAddRateVec = { maxAddRate,0.0f,0.0f };
	Vector3 minAddRateVec = { minAddRate,0.0f,0.0f };

	float dotVec = Dot(m_cameraUp,m_up);
	dotVec += 1.0f;
	dotVec /= 2.0f;

	addRateVec.Lerp(dotVec, maxAddRateVec, minAddRateVec);
	addRate = addRateVec.Length();

	//カメラの上を補完する係数を加算
	//m_cameraUpFraction += CAMERA_ROTATE_FRACTION_ADD_RATE;
	m_cameraUpFraction += addRate;
	//1を超えたら1に補正
	if (m_cameraUpFraction > 1.0f) {
		m_cameraUpFraction = 1.0f;
	}

	Move();
	Rotation();
	RotateShotDirection();
	ChangeWeapon();
	FireBullet();
	Hit();
	AddReviveCouter();
	Revive();
	DecInvTime();

	
	if (m_skinModelRender != nullptr) {
		m_skinModelRender->SetRotation(m_rot);
	}

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

	////注視点を自身に設定
	//m_gameCamera.SetTargetPosition(m_position);
	//注視点目標を自身に設定
	m_gameCamera.SetTargetPositionTarget(m_position);

	////視点を設定
	//m_gameCamera.SetCameraPosition(m_position + toCamera);
	//カメラ目標を設定
	m_gameCamera.SetCameraPositionTarget(m_position + toCamera);
	
	////カメラ目標を追いかける処理
	//m_gameCamera.ChaseCameraPosition();
	////注視点目標を追いかける処理
	//m_gameCamera.ChaseTargetPosition();

	// カメラの上方向目標をプレイヤーの上方向に設定。
	m_gameCamera.SetUpVectorTarget(m_up);

	//カメラの上を少しずつ補間していく
	m_gameCamera.LerpUpVector(m_cameraUpFraction, m_cameraUp);
	//補間したカメラの上でカメラの上を更新
	m_gameCamera.SetUp(m_cameraUp);
	//カメラの更新
	m_gameCamera.UpdateCamera();
	

	//自機のリスポーン処理
	//if (m_isInvincible == false 
	//	&& m_isInvinciblePrev == true) {
	//	m_skinModelRender = NewGO<SkinModelRender>(0);

	//	m_skinModelRender->Init(
	//		MODELPATH_UTC,
	//		enModelUpAxisZ,
	//		*RenderingEngine::GetInstance(),
	//		true,
	//		false,
	//		SKELETON_PATH_UTC
	//	);

	//	m_skinModelRender->SetPosition(m_position);
	//	m_skinModelRender->SetScale(m_scale);

	//	//ライトの受け取り処理
	//	RecieveDirectionLight(m_directionLight);
	//	RecievePointLight(m_pointLight);
	//	RecieveSpotLight(m_spotLight);

	//	InitModelFromInitData();

	//}


	//////////////////////////////////
	//現フレームの上を記録
	m_upPrev = m_up;
	//現フレームの無敵フラグを記録
	m_isInvinciblePrev = m_isInvincible;
	//現フレームの存在フラグを記録
	m_isExistPrev = m_isExist;
	//////////////////////////////////
	
	
	//テスト　モデルの削除
	if (g_pad[0]->IsTrigger(enButtonY)) {
		DeleteGO(m_skinModelRender);
	}

	

}