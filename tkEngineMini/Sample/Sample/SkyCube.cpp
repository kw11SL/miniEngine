#include "stdafx.h"
#include "SkyCube.h"

namespace {
	//ファイルパス関連
	const char* MODEL_FILEPATH = "Assets/modelData/skyCube/sky.tkm";							//モデルのファイルパス
	const char* SHADER_FILEPATH = "Assets/shader/skyCubeMap.fx";								//シェーダーのファイルパス
	const wchar_t* TEXTURE_FILEPATH = L"Assets/modelData/skyCube/skyCubeMapDay_Toon_04.dds";	//テクスチャのファイルパス

	//明るさ関連
	const float MIN_LUMINANCE = 0.0f;		//明るさの最低値
	const float MAX_LUMINANCE = 1.2f;		//明るさの最大値
	const float LUMINACE_ADDRATE = 0.01f;	//フェードインの割合
}

SkyCube::SkyCube()
{

}

SkyCube::~SkyCube()
{
	DeleteGO(m_skinModelRender);
}

bool SkyCube::Start()
{
	return true;
}

void SkyCube::Init()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

	//追加の初期化情報を作成
	ModelInitData initData;
	initData.m_tkmFilePath = MODEL_FILEPATH;
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
		MODEL_FILEPATH,
		enModelUpAxisY,
		false,
		false,
		nullptr
	);

	//座標、回転、拡大率を設定
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rot);
	m_skinModelRender->SetScale(m_scale);
}

void SkyCube::FadeIn(const float addRate)
{
	//明るさの最大値に達していたら処理しない
	if (m_luminance >= MAX_LUMINANCE) {
		return;
	}

	//明るさの最大値に達するまで足していく
	if (m_luminance <= MAX_LUMINANCE){
		m_luminance += addRate;
	}
}

void SkyCube::Update()
{
	//ゲーム中でなければ処理しない
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//ステージ開始時のフェードイン処理
	FadeIn(LUMINACE_ADDRATE);

	/*if (m_isDirty)
	{
		m_skinModelRender->UpdateWorldMatrix(m_position, m_rot, m_scale);
		m_skinModelRender->Update();
		m_isDirty = false;
	}*/
}