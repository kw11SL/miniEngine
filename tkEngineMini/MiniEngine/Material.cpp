#include "stdafx.h"
#include "Material.h"
#include <d3d12.h>
#include <DirectXTK/Src/d3dx12.h>

//ルートシグネチャとパイプラインステート周りはカリカリカリ
enum {
	enDescriptorHeap_CB,
	enDescriptorHeap_SRV,
	enNumDescriptorHeap
};
	
void Material::InitTexture(const TkmFile::SMaterial& tkmMat)
{
	const auto& nullTextureMaps = g_graphicsEngine->GetNullTextureMaps();

	const char* filePath = nullptr;
	char* map = nullptr;
	unsigned int mapSize;

	//アルベドマップ
	{
		if (tkmMat.albedoMap != nullptr) {
			//m_albedoMap.InitFromMemory(tkmMat.albedoMap.get(), tkmMat.albedoMapSize);
			filePath = tkmMat.albedoMapFilePath.c_str();
			map = tkmMat.albedoMap.get();
			mapSize = tkmMat.albedoMapSize;
		}
		else {
			/*m_albedoMap.InitFromMemory(
				nullTextureMaps.GetAlbedoMap().get(),
				nullTextureMaps.GetAlbedoMapSize());*/
			filePath = nullTextureMaps.GetAlbedoMapFilePath();
			map = nullTextureMaps.GetAlbedoMap().get();
			mapSize = nullTextureMaps.GetAlbedoMapSize();
		}
		//エンジンのファイルバンクから取得
		auto albedoMap = g_engine->GetTextureFromBank(filePath);
		//バンクになければ登録する
		if (albedoMap == nullptr){
			albedoMap = new Texture();
			albedoMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, albedoMap);
		}
		m_albedoMap = albedoMap;

	}

	//法線マップ
	{
		if (tkmMat.normalMap != nullptr) {
			//m_normalMap.InitFromMemory(tkmMat.normalMap.get(), tkmMat.normalMapSize);
			filePath = tkmMat.normalMapFilePath.c_str();
			map = tkmMat.normalMap.get();
			mapSize = tkmMat.normalMapSize;
		}
		else {
			/*m_normalMap.InitFromMemory(
				nullTextureMaps.GetNormalMap().get(),
				nullTextureMaps.GetNormalMapSize());*/
			filePath = nullTextureMaps.GetNormalMapFilePath();
			map = nullTextureMaps.GetNormalMap().get();
			mapSize = nullTextureMaps.GetNormalMapSize();
		}

		//エンジンのファイルバンクから取得
		auto normalMap = g_engine->GetTextureFromBank(filePath);
		//バンクになければ登録する
		if (normalMap == nullptr)
		{
			normalMap = new Texture();
			normalMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, normalMap);
		}
		m_normalMap = normalMap;

	}

	//スペキュラマップ
	{
		if (tkmMat.specularMap != nullptr) {
			//m_specularMap.InitFromMemory(tkmMat.specularMap.get(), tkmMat.specularMapSize);
			filePath = tkmMat.specularMapFilePath.c_str();
			map = tkmMat.specularMap.get();
			mapSize = tkmMat.specularMapSize;
		}
		else {
			////m_specularMap.InitFromMemory(
			//	nullTextureMaps.GetSpecularMap().get(),
			//	nullTextureMaps.GetSpecularMapSize());
			filePath = nullTextureMaps.GetSpecularMapFilePath();
			map = nullTextureMaps.GetSpecularMap().get();
			mapSize = nullTextureMaps.GetSpecularMapSize();
		}
		//エンジンのファイルバンクから取得
		auto specularMap = g_engine->GetTextureFromBank(filePath);
		//バンクになければ登録する
		if (specularMap == nullptr)
		{
			specularMap = new Texture();
			specularMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, specularMap);
		}
		m_specularMap = specularMap;

	}

	//リフレクションマップ
	{
		if (tkmMat.reflectionMap != nullptr) {
			//m_reflectionMap.InitFromMemory(tkmMat.reflectionMap.get(), tkmMat.reflectionMapSize);
			filePath = tkmMat.reflectionMapFilePath.c_str();
			map = tkmMat.reflectionMap.get();
			mapSize = tkmMat.reflectionMapSize;
		}
		else {
			/*m_reflectionMap.InitFromMemory(
				nullTextureMaps.GetReflectionMap().get(),
				nullTextureMaps.GetReflectionMapSize());*/
			filePath = nullTextureMaps.GetReflectionMapFilePath();
			map = nullTextureMaps.GetReflectionMap().get();
			mapSize = nullTextureMaps.GetReflectionMapSize();
		}
		//エンジンのファイルバンクから取得
		auto reflectionMap = g_engine->GetTextureFromBank(filePath);
		//バンクになければ登録する
		if (reflectionMap == nullptr)
		{
			reflectionMap = new Texture();
			reflectionMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, reflectionMap);
		}
		m_reflectionMap = reflectionMap;

	}

	//屈折マップ
	{
		if (tkmMat.refractionMap != nullptr) {
			//m_refractionMap.InitFromMemory(tkmMat.refractionMap.get(), tkmMat.refractionMapSize);
			filePath = tkmMat.refractionMapFilePath.c_str();
			map = tkmMat.refractionMap.get();
			mapSize = tkmMat.refractionMapSize;
		}
		else {
			/*m_refractionMap.InitFromMemory(
				nullTextureMaps.GetRefractionMap().get(),
				nullTextureMaps.GetRefractionMapSize());*/
			filePath = nullTextureMaps.GetRefractionMapFilePath();
			map = nullTextureMaps.GetRefractionMap().get();
			mapSize = nullTextureMaps.GetRefractionMapSize();
		}
		//エンジンのファイルバンクから取得
		auto refractionMap = g_engine->GetTextureFromBank(filePath);
		//バンクになければ登録する
		if (refractionMap == nullptr)
		{
			refractionMap = new Texture();
			refractionMap->InitFromMemory(map, mapSize);
			g_engine->RegistTextureToBank(filePath, refractionMap);
		}
		m_refractionMap = refractionMap;
	}
}
void Material::InitFromTkmMaterila(
	const TkmFile::SMaterial& tkmMat,
	//const wchar_t* fxFilePath,
	const char* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntryPointFunc,
	const char* psEntryPointFunc,
	const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat
)
{
	//テクスチャをロード。
	InitTexture(tkmMat);
	
	//定数バッファを作成。
	SMaterialParam matParam;
	matParam.hasNormalMap = m_normalMap->IsValid() ? 1 : 0;
	matParam.hasSpecMap = m_specularMap->IsValid() ? 1 : 0;
	m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);

	//ルートシグネチャを初期化。
	m_rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	//if (wcslen(fxFilePath) > 0) {
	//	//シェーダーを初期化。
	//	InitShaders(fxFilePath, vsEntryPointFunc, vsSkinEntryPointFunc, psEntryPointFunc);
	//	//パイプラインステートを初期化。
	//	InitPipelineState(colorBufferFormat);
	//}
	if (strlen(fxFilePath) > 0) {
		//シェーダーを初期化。
		InitShaders(fxFilePath, vsEntryPointFunc, vsSkinEntryPointFunc, psEntryPointFunc);
		//パイプラインステートを初期化。
		InitPipelineState(colorBufferFormat);
	}
}
void Material::InitPipelineState(const std::array<DXGI_FORMAT, MAX_RENDERING_TARGET>& colorBufferFormat)
{
	// 頂点レイアウトを定義する。
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//パイプラインステートを作成。
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature.Get();
	/*psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel.GetCompiledBlob());*/
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel->GetCompiledBlob());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = TRUE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	
//	psoDesc.NumRenderTargets = 3;
//	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。
//#ifdef SAMPE_16_02
//	psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;	//法線出力用。	
//	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;						//Z値。
//#else
//	psoDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;			//法線出力用。	
//	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	//Z値。
//#endif

	int numRenderTarget = 0;
	for (auto& format : colorBufferFormat) {
		if (format == DXGI_FORMAT_UNKNOWN) {
			//フォーマットが指定されていない場所が来たら終わり。
			break;
		}
		psoDesc.RTVFormats[numRenderTarget] = colorBufferFormat[numRenderTarget];
		numRenderTarget++;
	}
	psoDesc.NumRenderTargets = numRenderTarget;

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	m_skinModelPipelineState.Init(psoDesc);

	//続いてスキンなしモデル用を作成。
	//psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel.GetCompiledBlob());
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob());
	m_nonSkinModelPipelineState.Init(psoDesc);

	//続いて半透明マテリアル用。
	//psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob());
	psoDesc.BlendState.IndependentBlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

	
	m_transSkinModelPipelineState.Init(psoDesc);

	//psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel.GetCompiledBlob());
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob());
	m_transNonSkinModelPipelineState.Init(psoDesc);

}
void Material::InitShaders(
	//const wchar_t* fxFilePath,
	const char* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntriyPointFunc,
	const char* psEntryPointFunc
)
{
	//スキンなしモデル用のシェーダーをロードする。
	//m_vsNonSkinModel.LoadVS(fxFilePath, vsEntryPointFunc);
	m_vsNonSkinModel = g_engine->GetShaderFromBank(fxFilePath, vsEntryPointFunc);
	if (m_vsNonSkinModel == nullptr) {
		m_vsNonSkinModel = new Shader;
		m_vsNonSkinModel->LoadVS(fxFilePath, vsEntryPointFunc);
		g_engine->RegistShaderToBank(fxFilePath, vsEntryPointFunc, m_vsNonSkinModel);
	}
	/*m_vsNonSkinModel = g_engine->GetShaderFromBank(fxFilePath, vsEntryPointFunc);
	if (m_vsNonSkinModel == nullptr) {
		m_vsNonSkinModel = new Shader;
		m_vsNonSkinModel->LoadVS(fxFilePath, vsEntryPointFunc);
		g_engine->RegistShaderToBank(fxFilePath, vsEntryPointFunc, m_vsNonSkinModel);
	}*/

	//スキンありモデル用のシェーダーをロードする。
	//m_vsSkinModel.LoadVS(fxFilePath, vsSkinEntriyPointFunc);
	m_vsSkinModel = g_engine->GetShaderFromBank(fxFilePath, vsSkinEntriyPointFunc);
	if (m_vsSkinModel == nullptr) {
		m_vsSkinModel = new Shader;
		m_vsSkinModel->LoadVS(fxFilePath, vsSkinEntriyPointFunc);
		g_engine->RegistShaderToBank(fxFilePath, vsSkinEntriyPointFunc, m_vsSkinModel);
	}
	
	//m_psModel.LoadPS(fxFilePath, psEntryPointFunc);
	m_psModel = g_engine->GetShaderFromBank(fxFilePath, psEntryPointFunc);
	if (m_psModel == nullptr) {
		m_psModel = new Shader;
		m_psModel->LoadPS(fxFilePath, psEntryPointFunc);
		g_engine->RegistShaderToBank(fxFilePath, psEntryPointFunc, m_psModel);
	}
}
void Material::BeginRender(RenderContext& rc, int hasSkin)
{
	rc.SetRootSignature(m_rootSignature);
	
	if (hasSkin) {
	//	rc.SetPipelineState(m_skinModelPipelineState);
		rc.SetPipelineState(m_transSkinModelPipelineState);
	}
	else {
	//	rc.SetPipelineState(m_nonSkinModelPipelineState);
		rc.SetPipelineState(m_transNonSkinModelPipelineState);
	}
}
