#pragma once

#include "Bloom.h"
#include "ShadowMapRender.h"

//class bloom;
//class RenderTarget;

class RenderingEngine
{
public:
	RenderingEngine() {}
	~RenderingEngine() {}
	
	/// @brief �����_�����O�G���W���̏������܂Ƃ�
	void Init();
	
	/// @brief ���������s 
	/// @param rc �����_�����O�R���e�L�X�g
	void Execute(RenderContext& rc);

	/// @brief �ʏ�`�揈��
	/// @param rc �����_�����O�R���e�L�X�g
	void CommonRender(RenderContext& rc);

	/// @brief �V���h�E�}�b�v�ɕ`��
	void RenderToShadowMap(RenderContext& rc,Camera camera);

	/// @brief �����_�����O�^�[�Q�b�g���擾
	/// @return �����_�����O�^�[�Q�b�g
	RenderTarget& GetRenderTarget() 
	{
		return m_mainRenderTarget; 
	}

	/// @brief �e�`��p���f����ǉ�
	/// @param model ���f��
	void Add3DModelToShadowModel(Model& model)
	{
		m_shadowMap.AddModel(model);
	}

	/// @brief �ʏ�`��p���f����ǉ�
	/// @param model 
	void Add3DModelToCommonModel(Model& model)
	{
		m_commonModels.push_back(&model);
	}

private:
	//�����Ŏ��s����֐�

	/// @brief ���C�g�J�����̏�����
	void InitLightCamera();

	/// @brief ���C���̃����_�����O�^�[�Q�b�g�̏�����
	void InitMainRenderTarget();

	/// @brief �V���h�E�}�b�v�̏�����
	void InitShadowMap();
	
	/// @brief �u���[���̏�����
	/// @param mainRT �����_�����O�^�[�Q�b�g
	void InitBloom(RenderTarget& mainRT);

	/// @brief �u���[���̎��s
	/// @param rc �����_�����O�R���e�L�X�g
	void BloomRendering(RenderContext& rc, RenderTarget& mainRT);

private:
	//std::vector<Model*> m_shadowModels;		//�e�`��p���f��
	ShadowMapRender m_shadowMap;				//�V���h�E�}�b�v
	std::vector<Model*> m_commonModels;			//�ʏ�`��p���f��

	RenderTarget m_mainRenderTarget;			//���C�������_�����O�^�[�Q�b�g
	Bloom m_bloom;								//�u���[��

	Camera m_lightCamera;						//���C�g�J����

};