#pragma once

//class RenderTarget;
class Model;

class ShadowMapRender
{
public:
	/// @brief �V���h�E�}�b�v�̏�����
	void Init();

	/// @brief �V���h�E�}�b�v�ɕ`�悷�郂�f����o�^
	/// @param model 
	void AddModel(Model* model) 
	{ 
		m_modelsArray.push_back(model); 
	}

	/// @brief �V���h�E�}�b�v���擾
	/// @return �V���h�E�}�b�v
	RenderTarget& GetShadowMap()
	{
		return m_shadowMap;
	}

	/// @brief ���f�����폜
	/// @param model ���f��
	void DeleteModel(Model* model);

	/*/// @brief �J�X�P�[�h�V���h�E�p�̃V���h�E�}�b�v�ɕ`�悷�郂�f����o�^
	/// @param model1 //�ߌi�p���f��
	/// @param model2 //���i�p���f��
	/// @param model3 //���i�p���f��
	void MultiAddModel(Model& model1,Model& model2,Model& model3) 
	{ 
		m_models[0].push_back(&model1);
		m_models[1].push_back(&model2);
		m_models[2].push_back(&model3);
	}*/

	/// @brief �V���h�E�}�b�v�ւ̕`�揈��
	/// @param rc �����_�����O�R���e�L�X�g
	/// @param lightCamera ���C�g�J����
	void Render(RenderContext& rc, Camera& lightCamera);
	
	/*/// @brief �V���h�E�}�b�v�ւ̕`�揈��(�J�X�P�[�h�V���h�E�p) 
	/// @param rc �����_�����O�R���e�L�X�g
	/// @param lightCamera ���C�g�J����
	void MultiRender(RenderContext& rc, Camera& lightCamera);*/

private:
	/// @brief �V���h�E�}�b�v��������
	void InitShadowMap();

	/// @brief �J�X�P�[�h�V���h�E�p�ɕ����̃V���h�E�}�b�v��������
	void InitMultiShadowMap();

private:
	
	//1�����e�𐶐�����ꍇ
	std::vector<Model*> m_modelsArray;				//�e�`��p���f���̉ϒ��z��
	RenderTarget m_shadowMap;						//�V���h�E�}�b�v
	
	////�J�X�P�[�h�V���h�E�p
	////�����ɍ��킹�����f���ƃV���h�E�}�b�v���K�v
	//std::vector<Model*> m_models[NUM_SHADOW_MAP];	//���f���̔z��
	//RenderTarget m_shadowMaps[NUM_SHADOW_MAP];		//�V���h�E�}�b�v�̔z��

};

