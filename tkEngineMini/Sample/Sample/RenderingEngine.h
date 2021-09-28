#pragma once

#include "Bloom.h"

//class bloom;
//class RenderTarget;

class RenderingEngine
{
public:
	RenderingEngine() {}
	~RenderingEngine() {}
	
	/// @brief �����_�����O�G���W���̏�����
	void Init();
	
	/// @brief ���������s 
	/// @param rc �����_�����O�R���e�L�X�g
	void Execute(RenderContext& rc);

	RenderTarget& GetRenderTarget() { return m_mainRenderTarget; }

private:
	/// @brief ���C���̃����_�����O�^�[�Q�b�g�̏�����
	void InitMainRenderTarget();
	
	/// @brief �u���[���̏�����
	/// @param mainRT �����_�����O�^�[�Q�b�g
	void InitBloom(RenderTarget& mainRT);

	/// @brief �u���[���̎��s
	/// @param rc �����_�����O�R���e�L�X�g
	void BloomRendering(RenderContext& rc, RenderTarget& mainRT);

private:
	RenderTarget m_mainRenderTarget;		//���C�������_�����O�^�[�Q�b�g
	
	Bloom m_bloom;							//�u���[��

};