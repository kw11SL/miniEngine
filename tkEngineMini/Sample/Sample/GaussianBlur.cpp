#include "stdafx.h"
#include "GaussianBlur.h"

namespace
{
	//�K�E�V�A���u���[�p�V�F�[�_�̃t�@�C���p�X
	const char* GAUSSIANBLUR_SHADER_FILEPATH = "Assets/shader/GaussianBlur.fx";
	//���u���[�p�V�F�[�_�̒��_�V�F�[�_�G���g���[�|�C���g��
	const char* XBLUR_VS_ENTRY_POINT_NAME = "VSXBlur";
	//�c�u���[�p�V�F�[�_�̒��_�V�F�[�_�G���g���[�|�C���g��
	const char* YBLUR_VS_ENTRY_POINT_NAME = "VSYBlur";
	//�K�E�V�A���u���[�p�V�F�[�_�̃s�N�Z���V�F�[�_�G���g���[�|�C���g��
	const char* GAUSSIANBLUR_PS_ENTRY_POINT_NAME = "PSBlur";
}


void GaussianBlur::Init(Texture* baseTexture)
{
	m_baseTexture = baseTexture;

	//�����_�����O�^�[�Q�b�g��������
	InitRenderTargets();
	
	//�X�v���C�g��������
	InitSprites();
}

void GaussianBlur::ExecuteOnGPU(RenderContext& rc, float blurPower)
{
	//�d�݃e�[�u���̍X�V
	UpdateWeightsTable(blurPower);

	//���u���[�̎��s
	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(m_xBlurRenderTarget);
	//�����_�����O�^�[�Q�b�g�̐ݒ�
	rc.SetRenderTargetAndViewport(m_xBlurRenderTarget);
	//�����_�����O�^�[�Q�b�g�̃N���A
	rc.ClearRenderTargetView(m_xBlurRenderTarget);
	//�h���[
	m_xBlurSprite.Draw(rc);
	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_xBlurRenderTarget);

	//�c�u���[�̎��s
	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(m_yBlurRenderTarget);
	//�����_�����O�^�[�Q�b�g�̐ݒ�
	rc.SetRenderTargetAndViewport(m_yBlurRenderTarget);
	//�����_�����O�^�[�Q�b�g�̃N���A
	rc.ClearRenderTargetView(m_yBlurRenderTarget);
	//�h���[
	m_yBlurSprite.Draw(rc);
	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_yBlurRenderTarget);

}

void GaussianBlur::InitRenderTargets()
{
	//���u���[�p�����_�����O�^�[�Q�b�g���쐬
	m_xBlurRenderTarget.Create(
		//�����͌��ɂȂ�e�N�X�`���̔���
		m_baseTexture->GetWidth() / 2,
		//�����͂��̂܂�
		m_baseTexture->GetHeight(),
		1,
		1,
		//�t�H�[�}�b�g�͌��ɂȂ�e�N�X�`���Ɠ���
		m_baseTexture->GetFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

	//�c�u���[�p�����_�����O�^�[�Q�b�g���쐬
	m_yBlurRenderTarget.Create(
		//�����͌��ɂȂ�e�N�X�`���̔���
		m_baseTexture->GetWidth() / 2,
		//����������
		m_baseTexture->GetHeight() / 2,
		1,
		1,
		//�t�H�[�}�b�g�͌��ɂȂ�e�N�X�`���Ɠ���
		m_baseTexture->GetFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

}

void GaussianBlur::InitSprites()
{
	//���u���[�p�̃X�v���C�g��������
	{
		//���u���[�p�������f�[�^���쐬
		SpriteInitData xBlurSpriteInitData;
		xBlurSpriteInitData.m_fxFilePath = GAUSSIANBLUR_SHADER_FILEPATH;
		xBlurSpriteInitData.m_vsEntryPointFunc = XBLUR_VS_ENTRY_POINT_NAME;
		xBlurSpriteInitData.m_psEntryPoinFunc = GAUSSIANBLUR_PS_ENTRY_POINT_NAME;
		//�X�v���C�g�̉𑜓x�͉��u���[�p�����_�����O�^�[�Q�b�g�Ɠ���
		xBlurSpriteInitData.m_width = m_xBlurRenderTarget.GetWidth();
		//���������l
		xBlurSpriteInitData.m_height = m_xBlurRenderTarget.GetHeight();
		//�e�N�X�`���̓x�[�X�̃e�N�X�`��
		xBlurSpriteInitData.m_textures[0] = m_baseTexture;
		//�������ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
		xBlurSpriteInitData.m_colorBufferFormat[0] = m_xBlurRenderTarget.GetColorBufferFormat();
		//�萔�o�b�t�@�Ƀu���[�p�p�����[�^��ݒ�
		//�u���[�̏d�݃e�[�u����ݒ�
		xBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
		xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

		//�������f�[�^����ɉ��u���[�p�X�v���C�g��������
		m_xBlurSprite.Init(xBlurSpriteInitData);
	}

	//�c�u���[�p�̃X�v���C�g��������
	{
		//�c�u���[�p�������f�[�^���쐬
		SpriteInitData yBlurSpriteInitData;
		yBlurSpriteInitData.m_fxFilePath = GAUSSIANBLUR_SHADER_FILEPATH;
		yBlurSpriteInitData.m_vsEntryPointFunc = YBLUR_VS_ENTRY_POINT_NAME;
		yBlurSpriteInitData.m_psEntryPoinFunc = GAUSSIANBLUR_PS_ENTRY_POINT_NAME;
		//�X�v���C�g�̉𑜓x�͏c�u���[�p�����_�����O�^�[�Q�b�g�Ɠ���
		yBlurSpriteInitData.m_width = m_yBlurRenderTarget.GetWidth();
		//���������l
		yBlurSpriteInitData.m_height = m_yBlurRenderTarget.GetHeight();
		//�e�N�X�`���͉��u���[���|��������
		yBlurSpriteInitData.m_textures[0] = &m_xBlurRenderTarget.GetRenderTargetTexture();
		//�������ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
		yBlurSpriteInitData.m_colorBufferFormat[0] = m_yBlurRenderTarget.GetColorBufferFormat();
		//�萔�o�b�t�@�Ƀu���[�p�p�����[�^��ݒ�
		//�u���[�̏d�݃e�[�u����ݒ�
		yBlurSpriteInitData.m_expandConstantBuffer = &m_weights;
		yBlurSpriteInitData.m_expandConstantBufferSize = sizeof(m_weights);

		//�������f�[�^����ɉ��u���[�p�X�v���C�g��������
		m_yBlurSprite.Init(yBlurSpriteInitData);
	}
}

void GaussianBlur::UpdateWeightsTable(float blurPower)
{
	//�d�݂̍��v���L�^����ϐ�
	float totalWeight = 0;

	//�K�E�X�֐���p���ďd�݃e�[�u���̊e�d�݂��v�Z
	//���[�v�ϐ�numFar�͊�e�N�Z������̋���
	for (int numFar = 0; numFar < NUM_WEIGHTS; numFar++) {
		m_weights[numFar] = expf(-0.5f * (float)(numFar * numFar) / blurPower);
		totalWeight += 2.0f * m_weights[numFar];
	}

	//�d�݂̍��v�ŏ��Z���A�d�݂̍��v�l��1�ɂ���
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		m_weights[i] /= totalWeight;
	}

}