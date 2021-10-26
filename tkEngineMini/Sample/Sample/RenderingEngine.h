#pragma once

#include "Bloom.h"
#include "ShadowMapRender.h"

//class bloom;
//class RenderTarget;

//�V���O���g���p�^�[��
class RenderingEngine
{
public:

	/// @brief �I�u�W�F�N�g�̍쐬
	static void CreateInstance()
	{
		if (m_renderingEngine == nullptr) {
			m_renderingEngine = new RenderingEngine;
		}
	}
	
	/// @brief �I�u�W�F�N�g�̎擾
	/// @return ���g�̃I�u�W�F�N�g
	static RenderingEngine* GetInstance()
	{
		return m_renderingEngine;
	}
	
	/// @brief �I�u�W�F�N�g�̍폜
	static void DeleteInstance()
	{
		delete m_renderingEngine;
		m_renderingEngine = nullptr;
	}

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

	/// @brief �V���h�E�}�b�v�����_�[���̃V���h�E�}�b�v���擾
	/// @return �V���h�E�}�b�v
	RenderTarget& GetShadowMap()
	{
		return m_shadowMap.GetShadowMap();
	}

	/// @brief ���C�g�J�������擾
	/// @return ���C�g�J����
	Camera& GetLightCamera()
	{
		return m_lightCamera;
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

	/// @brief �ʏ�`�惂�f���̍폜����
	/// @param model 
	void DeleteCommonModel(Model& model);

	/// @brief	�e�`��p���f���̍폜����
	/// @param model 
	void DeleteShadowModel(Model& model)
	{
		m_shadowMap.DeleteModel(model);
	}

private:
	//�����Ŏ��s����֐�
	RenderingEngine() {}
	~RenderingEngine() {}


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
	static RenderingEngine* m_renderingEngine;	//�B��̃I�u�W�F�N�g

	//std::vector<Model*> m_shadowModels;		//�e�`��p���f��
	ShadowMapRender m_shadowMap;				//�V���h�E�}�b�v
	std::vector<Model*> m_commonModels;			//�ʏ�`��p���f��


	RenderTarget m_mainRenderTarget;			//���C�������_�����O�^�[�Q�b�g
	Bloom m_bloom;								//�u���[��

	Camera m_lightCamera;						//���C�g�J����

};