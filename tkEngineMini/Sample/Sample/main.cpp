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
	////�f�B���N�V�������C�g���`
	//DirectionLight directionLight;

	////�f�B���N�V�������C�g�̏�����
	//directionLight.Init({ 1.0f,0.0f,0.0f }, { 0.5f, 0.5f, 0.5f }, { 0.5f,0.5f,0.5f });
	////�J�����ʒu���擾��
	//Vector3 cameraPos = g_camera3D->GetPosition();
	////�����o�ɔ��f
	//directionLight.SetEyePos(cameraPos);

	//float cameraPosH = 0.0f;
	//float cameraPosV = 0.0f;


	//�J�����̐ݒ�
	g_camera3D->SetPosition({ 0.0f,200.0f,200.0f });
	g_camera3D->SetTarget({ 0.0f,50.0f,0.0f });

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//step-1 �G�t�F�N�g�G���W���̃C���X�^���X���쐬����B
	EffectEngine::CreateInstance();
	
	

	////���f���̏����������쐬
	//ModelInitData initData;
	////�t�@�C���p�X�̎w��
	//initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	////�V�F�[�_�[�̃t�@�C���p�X���w��
	//initData.m_fxFilePath = "Assets/shader/model.fx";
	////�f�B���N�V�������C�g�̏���萔�o�b�t�@�Ƃ���
	////�f�B�X�N���v�^�q�[�v�ɓo�^���邽�߁A���������Ƃ��ēn��
	//initData.m_expandConstantBuffer = directionLight.GetDirLightAddress();
	//initData.m_expandConstantBufferSize = sizeof(directionLight.GetDirLight());
	
	////�L�����N�^�[�̃��f�������̏��������ŏ�����
	//Model charaModel;
	//charaModel.Init(initData);
	
	/*Player_new* player = nullptr;
	player = NewGO<Player_new>(0, "pl");*/
	//player->InitDirectionLight(directionLight);
	//player->SetPostion({ 0.0f,0.0f,0.0f });

	/*SkinModelRender* m_skinModelRender = nullptr;
	m_skinModelRender = NewGO<SkinModelRender>(0, "player");
	
	m_skinModelRender->Init("Assets/modelData/unityChan.tkm", enModelUpAxisZ);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain");
	m_skinModelRender->InitDirectionLight(&directionLight);*/

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//step-2 ���[�U�[�G�t�F�N�g�̏������B
	Effect laserEffect;
	laserEffect.Init(u"Assets/effect/kick.efk");

	Effect laserEffect2;
	laserEffect2.Init(u"Assets/effect/laser2.efk");

	//�Q�[���V�[�����쐬
	NewGO<Game>(0, "game");

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		/*float stickLX = g_pad[0]->GetLStickXF();
		float stickLY = g_pad[0]->GetLStickYF();

		cameraPosH += stickLX;
		cameraPosV += stickLY;
		
		Vector3 cameraPos = Vector3::Zero;
		cameraPos.Set(cameraPosH, 0.0f, cameraPosV);

		g_camera3D->SetPosition({cameraPosH,0.0f,cameraPosV});*/


		////�f�B���N�V�������C�g�̑���
		////�E�X�e�B�b�N�̐��������A���������̓��͒l���󂯎��
		//float angleH = g_pad[0]->GetRStickXF();
		//float angleV = g_pad[0]->GetRStickYF();



		//Vector3 dirLigDirection = directionLight.GetDirection();
		//Quaternion rotH;
		//Quaternion rotV;

		//
		//rotH.SetRotationDeg(Vector3::AxisY, angleH);
		//rotH.Apply(dirLigDirection);
		//rotV.SetRotationDeg(Vector3::AxisX, angleV);
		//rotV.Apply(dirLigDirection);

		//////���ŕύX�����x�N�g���Ɖ�]�𔽉f
		//directionLight.SetDirection(dirLigDirection);

		//�G�t�F�N�g�̔���
		if (g_pad[0]->IsTrigger(enButtonA)) {
			//�Đ��J�n�B
			laserEffect.Play();
		}
		if (g_pad[0]->IsTrigger(enButtonB)) {
			//�Đ��J�n�B
			laserEffect2.Play();
		}
		
		//step-3 �G�t�F�N�g�𓮂����B	
		auto pos = laserEffect.GetPosition();
		pos.x += g_pad[0]->GetLStickXF();
		pos.z += g_pad[0]->GetLStickYF();

		auto rot = laserEffect.GetRotation();
		rot.AddRotationY(g_pad[0]->GetRStickXF() * 0.1f);

		laserEffect.SetPosition(pos);
		laserEffect.SetRotation(rot);

		pos = laserEffect2.GetPosition();
		pos.x += g_pad[0]->IsPress(enButtonLeft);
		pos.x -= g_pad[0]->IsPress(enButtonRight);
		laserEffect2.SetPosition(pos);


		//step-4 �G�t�F�N�g�̃��[���h�s����X�V����B
		laserEffect.Update();
		laserEffect2.Update();

		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		
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

		////�L�������f���̃h���[�R�[�������s
		//charaModel.Draw(renderContext);

		g_engine->EndFrame();
	}

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

