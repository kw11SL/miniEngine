#include "stdafx.h"
#include "BG.h"

namespace
{
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	//const char* MODEL_FILEPATH = "Assets/modelData/bg/stage_cupsule3.tkm";
	//const char* MODEL_FILEPATH = "Assets/modelData/bg/stage_cupsule4.tkm";
	const char* MODEL_FILEPATH = "Assets/modelData/bg/stage_cupsule5.tkm";

	const Vector3 INIT_POINT = { 0.0f,0.0f,0.0f };

	const float MODEL_INIT_SCALE_RATIO = 5.0f;
}

BG::BG()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
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