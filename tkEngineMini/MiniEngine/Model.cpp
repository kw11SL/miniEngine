#include "stdafx.h"
#include "Model.h"
#include "Material.h"

void Model::Init(const ModelInitData& initData)
{
	MY_ASSERT(
		initData.m_fxFilePath, 
		"error : initData.m_fxFilePathが指定されていません。"
	);
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePathが指定されていません。"
	);
	//内部のシェーダーをロードする処理が求めているのが
	//wchar_t型の文字列なので、ここで変換しておく。
	//wchar_t wfxFilePath[256] = {L""};
	//if (initData.m_fxFilePath != nullptr) {
	//	//MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
	//	//std::abort();
	//	mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	//}
	
	if (initData.m_skeleton != nullptr) {
		//スケルトンが指定されている。
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}
	
	m_modelUpAxis = initData.m_modelUpAxis;

	//m_tkmFile.Load(initData.m_tkmFilePath);
	auto tkmFile = g_engine->GetTkmFileFromBank(initData.m_tkmFilePath);
	if (tkmFile == nullptr) {
		//未登録
		tkmFile = new TkmFile;
		tkmFile->Load(initData.m_tkmFilePath);
		g_engine->RegistTkmFileToBank(initData.m_tkmFilePath, tkmFile);
	}
	m_tkmFile = tkmFile;
	
	m_meshParts.InitFromTkmFile(
		/*m_tkmFile, 
		wfxFilePath, */
		*m_tkmFile,
		initData.m_fxFilePath,
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandConstantBuffer_1,		
		initData.m_expandConstantBufferSize_1,	
		initData.m_expandConstantBuffer_2,		
		initData.m_expandConstantBufferSize_2,
		initData.m_expandConstantBuffer_3,
		initData.m_expandConstantBufferSize_3,
		initData.m_expandShaderResoruceView,
		initData.m_colorBufferFormat
	);

	UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	
}

void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
{
	Matrix mBias;
	if (m_modelUpAxis == enModelUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(Math::PI * -0.5f);
	}
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(pos);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world = mBias * mScale * mRot * mTrans;
}

void Model::ChangeAlbedoMap(const char* materialName, Texture& albedoMap)
{
	m_meshParts.QueryMeshs([&](const SMesh& mesh) {
		//todo マテリアル名をtkmファイルに出力したなかった・・・。
		//todo 今は全マテリアル差し替えます
		for (Material* material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
	});
	//ディスクリプタヒープの再作成。
	m_meshParts.CreateDescriptorHeaps();
	
}
void Model::Draw(RenderContext& rc)
{
	//描画フラグがオフだったら処理しない
	if (m_isDraw == false) {
		return;
	}

	m_meshParts.Draw(
		rc, 
		m_world, 
		g_camera3D->GetViewMatrix(), 
		g_camera3D->GetProjectionMatrix()
	);
}
void Model::Draw(RenderContext& rc, Camera& camera)
{
	//描画フラグがオフだったら処理しない
	if (m_isDraw == false) {
		return;
	}

	Draw(rc, camera.GetViewMatrix(), camera.GetProjectionMatrix());
}
void Model::Draw(RenderContext& rc, const Matrix& viewMatrix, const Matrix& projMatrix)
{
	//描画フラグがオフだったら処理しない
	if (m_isDraw == false) {
		return;
	}

	m_meshParts.Draw(
		rc,
		m_world,
		viewMatrix,
		projMatrix
	);
}

