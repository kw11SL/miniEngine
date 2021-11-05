#include "stdafx.h"
#include "FontRender.h"

FontRender::~FontRender()
{
	//�����_�����O�G���W���̃t�H���g�f�[�^���폜
	m_renderingEngine->DeleteFonts(m_fontData);
}

bool FontRender::Start()
{

	return true;
}

void FontRender::Init(const wchar_t* text)
{
	//�����_�����O�G���W�����擾
	m_renderingEngine = RenderingEngine::GetInstance();

	//�t�H���g�f�[�^�̍\���̂̊e�����o��ݒ�
	m_fontData.text = text;

	//�����_�����O�G���W���Ƀt�H���g�̃f�[�^��ǉ�
	m_renderingEngine->AddFontDataToFonts(m_fontData);
}

void FontRender::UpdateFontData()
{
	m_fontData.text = m_text;
	m_fontData.position = m_position;
	m_fontData.color = m_color;
	m_fontData.rotation = m_rotation;
	m_fontData.scale = m_scale;
	m_fontData.pivot = m_pivot;
}

void FontRender::Update()
{
	//�t�H���g�f�[�^���X�V
	UpdateFontData();

}