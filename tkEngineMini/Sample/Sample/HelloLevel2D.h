#pragma once
#include "level2D/Level2D.h"

/// <summary>
/// ���x��2D�����̃T���v���N���X�B
/// </summary>
class HelloLevel2D : public IGameObject
{
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
private:
	Level2D m_level2D;		//���x��2D�B
	Sprite m_sprite;			//�X�v���C�g�B
	Vector3 m_position = Vector3::Zero;		//���W�B
	Vector3 m_scale = Vector3::One;		//�傫���B
};

