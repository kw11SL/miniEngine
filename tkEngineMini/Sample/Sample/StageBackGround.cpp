#include "stdafx.h"
#include "StageBackGround.h"

namespace
{
	const char* MODEL_SHADER_PATH = "Assets/shader/model.fx";
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	const char* MODEL_FILEPATH = "Assets/modelData/backGround/backGround.tkm";
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

void StageBackGround::Init(RenderingEngine& renderingEngine, const Vector3& pos, const Quaternion& rot, const Vector3& scale)
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//�w�i�ɂ͉e�𗎂Ƃ������̂ŃV���h�E���V�[�o�[�t���O���I���ɂ���
	m_skinModelRender->Init(MODEL_FILEPATH, enModelUpAxisZ, renderingEngine, false, false);

	//���f���g��
	//m_scale *= MODEL_INIT_SCALE_RATIO;

	m_skinModelRender->SetPosition(pos);
	m_skinModelRender->SetRotation(rot);
	m_skinModelRender->SetScale(m_scale);

	//���[���h�s��̍X�V
	m_skinModelRender->UpdateWorldMatrix();

}

void StageBackGround::RecieveDirectionLight(DirectionLight* dirLight)
{
	m_skinModelRender->InitDirectionLight(dirLight);
}

void StageBackGround::RecievePointLight(PointLight* ptLight)
{
	m_skinModelRender->InitPointLight(ptLight);
}

void StageBackGround::RecieveSpotLight(SpotLight* spLight)
{
	m_skinModelRender->InitSpotLight(spLight);
}

void StageBackGround::InitModelFromInitData()
{
	m_skinModelRender->InitModel();
}

void StageBackGround::Update()
{

}