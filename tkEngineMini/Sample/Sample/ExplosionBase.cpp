#include "stdafx.h"
#include "ExplosionBase.h"

void ExplosionBase::Init(
	const Vector3& pos,
	const float scale
)
{
	//継承先で決めた初期化処理
	InitSub();

	m_position = pos;
	m_scale *= scale;

	//エフェクトを初期化
	InitEffect(m_effectFilePath);
	
	PlayEffect();
}

void ExplosionBase::InitSub()
{

}

void ExplosionBase::Update()
{
	//継承先で決めた処理
	UpdateSub();
}

void ExplosionBase::UpdateSub()
{

}

void ExplosionBase::InitEffect(const char16_t* filePath)
{
	//エフェクトを初期化
	m_effect.Init(filePath);
	m_effect.SetPosition(m_position);
	m_effect.SetRotation(m_rot);
	m_effect.SetScale(m_scale);
}

void ExplosionBase::DecLifeTime()
{
	m_lifeTime -= g_gameTime->GetFrameDeltaTime();
}

void ExplosionBase::Destroy()
{
	if (m_lifeTime <= 0.0f) {
		m_lifeTime = 0.0f;
		m_isExist = false;
	}
}

void ExplosionBase::EffectUpdate()
{
	m_effect.SetPosition(m_position);
	m_effect.SetRotation(m_rot);
	m_effect.SetScale(m_scale);

	m_effect.Update();
}