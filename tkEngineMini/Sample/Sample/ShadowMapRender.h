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
	void AddModel(Model& model) { m_models[0].push_back(&model); }

	/// @brief �`�揈��
	/// @param rc �����_�����O�R���e�L�X�g
	/// @param lightCamera ���C�g�J����
	void Render(RenderContext& rc, Camera& lightCamera);


private:
	//Model* m_model = nullptr;
	std::vector<Model*> m_models[4];
	RenderTarget m_shadowMaps[4];

};

