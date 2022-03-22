#pragma once
#define _CRT_SECURE_NO_WARNINGS

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
#include "EnemyManager.h"
#include "Title.h"
#include "SoundSource.h"
#include "SoundEngine.h"




//シャドウマップ周りの定数
namespace {

	//シャドウマップの数
	const int NUM_SHADOW_MAP = 3;

	//共通で使う名前
	const char* TITLE_NAME = "title";
	const char* GAME_SCENE_NAME = "game";
	const char* DIRECTION_LIGHT_NAME = "directionLight";
	const char* POINT_LIGHT_NAME = "pointLight";
	const char* SPOT_LIGHT_NAME = "spotLight";
	const char* PLAYER_NAME = "player";
	const char* ENEMY_NAME = "enemy";
	const char* SKYCUBE_NAME = "skyCube";
	const char* UI_NAME = "ui";
	const char* STAGE_NAME = "stage";
	const char* STAGE_BACK_GROUND_NAME = "stageBackGround";
	const char* EXPLOSION_PLAYER_NAME = "explosion";
	const char* EXPLOSION_ENEMY_NAME = "enemyExplosion";

}