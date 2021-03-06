#include "stdafx.h"
#include "SkinModelRender.h"

namespace
{
	//シェーダのファイルパス
	//通常描画用のシェーダーファイルパス
	const char* MODEL_FX_FILEPATH = "Assets/shader/model.fx";
	//非PBRのシェーダーファイルパス
	const char* MODEL_FX_FILEPATH_NONPBR = "Assets/shader/model_nonPBR.fx";

	//シャドウマップ描画用のシェーダーファイルパス
	const char* MODEL_FX_FILEPATH_SHADOWMAP = "Assets/shader/ShadowMap.fx";
	//シャドウレシーバー用のシェーダーファイルパス
	const char* MODEL_FX_FILEPATH_SHADOWRECIEVER = "Assets/shader/ShadowReciever.fx";
	//非PBRのシャドウレシーバー用のシェーダーファイルパス
	const char* MODEL_FX_FILEPATH_SHADOWRECIEVER_NONPBR = "Assets/shader/ShadowReciever_nonPBR.fx";

	//頂点シェーダのエントリーポイント名
	//通常のエントリーポイント
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	//スキンモデルのエントリーポイント
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";

	//ピクセルシェーダのエントリーポイント名
	//通常のエントリーポイント
	const char* PS_ENTRYPOINT_NAME = "PSMain";
	//スキンモデルのエントリーポイント
	const char* PS_SKIN_ENTRYPOINT_NAME = "PSSkinMain";
}

SkinModelRender::~SkinModelRender()
{
	//レンダリングエンジンからモデルを削除
	m_renderingEngine->DeleteCommonModel(&m_model);
	
	//シャドウキャスターフラグが立っていたら影用モデルも削除
	if (m_isShadowCaster) {
		m_renderingEngine->DeleteShadowModel(&m_shadowModel);
	}

}

bool SkinModelRender::Start()
{

	return true;
}

void SkinModelRender::Init(
	const char* modelFilePath, 
	EnModelUpAxis upAxis, 
	bool shadowCasterFlag, 
	bool shadowRecieverFlag, 
	const char* skeletonFilePath)
{
	m_renderingEngine = RenderingEngine::GetInstance();

	//シャドウキャスターフラグをつける
	SetShadowChastarFlag(shadowCasterFlag);
	
	//シャドウレシーバーフラグをつける
	SetShadowRecieverFlag(shadowRecieverFlag);

	//スケルトンが指定されていたら
	if (skeletonFilePath != nullptr) {
		//スケルトンを初期化
		m_skeleton.Init(skeletonFilePath);
		//モデルの初期化情報のメンバにスケルトンにスケルトンを登録
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	//通常描画用モデルを初期化
	{
		//共通処理
		m_modelInitData.m_tkmFilePath = modelFilePath;
		m_modelInitData.m_vsEntryPointFunc = VS_ENTRYPOINT_NAME;
		m_modelInitData.m_vsSkinEntryPointFunc = VS_SKIN_ENTRYPOINT_NAME;
		m_modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		m_modelInitData.m_modelUpAxis = upAxis;

		if (shadowRecieverFlag == false) {
			//シャドウレシーバーフラグがオフで、シェーダーが指定されていなければ(nullptrなら)通常のシェーダーを指定
			//すでにシェーダーが指定されているとき(≠nullptr)はそちらのシェーダーを使用する
			if (m_modelInitData.m_fxFilePath == nullptr) {
				m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH;
				//m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH_NONPBR;

			}

		}
		else {
			//シャドウレシーバーフラグがオンのとき
			//シャドウレシーバー用のシェーダーを指定
			m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH_SHADOWRECIEVER;
			//m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH_SHADOWRECIEVER_NONPBR;
			
			//シャドウマップを拡張SRV(シェーダーリソースビュー)に設定する
			m_modelInitData.m_expandShaderResoruceView[0] = &RenderingEngine::GetInstance()->GetShadowMap().GetRenderTargetTexture();
			//ライトビュープロジェクション行列を拡張定数バッファ―に設定する
			m_modelInitData.m_expandConstantBuffer_3 = (void*)&RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
			m_modelInitData.m_expandConstantBufferSize_3 = sizeof(RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix());

		}

		
		//モデルの初期化
		m_model.Init(m_modelInitData);
	}

	

	//通常モデルをレンダリングエンジンに追加
	m_renderingEngine->Add3DModelToCommonModel(&m_model);

	//シャドウキャスターフラグが立っていたら影用モデルをレンダリングエンジンに追加
	if (m_isShadowCaster == true) {

		//影用モデルを初期化
		{
			m_shadowModelInitData.m_tkmFilePath = modelFilePath;

			m_shadowModelInitData.m_fxFilePath = MODEL_FX_FILEPATH_SHADOWMAP;
			m_shadowModelInitData.m_vsEntryPointFunc = VS_ENTRYPOINT_NAME;
			m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

			m_shadowModelInitData.m_modelUpAxis = upAxis;

			m_shadowModel.Init(m_shadowModelInitData);
		}


		m_renderingEngine->Add3DModelToShadowModel(&m_shadowModel);
	}
	
}

void SkinModelRender::InitDirectionLight(DirectionLight* dirLight)
{
	m_modelInitData.m_expandConstantBuffer = dirLight->GetDirLightAddress();
	m_modelInitData.m_expandConstantBufferSize = sizeof(dirLight->GetDirLight());	
}

void SkinModelRender::InitPointLight(PointLight* ptLight)
{
	m_modelInitData.m_expandConstantBuffer_1 = ptLight->GetPointLightAddress();
	m_modelInitData.m_expandConstantBufferSize_1 = sizeof(ptLight->GetPointLight());
}

void SkinModelRender::InitSpotLight(SpotLight* spLight)
{
	m_modelInitData.m_expandConstantBuffer_2 = spLight->GetSpotLightAddress();
	m_modelInitData.m_expandConstantBufferSize_2 = sizeof(spLight->GetSpotLight());
}

void SkinModelRender::InitModel()
{
	m_model.Init(m_modelInitData);
}

void SkinModelRender::SetPosition(const Vector3& pos)
{
	m_position = pos;
}

void SkinModelRender::SetScale(const Vector3& scale)
{
	m_scale = scale;
}

void SkinModelRender::SetRotation(const Quaternion& rot)
{
	m_rot = rot;
}

void SkinModelRender::SetModelInitDataAdditional(const ModelInitData& modelInitData)
{
	m_modelInitData = modelInitData;
}

void SkinModelRender::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

void SkinModelRender::Update()
{
	//モデルの更新
	m_model.UpdateWorldMatrix(m_position,m_rot,m_scale);

	//スケルトンの更新
	m_skeleton.Update(m_model.GetWorldMatrix());

	//シャドウキャスターフラグがついていたら影モデルも更新
	if (m_isShadowCaster) {
		m_shadowModel.UpdateWorldMatrix(m_position, m_rot, m_scale);
	}

}