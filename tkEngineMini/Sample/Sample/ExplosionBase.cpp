#include "stdafx.h"
#include "ExplosionBase.h"

void ExplosionBase::Init(
	const Vector3& pos,
	const float scale
)
{
	//�p����Ō��߂�����������
	InitSub();

	m_position = pos;
	m_scale *= scale;

	//�G�t�F�N�g��������
	InitEffect(m_effectFilePath);
	
	PlayEffect();
}

void ExplosionBase::InitSub()
{

}

void ExplosionBase::Update()
{
	//�p����Ō��߂�����
	UpdateSub();
}

void ExplosionBase::UpdateSub()
{

}

void ExplosionBase::InitEffect(const char16_t* filePath)
{
	//�G�t�F�N�g��������
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