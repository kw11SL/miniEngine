#include "stdafx.h"
#include "system/system.h"
#include "Bloom.h"
#include "RenderingEngine.h"

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

	//�e�X�g�F�����_�����O�G���W��
	RenderingEngine::CreateInstance();
	//RenderingEngine renderingEngine;
	//�����_�����O�G���W���̏�����
	RenderingEngine::GetInstance()->Init();
	//renderingEngine.Init();

	//�e�N�X�`����\����邽�߂̃X�v���C�g��������
	SpriteInitData spriteInitData;
	//�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@
	spriteInitData.m_textures[0] = &RenderingEngine::GetInstance()->GetRenderTarget().GetRenderTargetTexture();
	spriteInitData.m_width = WINDOW_WIDTH;
	spriteInitData.m_height = WINDOW_HEIGHT;
	//�ʏ�̃V�F�[�_���w��
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

	//�\��t����X�v���C�g�̏�����
	Sprite frameBufferSprite;
	frameBufferSprite.Init(spriteInitData);

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//�Q�[���V�[�����쐬
	Game* game = nullptr;
	game = NewGO<Game>(0, "game");
	game->Init(*RenderingEngine::GetInstance());

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


		//�u���[���̏����e�X�g
		
		//�����_�����O�^�[�Q�b�g�����C�������_�����O�^�[�Q�b�g�ɕύX(=�I�t�X�N���[�������_�����O�ɂ���)
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		renderContext.WaitUntilToPossibleSetRenderTarget(RenderingEngine::GetInstance()->GetRenderTarget());
		//�����_�����O�^�[�Q�b�g��ݒ�
		renderContext.SetRenderTargetAndViewport(RenderingEngine::GetInstance()->GetRenderTarget());
		//�����_�����O�^�[�Q�b�g�̃N���A
		renderContext.ClearRenderTargetView(RenderingEngine::GetInstance()->GetRenderTarget());
		
		////////���f���̃h���[////////
		//�o�^����Ă���Q�[���I�u�W�F�N�g�̕`��֐����Ăяo���B
		//GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		//////////////////////////////
		
		//�e�X�g�F�����_�����O�G���W���ɂ��ʏ�`��
		RenderingEngine::GetInstance()->CommonRender(renderContext);
		
		//�e�X�g�F�����_�����O�G���W���̏���
		RenderingEngine::GetInstance()->Execute(renderContext);

		//���C�������_�����O�^�[�Q�b�g�ɕ`�悵�����̂��t���[���o�b�t�@�ɃR�s�[
		//�����_�����O�^�[�Q�b�g���I���X�N���[���ɖ߂�
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()		
		);
		frameBufferSprite.Draw(renderContext);

		//step-6 �G�t�F�N�g�̃h���[�B
		EffectEngine::GetInstance()->Draw();

		g_engine->EndFrame();
	}

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	//�Q�[�����폜
	DeleteGO(game);

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

