#include "stdafx.h"
#include "FontRender.h"

FontRender::~FontRender()
{
	//レンダリングエンジンのフォントデータを削除
	m_renderingEngine->DeleteFonts(m_fontData);
}

bool FontRender::Start()
{

	return true;
}

void FontRender::Init(const wchar_t* text)
{
	//レンダリングエンジンを取得
	m_renderingEngine = RenderingEngine::GetInstance();

	//フォントデータの構造体の各メンバを設定
	m_fontData.text = text;

	//レンダリングエンジンにフォントのデータを追加
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
	//フォントデータを更新
	UpdateFontData();

}