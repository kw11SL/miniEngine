#include "stdafx.h"
#include "NullTextureMaps.h"

void NullTextureMaps::Init()
{
	//各種テクスチャをロード。
	auto TexLoad = [&](
		const char* loadTexFilePath,
		std::unique_ptr<char[]>& outTexData,
		unsigned int& outTexSize
		) {
		FILE* fp = fopen(loadTexFilePath, "rb");
		if (fp == nullptr) {
			//nullテクスチャのロードに失敗。
			MessageBoxA(nullptr, "nullテクスチャのロードに失敗しました。", "エラー", MB_OK);
			std::abort();
		}
		//テクスチャサイズを計算。
		fseek(fp, 0L, SEEK_END);
		outTexSize = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		//メモリを確保
		outTexData = std::make_unique<char[]>(outTexSize);
		fread(outTexData.get(), outTexSize, 1, fp);
		fclose(fp);
	};

	//アルベドマップをロード。
	m_albedoMapFilePath = "Assets/modelData/preset/NullAlbedoMap.DDS";
	TexLoad(
		m_albedoMapFilePath,
		m_albedoMap,
		m_albedoMapSize);

	//法線マップをロード。
	m_normalMapFilePath = "Assets/modelData/preset/NullNormalMap.DDS";
	TexLoad(
		m_normalMapFilePath,
		m_normalMap,
		m_normalMapSize);

	m_specMapFilePath = "Assets/modelData/preset/specMap_None.DDS";
	//スペキュラマップマップをロード。
	TexLoad(
		m_specMapFilePath,
		m_specMap,
		m_specMapSize);

	//０の値を格納しているマップをロード。
	m_zeroValueMapFilePath = "Assets/modelData/preset/ZeroValueMap.DDS";
	TexLoad(
		m_zeroValueMapFilePath,
		m_zeroValueMap,
		m_zeroValueMapSize);

}