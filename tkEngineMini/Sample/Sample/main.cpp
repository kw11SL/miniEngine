#include "stdafx.h"
#include "system/system.h"
#include "Player.h"
#include "Background.h"
#include "HelloFont.h"
#include "Hello2D.h"
#include "HelloLevel2D.h"
#include "PhysicsBall.h"
#include "Seesaw.h"
#include "effect/effect.h"
#include "DirectionLight.h"
#include "Game.h"
#include "Player_new.h"
#include "SkinModelRender.h"




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
	
	//�J�����̐ݒ�
	g_camera3D->SetPosition({ 0.0f,50.0f,200.0f });
	g_camera3D->SetTarget({ 0.0f,50.0f,0.0f });
	float cameraH = 50.0f;

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//step-1 �G�t�F�N�g�G���W���̃C���X�^���X���쐬����B
	EffectEngine::CreateInstance();

	
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

		//�o�^����Ă���Q�[���I�u�W�F�N�g�̕`��֐����Ăяo���B
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//step-6 �G�t�F�N�g�̃h���[�B
		EffectEngine::GetInstance()->Draw();

		g_engine->EndFrame();
	}

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

