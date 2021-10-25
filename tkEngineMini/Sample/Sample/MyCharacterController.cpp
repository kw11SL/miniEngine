#include "stdafx.h"
#include "MyCharacterController.h"

namespace {

	//レイの始点の位置(キャラの下方向を正として逆方向に設定するので負の値)
	const float RAY_START_OFFSET = -10.0f;
	//レイの終点の位置(キャラの下方向を正として順方向に設定するので正の値)
	const float RAY_END_OFFSET = 1000.0f;


	//衝突時に呼ばれる関数オブジェクト(地面用)
	struct SweepResultGround : public btCollisionWorld::RayResultCallback
	{
		bool isHit = false;								//衝突フラグ
		Vector3 hitPos = Vector3(0.0f, 0.0f, 0.0f);		//レイ衝突点の座標
		Vector3 hitNormal;								//レイ衝突点の法線
		float distToHitPos = FLT_MAX;					//交点までの距離。
		Vector3 startPos;								//レイの始点
		Vector3 endPos;									//レイの終点

		/// @brief レイとコライダーが衝突した時に呼ばれるコールバック
		/// @param rayResult 
		/// @param normalInWorldSpace 
		/// @return 
		btScalar addSingleResult(
			btCollisionWorld::LocalRayResult& rayResult, 
			bool normalInWorldSpace ) override
		{

			//衝突点を引っ張ってくる処理		
			//ヒットフラグをオン
			isHit = true;
			//レイの衝突点の座標を取得
			Vector3 hitPosTmp;
			//m_hitFraction：衝突点の補間率。始点と終点の間のどこに位置するかの情報
			//つまり線形補完することで衝突点の座標が求められる
			hitPosTmp.Lerp(rayResult.m_hitFraction, startPos, endPos );
			
			hitPos = hitPosTmp;

			if (rayResult.m_hitFraction < distToHitPos) {
				// こちらの方が近いので、衝突点を更新
				hitPos = hitPosTmp;
				//衝突点の法線を持ってくる
				hitNormal.x = rayResult.m_hitNormalLocal.x();
				hitNormal.y = rayResult.m_hitNormalLocal.y();
				hitNormal.z = rayResult.m_hitNormalLocal.z();

				distToHitPos = rayResult.m_hitFraction;
			}
			
			return 0.0f;
		}
	};
}


void MyCharacterController::Init(float radius, float height, Vector3& pos)
{
	//座標をm_positionに設定する
	//座標はモデルの原点
	SetPosition(pos);

	////////////////////////////////////////////////////////////////////////////////////////
	//カプセルコライダーの初期化
	m_radius = radius;
	m_height = height;

	//初期化終了
	m_isInit = true;
}


const Vector3& MyCharacterController::Execute(Vector3& moveSpeed,Vector3& downVector)
{
	// 次の移動先となる座標を計算する。
	Vector3 nextPos = m_position + moveSpeed;
	
	//レイの作成
	//始点と終点を作成
	Vector3 start;
	Vector3 end;

	//レイの始点は移動先の座標
	start = nextPos;
	//下方向と逆向きにやや上を指定(衝突点を確定させるため)
	start += downVector * RAY_START_OFFSET;
	
	//終点に始点の座標を代入
	end = start;
	// 終点は下方向に300
	end += downVector * RAY_END_OFFSET;


	//関数オブジェクトを定義
	SweepResultGround callBack;
	callBack.startPos = start;
	callBack.endPos = end;

	// 物理ワールドに登録されているコライダーとレイの交差テスト
	PhysicsWorld::GetInstance()->RayTest(
		start,
		end,
		callBack
	);

	//衝突していたら
	if (callBack.isHit) {
		
		//座標を衝突点にする
		nextPos = callBack.hitPos;
		
		//下方向ベクトルの更新
		//下方向を引っ張ってきた法線と逆方向にする
		downVector = callBack.hitNormal * -1.0f;
	}

	//移動先の座標が確定したのでm_positionに代入
	m_position = nextPos;

	//処理後の座標を返す
	return m_position;
}