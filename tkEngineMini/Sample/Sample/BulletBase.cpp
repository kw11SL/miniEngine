#include "stdafx.h"
#include "BulletBase.h"

BulletBase::~BulletBase()
{
	//エフェクトを停止
	m_shotEffect.Stop();
	DeleteGO(m_skinModelRender);
}

void BulletBase::Init(
	RenderingEngine& renderingEngine,
	const Vector3& initPoint,
	const Vector3& initUp,
	const Vector3& direction
)
{
	//継承先で定義した処理
	InitSub();

	//初期位置を決定
	m_position = initPoint;
	//上方向を決定
	m_up = initUp;
	//前方を決定
	m_direction = direction;
	m_direction.Normalize();

	//決定した上方向の反対が下方向になる
	m_downVector = m_up * -1.0f;
	m_downVector.Normalize();

	//キャラコンの初期化
	m_myCharaCon.Init(m_position);

	//球面移動用クラスの初期化
	m_sphericalMove.Init(m_forward, m_right, m_up);


}

void BulletBase::InitSub()
{

}

void BulletBase::UpdateSub()
{

}

void BulletBase::Update()
{

}



