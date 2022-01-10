#include "stdafx.h"
#include "Bloom.h"

namespace
{
	const int WINDOW_WIDTH = 1280;	//��
	const int WINDOW_HEIGHT = 720;	//����

	const char* POSTEFFECT_FILEPATH = "Assets/shader/PostEffect.fx";
	const char* GAUSSIAN_BLUR_EFFECT_FILEPATH = "Assets/shader/GaussianBlur.fx";
}

void Bloom::Init(RenderTarget& mainRenderTarget)
{
	//�P�x���o�p�����_�����O�^�[�Q�b�g�̐ݒ�
	m_luminanceRenderTarget.Create(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

	//�P�x���o�p�X�v���C�g��������
	SpriteInitData luminanceSpriteInitData;
	luminanceSpriteInitData.m_fxFilePath = POSTEFFECT_FILEPATH;
	luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	luminanceSpriteInitData.m_width = mainRenderTarget.GetWidth();
	luminanceSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g
	luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//�������ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w��
	luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//�X�v���C�g��������
	m_luminanceSprite.Init(luminanceSpriteInitData);

	//�P�x���o�p�����_�����O�^�[�Q�b�g�̃e�N�X�`���ŏ�����(�X�v���C�g�̕��ł͂Ȃ�)
	m_gaussBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
	//�u���[���|�����e�N�X�`���Ƀu���[���|���Ă���
	m_gaussBlur[1].Init(&m_gaussBlur[0].GetExecutedTexture());
	m_gaussBlur[2].Init(&m_gaussBlur[1].GetExecutedTexture());
	m_gaussBlur[3].Init(&m_gaussBlur[2].GetExecutedTexture());

	//�u���[�摜�����Z�������邽�߂̃X�v���C�g��������
	SpriteInitData addBrendSpriteInitData;
	//�e�N�X�`���̓u���[�����������P�x���o�e�N�X�`��
	addBrendSpriteInitData.m_textures[0] = &m_gaussBlur[0].GetExecutedTexture();
	addBrendSpriteInitData.m_textures[1] = &m_gaussBlur[1].GetExecutedTexture();
	addBrendSpriteInitData.m_textures[2] = &m_gaussBlur[2].GetExecutedTexture();
	addBrendSpriteInitData.m_textures[3] = &m_gaussBlur[3].GetExecutedTexture();

	//�𑜓x�̓����_�����O�^�[�Q�b�g�Ɠ���
	addBrendSpriteInitData.m_width = mainRenderTarget.GetWidth();
	addBrendSpriteInitData.m_height = mainRenderTarget.GetHeight();

	//�u���[�摜���������邽�߂ɃV�F�[�_���p�̂��̂ɕύX
	addBrendSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
	addBrendSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//�u���[���p�̃G���g���[�|�C���g���w��
	addBrendSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";
	//�A���t�@�u�����h���[�h�����Z�����ɂ���
	addBrendSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	addBrendSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	
	m_finalSprite.Init(addBrendSpriteInitData);

}

void Bloom::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//�P�x���o����
	//�P�x���o�p�����_�����O�^�[�Q�b�g�ɕύX
	rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
	//�����_�����O�^�[�Q�b�g��ݒ�
	rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
	//�����_�����O�^�[�Q�b�g���N���A
	rc.ClearRenderTargetView(m_luminanceRenderTarget);
	//�P�x���o���s��
	m_luminanceSprite.Draw(rc);
	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

	//�K�E�V�A���u���[�𕡐�����s����
	m_gaussBlur[0].ExecuteOnGPU(rc, 20.0f);
	m_gaussBlur[1].ExecuteOnGPU(rc, 20.0f);
	m_gaussBlur[2].ExecuteOnGPU(rc, 20.0f);
	m_gaussBlur[3].ExecuteOnGPU(rc, 20.0f);

	//�u���[�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//�����_�����O�^�[�Q�b�g��ݒ�
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	//����(=�X�v���C�g�̕`��)
	m_finalSprite.Draw(rc);
	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

}