#include "stdafx.h"
#include "FontRender.h"

void FontRender::Init(
	wchar_t const* text,
	const Vector2& position,
	const Vector4& color,
	const float rotation,
	const float scale,
	const Vector2& pivot)
{
	m_fontData.m_text = text;
	m_fontData.m_position = position;
	m_fontData.m_color = color;
	m_fontData.m_rotation = rotation;
	m_fontData.m_scale = scale;
	m_fontData.m_pivot = pivot;
}


void FontRender::Update()
{

}