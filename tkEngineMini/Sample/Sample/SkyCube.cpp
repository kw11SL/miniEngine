#include "stdafx.h"
#include "SkyCube.h"

namespace {
	//�t�@�C���p�X�֘A
	const char* MODEL_FILEPATH = "Assets/modelData/skyCube/sky.tkm";							//���f���̃t�@�C���p�X
	const char* SHADER_FILEPATH = "Assets/shader/skyCubeMap.fx";								//�V�F�[�_�[�̃t�@�C���p�X
	const wchar_t* TEXTURE_FILEPATH = L"Assets/modelData/skyCube/skyCubeMapDay_Toon_04.dds";	//�e�N�X�`���̃t�@�C���p�X

	//���邳�֘A
	const float MIN_LUMINANCE = 0.0f;		//���邳�̍Œ�l
	const float MAX_LUMINANCE = 1.2f;		//���邳�̍ő�l
	const float LUMINACE_ADDRATE = 0.01f;	//�t�F�[�h�C���̊���
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

	//�ǉ��̏����������쐬
	ModelInitData initData;
	initData.m_tkmFilePath = MODEL_FILEPATH;
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
		MODEL_FILEPATH,
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

void SkyCube::FadeIn(const float addRate)
{
	//���邳�̍ő�l�ɒB���Ă����珈�����Ȃ�
	if (m_luminance >= MAX_LUMINANCE) {
		return;
	}

	//���邳�̍ő�l�ɒB����܂ő����Ă���
	if (m_luminance <= MAX_LUMINANCE){
		m_luminance += addRate;
	}
}

void SkyCube::Update()
{
	//�Q�[�����łȂ���Ώ������Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//�X�e�[�W�J�n���̃t�F�[�h�C������
	FadeIn(LUMINACE_ADDRATE);

	/*if (m_isDirty)
	{
		m_skinModelRender->UpdateWorldMatrix(m_position, m_rot, m_scale);
		m_skinModelRender->Update();
		m_isDirty = false;
	}*/
}