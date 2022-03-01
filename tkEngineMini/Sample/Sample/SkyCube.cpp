#include "stdafx.h"
#include "SkyCube.h"

namespace {
	const char* MODEL_FILEPATH = "Assets/modelData/skyCube/sky.tkm";
	const char* SHADER_FILEPATH = "Assets/shader/skyCubeMap.fx";
	const wchar_t* TEXTURE_FILEPATH = L"Assets/modelData/skyCube/skyCubeMapDay_Toon_04.dds";
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

void SkyCube::Init(const char* filePath)
{
	m_skinModelRender = NewGO<SkinModelRender>(0);

	//�ǉ��̏����������쐬
	ModelInitData initData;
	initData.m_tkmFilePath = filePath;
	initData.m_fxFilePath = SHADER_FILEPATH;
	initData.m_modelUpAxis = enModelUpAxisY;
	initData.m_vsEntryPointFunc = "VSMain";
	initData.m_psEntryPointFunc = "PSMain";

	//�e�N�X�`����������
	m_texture.InitFromDDSFile(TEXTURE_FILEPATH,true);
	//SRV�Ƀe�N�X�`���̃A�h���X��n��
	initData.m_expandShaderResoruceView[0] = &m_texture;
	//�g���o�b�t�@�ɖ��邳�̏���n��
	initData.m_expandConstantBuffer = &m_luminance;
	initData.m_expandConstantBufferSize = sizeof(m_luminance);

	//�ǉ��̏���������o�^
	m_skinModelRender->SetModelInitDataAdditional(initData);

	//���f����������(�����_�����O�G���W���ɓo�^���邽��)
	m_skinModelRender->Init(
		filePath,
		enModelUpAxisY,
		false,
		false,
		nullptr
	);

	//���W�A��]�A�g�嗦��ݒ�
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rot);
	m_skinModelRender->SetScale(m_scale);
}

void SkyCube::Update()
{
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_luminance -= 0.01f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_luminance += 0.01f;
	}


	if (m_isDirty)
	{
		m_skinModelRender->UpdateWorldMatrix(m_position, m_rot, m_scale);
		m_skinModelRender->Update();
		m_isDirty = false;
	}
}