#pragma once
#include "Bloom.h"
#include "ShadowMapRender.h"

//�t�H���g�f�[�^�̍\����
struct SFontData;


/// @brief �����_�����O�G���W��
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
	void CommonRendering(RenderContext& rc);

	/// @brief �V���h�E�}�b�v�ɕ`��
	void RenderToShadowMap(RenderContext& rc,Camera camera);

	/// @brief �u���[���̎��s
	/// @param rc �����_�����O�R���e�L�X�g
	void BloomRendering(RenderContext& rc, RenderTarget& mainRT);

	/// @brief �X�v���C�g��`��
	/// @param rc 
	void SpriteRendering(RenderContext& rc);

	/// @brief �t�H���g�̕`��
	/// @param rc �����_�����O�R���e�L�X�g
	void FontRendering(RenderContext& rc);

	/// @brief ���C�g�J�����̍X�V
	void UpdateLightCamera();

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

	/// @brief �t�H���g�f�[�^�̔z��Ƀt�H���g�f�[�^��ǉ�
	/// @param fontData 
	void AddFontDataToFonts(SFontData& fontData)
	{
		std::vector<SFontData*>::iterator itr;
		itr = std::find(m_fontDataVector.begin(), m_fontDataVector.end(), &fontData);

		//�r���Ō��������ꍇ�͔z��ɒǉ����Ȃ�
		if (itr != m_fontDataVector.end()) {
			return;
		}
		//�r���Ō�����Ȃ������ꍇ(�C�e���[�^���I�[)�܂��Ȃ��̂Œǉ�
		else if (itr == m_fontDataVector.end()) {
			m_fontDataVector.push_back(&fontData);
		}
	}

	/// @brief �X�v���C�g�̔z��ɃX�v���C�g��ǉ�
	/// @param sprite 
	void AddSpriteToSprites(Sprite& sprite) 
	{
		std::vector<Sprite*>::iterator itr;
		itr = std::find(m_sprites.begin(),m_sprites.end(),&sprite);

		//�r���Ō��������ꍇ�͔z��ɒǉ����Ȃ�
		if (itr != m_sprites.end()) {
			return;
		}
		//�r���Ō�����Ȃ������ꍇ(�C�e���[�^���I�[)�܂��Ȃ��̂Œǉ�
		else if(itr == m_sprites.end()) {
			m_sprites.push_back(&sprite);
		}

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

	/// @brief �X�v���C�g���폜
	/// @param sprite 
	void DeleteSprite(Sprite& sprite);

	/// @brief �t�H���g�f�[�^�̍폜����
	/// @param fontData 
	void DeleteFonts(SFontData& fontData);

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

	

private:
	static RenderingEngine* m_renderingEngine;	//�B��̃I�u�W�F�N�g

	ShadowMapRender m_shadowMap;				//�V���h�E�}�b�v
	std::vector<Model*> m_commonModels;			//�ʏ�`��p���f��
	std::vector<Sprite*> m_sprites;				//�X�v���C�g�̔z��
	std::vector<SFontData*> m_fontDataVector;	//�t�H���g�f�[�^�̔z��

	RenderTarget m_mainRenderTarget;			//���C�������_�����O�^�[�Q�b�g
	Bloom m_bloom;								//�u���[��

	Camera m_lightCamera;						//���C�g�J����

};