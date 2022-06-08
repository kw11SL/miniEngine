#include "stdafx.h"
#include "RenderingEngine.h"

namespace {
	const int WINDOW_WIDTH = 1280;	//��
	const int WINDOW_HEIGHT = 720;	//����
}

RenderingEngine* RenderingEngine::m_renderingEngine = nullptr;

void RenderingEngine::Init()
{
	//���C�g�J�����̏�����
	InitLightCamera();
	//���C�������_�����O�^�[�Q�b�g�̏�����
	InitMainRenderTarget();
	//�t���[���o�b�t�@�ɓ\��t����X�v���C�g�̏�����
	InitFrameBufferSprite();
	//�V���h�E�}�b�v�̏�����
	InitShadowMap();
	//�|�X�g�G�t�F�N�g�̏�����
	InitPostEffect();
}

void RenderingEngine::InitPostEffect()
{
	//�u���[���̏�����
	InitBloom(m_mainRenderTarget);
}

void RenderingEngine::Execute(RenderContext& rc)
{
	///////////////////////////////////////////////////////////////
	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//�����_�����O�^�[�Q�b�g��ݒ�
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	//�����_�����O�^�[�Q�b�g�̃N���A
	rc.ClearRenderTargetView(m_mainRenderTarget);
	///////////////////////////////////////////////////////////////

	//���C�g�J�����̍X�V
	UpdateLightCamera();

	//�V���h�E�}�b�v�ւ̕`��
	RenderToShadowMap(rc, m_lightCamera);

	//�ʏ�`��
	CommonRendering(rc);

	//�G�t�F�N�g��`��
	EffectEngine::GetInstance()->Draw();

	//�u���[������
	BloomRendering(rc, m_mainRenderTarget);

	//�X�v���C�g��`��
	SpriteRendering(rc);

	//�t�H���g��`��
	FontRendering(rc);

	///////////////////////////////////////////////////////////////
	//���C�������_�����O�^�[�Q�b�g�ɕ`�悵�����̂��t���[���o�b�t�@�ɃR�s�[
	//�����_�����O�^�[�Q�b�g���I���X�N���[���ɖ߂�
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//�t���[���o�b�t�@�p�X�v���C�g��`��
	m_frameBufferSprite.Draw(rc);
	///////////////////////////////////////////////////////////////
}

void RenderingEngine::CommonRendering(RenderContext& rc)
{
	//���C�������_�����O�^�[�Q�b�g�����p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//�`�����ݐ�����C�������_�����O�^�[�Q�b�g�ɂ���B
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	////�����_�����O�^�[�Q�b�g���N���A
	//rc.ClearRenderTargetView(m_mainRenderTarget);

	//���f�����h���[
	for (auto& model : m_commonModels) {
		model->Draw(rc);
	}

	//�����_�����O�^�[�Q�b�g�̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}

void RenderingEngine::RenderToShadowMap(RenderContext& rc , Camera camera)
{
	//�V���h�E�}�b�v�ɕ`������
	m_shadowMap.Render(rc, camera);
}

void RenderingEngine::BloomRendering(RenderContext& rc, RenderTarget& mainRT)
{
	//�u���[������
	m_bloom.Render(rc, mainRT);
}

void RenderingEngine::SpriteRendering(RenderContext& rc)
{
	//���C�������_�����O�^�[�Q�b�g�����p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//�`�����ݐ�����C�������_�����O�^�[�Q�b�g�ɂ���B
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	//�����_�����O�^�[�Q�b�g���N���A
	//rc.ClearRenderTargetView(m_mainRenderTarget);

	for (auto& sprite : m_sprites) {
		//�X�v���C�g��`��
		sprite->Draw(rc);
	}

	//�����_�����O�^�[�Q�b�g�̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}

void RenderingEngine::FontRendering(RenderContext& rc)
{
	//���C�������_�����O�^�[�Q�b�g�����p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//�`�����ݐ�����C�������_�����O�^�[�Q�b�g�ɂ���B
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	//�����_�����O�^�[�Q�b�g���N���A
	//rc.ClearRenderTargetView(m_mainRenderTarget);

	for (auto& font : m_fontDataVector) {

		//�`��J�n
		font->font.Begin(rc);

		//�t�H���g��`��
		font->font.Draw(
			font->text,
			font->position,
			font->color,
			font->rotation,
			font->scale,
			font->pivot
		);

		//�`��I��
		font->font.End(rc);
	}

	//�����_�����O�^�[�Q�b�g�̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}


void RenderingEngine::InitLightCamera()
{
	m_lightCamera.SetPosition(0.0f, 2000.0f, 0.0f);
	m_lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	m_lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	m_lightCamera.SetNear(1.0f);
	m_lightCamera.SetFar(20000.0f);
	m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	m_lightCamera.SetViewAngle(Math::DegToRad(120.0f));
	m_lightCamera.Update();
}

void RenderingEngine::UpdateLightCamera()
{
	m_lightCamera.SetPosition(g_camera3D->GetPosition());
	//m_lightCamera.SetTarget(g_camera3D->GetTarget());
	m_lightCamera.SetTarget({ 0.0f,0.0f,0.0f });
	m_lightCamera.SetUp(g_camera3D->GetUp());
	m_lightCamera.Update();
}

void RenderingEngine::InitMainRenderTarget()
{
	m_mainRenderTarget.Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
}

void RenderingEngine::InitFrameBufferSprite()
{
	//�������f�[�^���쐬
	SpriteInitData spriteInitData;
	//�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g
	spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = WINDOW_WIDTH;
	spriteInitData.m_height = WINDOW_HEIGHT;
	//�V�F�[�_�[���w��
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//�\��t����X�v���C�g�̏�����
	m_frameBufferSprite.Init(spriteInitData);
}

void RenderingEngine::InitShadowMap()
{
	m_shadowMap.Init();
}

void RenderingEngine::InitBloom(RenderTarget& mainRT)
{
	m_bloom.Init(mainRT);
}

void RenderingEngine::DeleteCommonModel(Model* model)
{
	//�C�e���[�^���쐬
	std::vector<Model*>::iterator itr;
	
	//���f��������
	itr = std::find(
		m_commonModels.begin(),
		m_commonModels.end(),
		model
	);

	//���f��������������폜
	if (itr != m_commonModels.end()) {
		m_commonModels.erase(itr);
	}
}

void RenderingEngine::DeleteSprite(Sprite* sprite)
{
	//�C�e���[�^���쐬
	std::vector<Sprite*>::iterator itr;

	//�X�v���C�g������
	itr = std::find(
		m_sprites.begin(),
		m_sprites.end(),
		sprite
	);

	//�X�v���C�g������������폜
	if (itr != m_sprites.end()) {
		m_sprites.erase(itr);
	}
}

void RenderingEngine::DeleteFonts(SFontData& fontData)
{
	//�C�e���[�^���쐬
	std::vector<SFontData*>::iterator itr;

	//�t�H���g�f�[�^������
	itr = std::find(
		m_fontDataVector.begin(),
		m_fontDataVector.end(),
		&fontData
	);

	//�t�H���g�f�[�^������������폜
	if (itr != m_fontDataVector.end()) {
		m_fontDataVector.erase(itr);
	}
}

