#include "stdafx.h"
#include "StageBackGround.h"

namespace
{
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	//const char* MODEL_FILEPATH = "Assets/modelData/backGround/backGround.tkm";
	const char* MODEL_FILEPATH = "Assets/modelData/backGround/backGround_1.tkm";
	//const char* MODEL_FILEPATH = "Assets/modelData/backGround/backGround_2.tkm";

	const Vector3 INIT_POINT = { 0.0f,0.0f,0.0f };

	const float MODEL_INIT_SCALE_RATIO = 5.0f;
}

StageBackGround::StageBackGround()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
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