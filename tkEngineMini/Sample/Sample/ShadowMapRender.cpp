#include "stdafx.h"
#include "ShadowMapRender.h"

void ShadowMapRender::Init()
{
	//クリアカラー
	float clearColor[4] = {1.0f,1.0f,1.0f,1.0f};
	
	m_shadowMaps[0].Create(
		1024,
		1024,
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

}

void ShadowMapRender::Render(RenderContext& rc , Camera& lightCamera)
{

	int shadowMapNo = 0;
	//レンダリングターゲットの設定
	for (auto& shadowMap : m_shadowMaps) {

		rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
		rc.SetRenderTargetAndViewport(shadowMap);
		rc.ClearRenderTargetView(shadowMap);
	
		//モデルを描画
		for (auto& model : m_models[shadowMapNo]) {
			model->Draw(
				rc,
				lightCamera
			);
		}

		//描画終了でクリア
		m_models[shadowMapNo].clear();
		//書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
	
		//次のモデルへ
		shadowMapNo++;
	}


}