#include "stdafx.h"
#include "SkinModelRender.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace
{
	//シェーダのファイルパス
	//通常描画用のシェーダーファイルパス
	const char* MODEL_FX_FILEPATH = "Assets/shader/model.fx";
	//シャドウマップ描画用のシェーダーファイルパス
	const char* MODEL_FX_FILEPATH_SHADOWMAP = "Assets/shader/ShadowMap.fx";
	//シャドウレシーバー用のシェーダーファイルパス
	const char* MODEL_FX_FILEPATH_SHADOWRECIEVER = "Assets/shader/ShadowReciever.fx";


	//シェーダのエントリーポイント名
	//通常のエントリーポイント
	const char* VS_ENTRYPOINT_NAME = "VSMain";
	//スキンモデルのエントリーポイント
	const char* VS_SKIN_ENTRYPOINT_NAME = "VSSkinMain";
}

bool SkinModelRender::Start()
{

	return true;
}

void SkinModelRender::Init(const char* modelFilePath, EnModelUpAxis upAxis , RenderingEngine& renderingEngine , bool shadowCasterFlag , bool shadowRecieverFlag)
{
	m_renderingEngine = &renderingEngine;
	
	//シャドウキャスターフラグをつける
	SetShadowChastarFlag(shadowCasterFlag);
	
	//シャドウレシーバーフラグをつける
	SetShadowRecieverFlag(shadowRecieverFlag);


	//通常描画用モデルを初期化
	{
		//共通処理
		m_modelInitData.m_tkmFilePath = modelFilePath;
		m_modelInitData.m_vsEntryPointFunc = VS_ENTRYPOINT_NAME;
		m_modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		/*m_modelInitData.m_vsEntryPointFunc = "VSMain";
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";*/
		m_modelInitData.m_modelUpAxis = upAxis;

		if (shadowRecieverFlag == false) {
			//シャドウレシーバーフラグがオフなら通常のシェーダーを指定
			m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH;
		}
		else {
			//シャドウレシーバーフラグがオンのとき
			//シャドウレシーバー用のシェーダーを指定
			m_modelInitData.m_fxFilePath = MODEL_FX_FILEPATH_SHADOWRECIEVER;
			//シャドウマップを拡張SRV(シェーダーリソースビュー)に設定する
			m_modelInitData.m_expandShaderResoruceView[0] = &RenderingEngine::GetInstance()->GetShadowMap().GetRenderTargetTexture();
			//ライトビュープロジェクション行列を拡張定数バッファ―に設定する
			m_modelInitData.m_expandConstantBuffer_3 = (void*)&RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
			m_modelInitData.m_expandConstantBufferSize_3 = sizeof(RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix());

		}

		
		//モデルの初期化
		m_model.Init(m_modelInitData);
	}

	//影用モデルを初期化
	{
		m_shadowModelInitData.m_tkmFilePath = modelFilePath;

		m_shadowModelInitData.m_fxFilePath = MODEL_FX_FILEPATH_SHADOWMAP;
		m_shadowModelInitData.m_vsEntryPointFunc = VS_ENTRYPOINT_NAME;
		m_shadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;

		m_shadowModelInitData.m_modelUpAxis = upAxis;

		m_shadowModel.Init(m_shadowModelInitData);
	}

	//通常モデルをレンダリングエンジンに追加
	m_renderingEngine->Add3DModelToCommonModel(m_model);

	//シャドウキャスターフラグが立っていたら影用モデルをレンダリングエンジンに追加
	if (m_isShadowCaster == true) {
		m_renderingEngine->Add3DModelToShadowModel(m_shadowModel);
	}
	
}

void SkinModelRender::InitDirectionLight(DirectionLight* dirLight)
{
	m_modelInitData.m_expandConstantBuffer = dirLight->GetDirLightAddress();
	m_modelInitData.m_expandConstantBufferSize = sizeof(dirLight->GetDirLight());
	
	m_model.Init(m_modelInitData);
}

void SkinModelRender::InitPointLight(PointLight* ptLight)
{
	m_modelInitData.m_expandConstantBuffer_1 = ptLight->GetPointLightAddress();
	m_modelInitData.m_expandConstantBufferSize_1 = sizeof(ptLight->GetPointLight());

	m_model.Init(m_modelInitData);
}

void SkinModelRender::InitSpotLight(SpotLight* spLight)
{
	m_modelInitData.m_expandConstantBuffer_2 = spLight->GetSpotLightAddress();
	m_modelInitData.m_expandConstantBufferSize_2 = sizeof(spLight->GetSpotLight());

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

void SkinModelRender::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

void SkinModelRender::Update()
{
	//モデルの更新
	m_model.UpdateWorldMatrix(m_position,m_rot,m_scale);

	//シャドウキャスターフラグがついていたら影モデルも更新
	if (m_isShadowCaster) {
		m_shadowModel.UpdateWorldMatrix(m_position, m_rot, m_scale);
	}

}