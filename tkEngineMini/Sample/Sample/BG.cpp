#include "stdafx.h"
#include "BG.h"

namespace
{
	const char* MODEL_FILEPATH = "Assets/modelData/bg/stage_cupsule3.tkm";	//モデルのファイルパス

	const Vector3 INIT_POINT = { 0.0f,0.0f,0.0f };							//初期位置

	const float MODEL_INIT_SCALE_RATIO = 5.0f;								//拡大率
}

BG::BG()
{
}

BG::~BG()
{
	DeleteGO(m_skinModelRender);
}

bool BG::Start()
{
	return true;
}

void BG::Init(const Vector3& pos, const Quaternion& rot,const Vector3& scale)
{

	m_skinModelRender = NewGO<SkinModelRender>(0);

	//ライトを検索、受け取り
	m_directionLight = FindGO<DirectionLight>(DIRECTION_LIGHT_NAME);
	m_pointLight = FindGO<PointLight>(POINT_LIGHT_NAME);
	m_spotLight = FindGO<SpotLight>(SPOT_LIGHT_NAME);
	if (m_directionLight != nullptr) { RecieveDirectionLight(m_directionLight); }
	if (m_pointLight != nullptr) { RecievePointLight(m_pointLight); }
	if (m_spotLight != nullptr) { RecieveSpotLight(m_spotLight); }

	//背景には影を落としたいのでシャドウレシーバーフラグをオンにする
	m_skinModelRender->Init(MODEL_FILEPATH, enModelUpAxisY, false, true);

	//モデル拡大
	//m_scale *= MODEL_INIT_SCALE_RATIO;

	m_position = pos;
	m_rotation = rot;
	m_scale = scale;
	
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);

	//ワールド行列の更新
	m_skinModelRender->UpdateWorldMatrix();

	//静的オブジェクトの作成
	m_physicsStaticObject.CreateFromModel(
		m_skinModelRender->GetModel(),
		m_skinModelRender->GetMatrix()
	);

}

void BG::RecieveDirectionLight(DirectionLight* dirLight)
{
	m_skinModelRender->InitDirectionLight(dirLight);
}

void BG::RecievePointLight(PointLight* ptLight)
{
	m_skinModelRender->InitPointLight(ptLight);
}

void BG::RecieveSpotLight(SpotLight* spLight)
{
	m_skinModelRender->InitSpotLight(spLight);
}

void BG::InitModelFromInitData()
{
	m_skinModelRender->InitModel();
}

void BG::Update()
{


}