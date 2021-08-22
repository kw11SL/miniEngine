#pragma once
class GaussianBlur
{
public:
	GaussianBlur() {}
	~GaussianBlur() {}

	/// @brief �e�N�X�`���̏�����
	/// @param baseTexture �u���[���|���錳�ƂȂ�e�N�X�`��
	void Init(Texture* baseTexture);
	
	/// @brief �K�E�V�A���u���[��GPU��Ŏ��s
	/// @param rc �����_�����O�^�[�Q�b�g
	/// @param blurPower �u���[�̋��x�B�l���傫���قǃ{�P��
	void ExecuteOnGPU(RenderContext& rc, float blurPower);

	/// @brief �c�A���u���[��K�p�����e�N�X�`�����擾
	/// @return �u���[��K�p�����e�N�X�`��
	Texture& GetExecutedTexture()
	{
		return m_yBlurRenderTarget.GetRenderTargetTexture();
	}

private:
	//�����Ŏg�p����֐�
	
	/// @brief �����_�����O�^�[�Q�b�g�̏�����
	void InitRenderTargets();
	
	/// @brief �X�v���C�g�̏�����
	void InitSprites();
	
	/// @brief �d�݃e�[�u�����X�V 
	/// @param blurPower �u���[�̋��x�B�l���傫���قǃ{�P��
	void UpdateWeightsTable(float blurPower);

private:
	enum {NUM_WEIGHTS = 8};						//�d�݂̐�
	float m_weights[NUM_WEIGHTS] = {0.0f};		//�d�݃e�[�u��
	Texture* m_baseTexture = nullptr;			//�u���[��K�p����x�[�X�̃e�N�X�`��
	RenderTarget m_xBlurRenderTarget;			//���u���[�摜��`�悷�郌���_�����O�^�[�Q�b�g
	RenderTarget m_yBlurRenderTarget;			//�c�u���[�摜��`�悷�郌���_�����O�^�[�Q�b�g
	Sprite m_xBlurSprite;						//���u���[�摜��`�悷�邽�߂̃X�v���C�g
	Sprite m_yBlurSprite;						//�c�u���[�摜��`�悷�邽�߂̃X�v���C�g
};

