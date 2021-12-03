#include "stdafx.h"

#include "MapChip_New.h"
#include "Level_New.h"

MapChip_New::MapChip_New(const LevelObjectData& objData, const char* filePath)
{
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = filePath;
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
	initData.m_vsEntryPointFunc = "VSMain";

	//モデルを読み込む。
	m_model.Init(initData);

	//モデルの更新。
	m_model.UpdateWorldMatrix(objData.position, objData.rotation, objData.scale);

	//静的物理オブジェクトを作成。
	//m_physicsStaticObject.CreateFromModel(m_model, m_model.GetWorldMatrix());
}

void MapChip_New::Draw(RenderContext& rc)
{
	m_model.Draw(rc);
}