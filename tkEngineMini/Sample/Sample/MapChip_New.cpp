#include "stdafx.h"

#include "MapChip_New.h"
#include "Level_New.h"

MapChip_New::MapChip_New(const LevelObjectData& objData, const char* filePath)
{
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = filePath;
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
	initData.m_vsEntryPointFunc = "VSMain";

	//���f����ǂݍ��ށB
	m_model.Init(initData);

	//���f���̍X�V�B
	m_model.UpdateWorldMatrix(objData.position, objData.rotation, objData.scale);

	//�ÓI�����I�u�W�F�N�g���쐬�B
	//m_physicsStaticObject.CreateFromModel(m_model, m_model.GetWorldMatrix());
}

void MapChip_New::Draw(RenderContext& rc)
{
	m_model.Draw(rc);
}