#include "stdafx.h"
#include "SkyCube.h"

namespace {
	const char* MODEL_FILEPATH = "Assets/modelData/skyCube/sky.tkm";
	const char* SHADER_FILEPATH = "Assets/shader/skyCubeMap.fx";
	const wchar_t* TEXTURE_FILEPATH = L"Assets/modelData/skyCube/skyCubeMap.dds";
}

SkyCube::~SkyCube()
{
	DeleteGO(m_skinModelRender);
}

bool SkyCube::Start()
{
	return true;
}

void SkyCube::Init(const char* filePath)
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

	//追加の初期化情報を作成
	ModelInitData initData;
	initData.m_tkmFilePath = filePath;
	initData.m_fxFilePath = SHADER_FILEPATH;
	initData.m_modelUpAxis = enModelUpAxisY;
	initData.m_vsEntryPointFunc = "VSMain";
	initData.m_psEntryPointFunc = "PSMain";

	//テクスチャを初期化
	m_texture.InitFromDDSFile(TEXTURE_FILEPATH,true);
	//SRVにテクスチャのアドレスを渡す
	initData.m_expandShaderResoruceView[0] = &m_texture;
	//拡張バッファに明るさの情報を渡す
	initData.m_expandConstantBuffer = &m_luminance;
	initData.m_expandConstantBufferSize = sizeof(m_luminance);

	//追加の初期化情報を登録
	m_skinModelRender->SetModelInitDataAdditional(initData);

	//モデルを初期化(レンダリングエンジンに登録するため)
	m_skinModelRender->Init(
		filePath,
		enModelUpAxisY,
		*RenderingEngine::GetInstance(),
		false,
		false,
		nullptr
	);

	
	////初期化
	//m_skinModelRender->InitModel();

	m_skinModelRender->UpdateWorldMatrix(m_position, m_rot, m_scale);

	m_skinModelRender->Update();
}

void SkyCube::Update()
{
	if (m_isDirty)
	{
		m_skinModelRender->UpdateWorldMatrix(m_position, m_rot, m_scale);
		m_skinModelRender->Update();
		m_isDirty = false;
	}
}