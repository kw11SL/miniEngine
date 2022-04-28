#pragma once
#define _CRT_SECURE_NO_WARNINGS

//#define DEBUG_ON			//�f�o�b�O�t���O

#include "MiniEngine.h"
#include "ExEngine.h"
#include "RenderingEngine.h"
#include "effect/Effect.h"
#include "SkinModelRender.h"
#include "SpriteRender.h"
#include "FontRender.h"
#include "Game.h"
#include "BG.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "GameCamera.h"
#include "MyCharacterController.h"
#include "SphericalMove.h"
#include "Bullet.h"
#include "Explosion.h"
#include "GameDirector.h"
#include "BulletManager.h"
#include "ExplosionManager.h"
#include "EnemyManager.h"
#include "Title.h"
#include "SoundSource.h"
#include "SoundEngine.h"


namespace {
	//�V���h�E�}�b�v����̒萔
	//�V���h�E�}�b�v�̐�
	const int NUM_SHADOW_MAP = 3;

	//���ʂŎg�����O
	const char* TITLE_NAME = "title";							//�^�C�g��								
	const char* GAME_SCENE_NAME = "game";						//�Q�[���V�[��
	const char* DIRECTION_LIGHT_NAME = "directionLight";		//�f�B���N�V�������C�g
	const char* POINT_LIGHT_NAME = "pointLight";				//�|�C���g���C�g
	const char* SPOT_LIGHT_NAME = "spotLight";					//�X�|�b�g���C�g
	const char* PLAYER_NAME = "player";							//�v���C���[
	const char* ENEMY_NAME = "enemy";							//�G�l�~�[
	const char* SKYCUBE_NAME = "skyCube";						//�X�J�C�L���[�u
	const char* UI_NAME = "ui";									//UI
	const char* STAGE_NAME = "stage";							//�X�e�[�W
	const char* STAGE_BACK_GROUND_NAME = "stageBackGround";		//�w�i
	const char* BULLET_PLAYER_NAME = "bullet";					//�e(�v���C���[)
	const char* BULLET_ENEMY_NAME = "enemyBullet";				//�e(�G)
	const char* EXPLOSION_PLAYER_NAME = "explosion";			//����(�v���C���[)
	const char* EXPLOSION_ENEMY_NAME = "enemyExplosion";		//����(�G�l�~�[)
}