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

	g_camera3D->SetFar(500000.0f);

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	//�������[���h�̃C���X�^���X���쐬
	PhysicsWorld::CreateInstance();
	//�T�E���h�G���W�����쐬
	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();
	//�Q�[���f�B���N�^�[���쐬
	GameDirector::CreateInstance();
	//�o���b�g�}�l�[�W�����쐬
	BulletManager::CreateInstance();
	//�G�l�~�[�}�l�[�W�����쐬
	EnemyManager::CreateInstance();

	//step-1 �G�t�F�N�g�G���W���̃C���X�^���X���쐬����B
	EffectEngine::CreateInstance();
	//�e�X�g�F�����_�����O�G���W��
	RenderingEngine::CreateInstance();
	//�����_�����O�G���W���̏�����
	RenderingEngine::GetInstance()->Init();

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

	//�^�C�g���̍쐬
	NewGO<Title>(0, "title");

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{	
		//�e�X�g�F�Q�[���̍폜
		if(g_pad[0]->IsTrigger(enButtonStart)){
			QueryGOs<Game>("game", [&](Game* gameScene) {
				
				//�Q�[�����폜
				DeleteGO(gameScene);
				//�Q�[����������Ԃɖ߂�
				GameDirector::GetInstance()->ResetGame();
				//�o���N���t�@�C��������
				//g_engine->ClearFileBank();

				//�^�C�g����NewGO
				NewGO<Title>(0, "title");

				//�₢���킹�I��
				return false;
			});

			//�o���b�g�}�l�[�W�������炷�ׂĂ̒e������
			BulletManager::GetInstance()->DeleteBullets();
		}

		//// �e�X�g�F�|�[�Y��Ԃ̐؂�ւ�
		//if (g_pad[0]->IsTrigger(enButtonSelect)) {
		//	if (GameDirector::GetInstance()->GetGameState() == enGame) {
		//		GameDirector::GetInstance()->SetGameState(enPause);
		//	}
		//	else if(GameDirector::GetInstance()->GetGameState() == enPause) {
		//		GameDirector::GetInstance()->SetGameState(enGame);
		//	}
		//}

		////�e�X�g�F�Q�[���J�n����Q�[�����ւ̕ύX
		//if (g_pad[0]->IsTrigger(enButtonA)) {
		//	if (GameDirector::GetInstance()->GetGameState() == enStart) {
		//		GameDirector::GetInstance()->SetGameState(enGame);
		//	}
		//}

		//�����_�����O�J�n�B
		g_engine->BeginFrame();

		//�o�^����Ă���Q�[���I�u�W�F�N�g�̍X�V�֐����Ăяo���B
		GameObjectManager::GetInstance()->ExecuteUpdate();

		//�Q�[���f�B���N�^�[�̍X�V����
		GameDirector::GetInstance()->ExecuteUpdate();

		//�o���b�g�}�l�[�W���̍X�V����
		BulletManager::GetInstance()->ExecuteUpdate();

		//�������[���h�̍X�V�B
		PhysicsWorld::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

		//step-5 �G�t�F�N�g�G���W���̍X�V�B
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());
		
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

		//�e�X�g�F�����_�����O�G���W���̏���
		RenderingEngine::GetInstance()->Execute(renderContext);

		////step-6 �G�t�F�N�g�̃h���[�B
		//EffectEngine::GetInstance()->Draw();

		//���C�������_�����O�^�[�Q�b�g�ɕ`�悵�����̂��t���[���o�b�t�@�ɃR�s�[
		//�����_�����O�^�[�Q�b�g���I���X�N���[���ɖ߂�
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()		
		);
		frameBufferSprite.Draw(renderContext);

		

		g_engine->EndFrame();
	}

	////////////////////////////////////////////////////
	//�}�l�[�W���A�G���W���̍폜
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	//�����_�����O�G���W�����폜
	RenderingEngine::DeleteInstance();
	//�T�E���h�G���W�����폜
	CSoundEngine::DeleteInstance();
	//�Q�[���f�B���N�^�[���폜
	GameDirector::DeleteInstance();
	//�o���b�g�}�l�[�W�����폜
	BulletManager::DeleteInstance();
	//�G�l�~�[�}�l�[�W�����폜
	EnemyManager::DeleteInstance();
	////////////////////////////////////////////////////

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

