#include "stdafx.h"
#include "MyCharacterController.h"

namespace {

	//衝突時に呼ばれる関数オブジェクト(地面用)
	struct SweepResultGround : public btCollisionWorld::ConvexResultCallback 
	{
		bool isHit = false;									//衝突フラグ
		Vector3 hitPos = Vector3(0.0f, 0.0f, 0.0f);		//レイ衝突点の座標
		Vector3 hitNormal;								//レイ衝突点の法線

		//btCollisionObject* me = nullptr;					//自分自身を除外するためのメンバ

		virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) 
		{
			////自分自身を衝突から除外する処理
			//if (convexResult.m_hitCollisionObject == me) {
			//	return 0.0f;
			//}


			//衝突点を引っ張ってくる処理
			
			//ヒットフラグをオン
			isHit = true;

			//レイの衝突点の座標を取得
			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
			//取得した衝突点を格納
			hitPos = hitPosTmp;

			////レイの衝突点の法線を取得
			//Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
			////取得した法線を格納
			//hitNormal = hitNormalTmp;

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
	//半径と高さを渡し、カプセルコライダーを初期化
	m_cupsuleCollider.Init(m_radius, m_height);
	////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////
	//剛体の初期化データを作成
	RigidBodyInitData rbInitData;
	//カプセルコライダーを剛体に登録
	rbInitData.collider = &m_cupsuleCollider;
	//質量を設定
	rbInitData.mass = 0.0f;
	
	//剛体を初期化情報で初期化
	m_rigidBody.Init(rbInitData);
	////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////
	//剛体の各種設定
	//剛体のワールド座標を取得
	btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
	//剛体の基点を更新
	//カプセルコライダーの高さの半分を基点にしている
	trans.setOrigin(btVector3(pos.x, pos.y + m_height * 0.5, pos.z));
	//剛体にコリジョン属性を付与する(=処理を分ける際の識別に使用)
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
	m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	////////////////////////////////////////////////////////////////////////////////////////


	//初期化終了
	m_isInit = true;
}

void MyCharacterController::RemoveRigidBody()
{
	PhysicsWorld::GetInstance()->RemoveRigidBody(m_rigidBody);
}

const Vector3& MyCharacterController::Execute(Vector3& moveSpeed,Vector3& downVector)
{
	//座標を保存
	Vector3 posTmp = m_position;

	// 移動速度を使って、キャラコンの座標を動かす
	// 1フレームの移動量を計算する
	Vector3 addPos = moveSpeed;
	//m_position += addPos;
	
	// step-1 下方向にレイを飛ばして、地面との当たり判定を行う。
	// ヒント　当たり判定はCharaConの179行目
	
	//レイの作成
	//始点と終点を作成
	btTransform start;
	btTransform end;

	start.setIdentity();
	end.setIdentity();

	//レイの始点をコライダの底辺にする
	start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));

	////レイの始点から終点へのベクトルを作る
	//btVector3 endTmp = start.getOrigin() + (btVector3(downVector.x, downVector.y, downVector.z));
	
	////レイの終点をコライダから下方向ベクトルの終点にする
	//end.setOrigin(endTmp);

	end.setOrigin(btVector3(downVector.x, downVector.y, downVector.z));
	//end.setOrigin(btVector3(posTmp.x, posTmp.y - 10.0f, posTmp.z));


	//関数オブジェクトを定義
	SweepResultGround callBack;
	//callBack.me = m_rigidBody.GetBody();

	// step-2 ぶつかっていたら、m_positionに交点の座標を代入する
	// ヒント　ぶつかっているときに呼ばれるコールバック関数はキャラコンの27行目。

	//レイの衝突を検出し、関数オブジェクトに結果を格納する
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_cupsuleCollider.GetBody(),
		start,
		end,
		callBack
	);

	//衝突していたら
	if (callBack.isHit) {
		//座標を衝突点にする
		m_position = callBack.hitPos;
	}

	//座標を移動速度で更新
	m_position += addPos;


	//処理後の座標を返す
	return m_position;
}