#include "stdafx.h"
#include "system/system.h"

namespace
{
	const int WINDOW_WIDTH = 1280;	//��
	const int WINDOW_HEIGHT = 720;	//����

	const char* POSTEFFECT_FILEPATH = "Assets/shader/PostEffect.fx";
	const char* GAUSSIAN_BLUR_EFFECT_FILEPATH = "Assets/shader/GaussianBlur.fx";
}

//�d�݃e�[�u���̐ݒ�
const int NUM_WEIGHTS = 8;
struct SBlurParam
{
	float weights[NUM_WEIGHTS];
};

//�֐��錾
void InitRootSignature(RootSignature& rs);

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////
	
	RootSignature rs;
	InitRootSignature(rs);


	//�J�����̐ݒ�
	g_camera3D->SetPosition({ 0.0f,50.0f,200.0f });
	g_camera3D->SetTarget({ 0.0f,50.0f,0.0f });
	float cameraH = 50.0f;

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//step-1 �G�t�F�N�g�G���W���̃C���X�^���X���쐬����B
	EffectEngine::CreateInstance();


	////�u���[�������e�X�g

	////���C�������_�����O�^�[�Q�b�g�̐ݒ�
	////�J���[�o�b�t�@�̃t�H�[�}�b�g��32bit���������_
	//RenderTarget mainRenderTarget;
	//mainRenderTarget.Create(
	//	WINDOW_WIDTH,
	//	WINDOW_HEIGHT,
	//	1,
	//	1,
	//	DXGI_FORMAT_R32G32B32A32_FLOAT,
	//	DXGI_FORMAT_D32_FLOAT
	//);

	////�P�x���o�p�����_�����O�^�[�Q�b�g�̐ݒ�
	//RenderTarget luminanceRenderTarget;
	//luminanceRenderTarget.Create(
	//	WINDOW_WIDTH,
	//	WINDOW_HEIGHT,
	//	1,
	//	1,
	//	DXGI_FORMAT_R32G32B32A32_FLOAT,
	//	DXGI_FORMAT_D32_FLOAT
	//);
	////�P�x���o�p�X�v���C�g��������
	//SpriteInitData luminanceSpriteInitData;

	//luminanceSpriteInitData.m_fxFilePath = POSTEFFECT_FILEPATH;
	//luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
	//luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
	//luminanceSpriteInitData.m_width = WINDOW_WIDTH;
	//luminanceSpriteInitData.m_height = WINDOW_HEIGHT;
	////�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g
	//luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	////�������ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w��
	//luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	////�X�v���C�g��������
	//Sprite luminanceSprite;
	//luminanceSprite.Init(luminanceSpriteInitData);

	////�K�E�V�A���u���[�̏�����
	//GaussianBlur gaussBlur;
	////�P�x���o�p�����_�����O�^�[�Q�b�g�̃e�N�X�`���ŏ�����(�X�v���C�g�̕��ł͂Ȃ�)
	//gaussBlur.Init(&luminanceRenderTarget.GetRenderTargetTexture());

	////�u���[�摜�����Z�������邽�߂̃X�v���C�g��������
	//SpriteInitData addBrendSpriteInitData;
	////�e�N�X�`���̓u���[�����������P�x���o�e�N�X�`��
	//addBrendSpriteInitData.m_textures[0] = &gaussBlur.GetExecutedTexture();
	//addBrendSpriteInitData.m_width = WINDOW_WIDTH;
	//addBrendSpriteInitData.m_height = WINDOW_HEIGHT;
	////�ʏ�̃V�F�[�_���w��
	//addBrendSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	////�A���t�@�u�����h���[�h�����Z�����ɂ���
	//addBrendSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//addBrendSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	////���Z�����X�v���C�g�̏�����
	//Sprite addBrendSprite;
	//addBrendSprite.Init(addBrendSpriteInitData);

	////�e�N�X�`����\����邽�߂̃X�v���C�g��������
	//SpriteInitData spriteInitData;
	////�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@
	//spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//spriteInitData.m_width = WINDOW_WIDTH;
	//spriteInitData.m_height = WINDOW_HEIGHT;
	////�ʏ�̃V�F�[�_���w��
	//spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	////�\��t����X�v���C�g�̏�����
	//Sprite frameBufferSprite;
	//frameBufferSprite.Init(spriteInitData);

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//�Q�[���V�[�����쐬
	NewGO<Game>(0, "game");

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();

		//�e�X�g�F�J�����㉺
		if (g_pad[0]->IsPress(enButtonSelect)) {
			cameraH -= 2.0f;
		}
		else if (g_pad[0]->IsPress(enButtonStart)) {
			cameraH += 2.0f;
		}

		g_camera3D->SetPosition({ 0.0f,cameraH,200.0f });
		
		//�o�^����Ă���Q�[���I�u�W�F�N�g�̍X�V�֐����Ăяo���B
		GameObjectManager::GetInstance()->ExecuteUpdate();

		//�������[���h�̍X�V�B
		PhysicsWorld::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

		//step-5 �G�t�F�N�g�G���W���̍X�V�B
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

		////�u���[���̏����e�X�g
		//
		////�����_�����O�^�[�Q�b�g�����C�������_�����O�^�[�Q�b�g�ɕύX(=�I�t�X�N���[�������_�����O�ɂ���)
		////�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		//renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		////�����_�����O�^�[�Q�b�g��ݒ�
		//renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		////�����_�����O�^�[�Q�b�g�̃N���A
		//renderContext.ClearRenderTargetView(mainRenderTarget);

		////////���f���̃h���[
		//�o�^����Ă���Q�[���I�u�W�F�N�g�̕`��֐����Ăяo���B
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		////���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		//renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		////�P�x���o����
		////�P�x���o�p�����_�����O�^�[�Q�b�g�ɕύX
		//renderContext.WaitUntilToPossibleSetRenderTarget(luminanceRenderTarget);
		////�����_�����O�^�[�Q�b�g��ݒ�
		//renderContext.SetRenderTargetAndViewport(luminanceRenderTarget);
		////�����_�����O�^�[�Q�b�g���N���A
		//renderContext.ClearRenderTargetView(luminanceRenderTarget);
		////�P�x���o���s��
		//luminanceSprite.Draw(renderContext);
		////�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		//renderContext.WaitUntilFinishDrawingToRenderTarget(luminanceRenderTarget);

		////�K�E�V�A���u���[�̎��s
		//gaussBlur.ExecuteOnGPU(renderContext, 20);

		////�u���[�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
		////�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		//renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		////�����_�����O�^�[�Q�b�g��ݒ�
		//renderContext.SetRenderTargetAndViewport(mainRenderTarget);
		////����(=�X�v���C�g�̕`��)
		//addBrendSprite.Draw(renderContext);
		////�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		//renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		////���C�������_�����O�^�[�Q�b�g�ɕ`�悵�����̂��t���[���o�b�t�@�ɃR�s�[
		////�����_�����O�^�[�Q�b�g���I���X�N���[���ɖ߂�
		//renderContext.SetRenderTarget(
		//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()		
		//);
		//frameBufferSprite.Draw(renderContext);

		//step-6 �G�t�F�N�g�̃h���[�B
		EffectEngine::GetInstance()->Draw();

		g_engine->EndFrame();
	}

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

//���[�g�V�O�l�`���̏�����
void InitRootSignature(RootSignature& rs)
{
	rs.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);
}

