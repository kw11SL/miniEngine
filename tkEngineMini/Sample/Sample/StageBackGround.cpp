#include "stdafx.h"
#include "StageBackGround.h"

namespace
{
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* MODEL_FILEPATH = "Assets/modelData/backGround/backGround_1.tkm";

	const Vector3 INIT_POINT = { 0.0f,0.0f,0.0f };

	const float MODEL_INIT_SCALE_RATIO = 5.0f;
}

StageBackGround::StageBackGround()
{
}

StageBackGround::~StageBackGround()
{
	DeleteGO(m_skinModelRender);
}

bool StageBackGround::Start()
{
	return true;
}

void StageBackGround::Init(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

	//ライトを検索、受け取り
	m_directionLight = FindGO<DirectionLight>(DIRECTION_LIGHT_NAME);
	m_pointLight = FindGO<PointLight>(POINT_LIGHT_NAME);
	m_spotLight = FindGO<SpotLight>(SPOT_LIGHT_NAME);
	if (m_directionLight != nullptr) { RecieveDirectionLight(m_directionLight); }
	if (m_pointLight != nullptr) { RecievePointLight(m_pointLight); }
	if (m_spotLight != nullptr) { RecieveSpotLight(m_spotLight); }

	m_skinModelRender->Init(MODEL_FILEPATH, enModelUpAxisZ, false, false);

	//モデル拡大
	//m_scale *= MODEL_INIT_SCALE_RATIO;

	m_skinModelRender->SetPosition(pos);
	m_skinModelRender->SetRotation(rot);
	m_skinModelRender->SetScale(m_scale);

	//ワールド行列の更新
	m_skinModelRender->UpdateWorldMatrix();

}



void StageBackGround::Update()
{

}