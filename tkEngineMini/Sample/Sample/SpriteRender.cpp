#include "stdafx.h"
#include "SpriteRender.h"

namespace
{
	const char* SPRITE_FX_FILEPATH = "Assets/shader/sprite.fx";
}

bool SpriteRender::Start()
{
	return true;
}

void SpriteRender::Init(const char* spriteFilePath, const UINT& width, const UINT& height, AlphaBlendMode alphaBrendMode)
{
	m_spriteInitData.m_ddsFilePath[0] = spriteFilePath;
	m_spriteInitData.m_vsEntryPointFunc = "VSMain";
	m_spriteInitData.m_psEntryPoinFunc = "PSMain";
	m_spriteInitData.m_fxFilePath = SPRITE_FX_FILEPATH;
	m_spriteInitData.m_width = width;
	m_spriteInitData.m_height = height;
	m_spriteInitData.m_alphaBlendMode = alphaBrendMode;

	m_sprite.Init(m_spriteInitData);
}

void SpriteRender::InitShader(const char* fxFilePath, const char* vsEntryPoint, const char* psEntryPoint)
{
	m_spriteInitData.m_fxFilePath = fxFilePath;
	m_spriteInitData.m_vsEntryPointFunc = vsEntryPoint;
	m_spriteInitData.m_psEntryPoinFunc = psEntryPoint;

	m_sprite.Init(m_spriteInitData);
}

void SpriteRender::SetPosition(const Vector3& pos)
{
	m_position = pos;
}

void SpriteRender::SetRotation(const Quaternion& rot)
{
	m_rotation = rot;
}

void SpriteRender::SetScale(const Vector3& scale)
{
	m_scale = scale;
}

void SpriteRender::SetPivot(const Vector2& pivot)
{
	m_pivot = pivot;
}

void SpriteRender::SetColor(const Vector4& color)
{
	m_color = color;
	m_sprite.SetColor(m_color);
}

void SpriteRender::Render(RenderContext& rc)
{
	m_sprite.Draw(rc);
}

void SpriteRender::Update()
{
	m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
	SetColor(m_color);
}