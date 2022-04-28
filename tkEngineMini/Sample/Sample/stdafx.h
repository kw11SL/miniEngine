#pragma once
#define _CRT_SECURE_NO_WARNINGS

//#define DEBUG_ON			//デバッグフラグ

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
	//シャドウマップ周りの定数
	//シャドウマップの数
	const int NUM_SHADOW_MAP = 3;

	//共通で使う名前
	const char* TITLE_NAME = "title";							//タイトル								
	const char* GAME_SCENE_NAME = "game";						//ゲームシーン
	const char* DIRECTION_LIGHT_NAME = "directionLight";		//ディレクションライト
	const char* POINT_LIGHT_NAME = "pointLight";				//ポイントライト
	const char* SPOT_LIGHT_NAME = "spotLight";					//スポットライト
	const char* PLAYER_NAME = "player";							//プレイヤー
	const char* ENEMY_NAME = "enemy";							//エネミー
	const char* SKYCUBE_NAME = "skyCube";						//スカイキューブ
	const char* UI_NAME = "ui";									//UI
	const char* STAGE_NAME = "stage";							//ステージ
	const char* STAGE_BACK_GROUND_NAME = "stageBackGround";		//背景
	const char* BULLET_PLAYER_NAME = "bullet";					//弾(プレイヤー)
	const char* BULLET_ENEMY_NAME = "enemyBullet";				//弾(敵)
	const char* EXPLOSION_PLAYER_NAME = "explosion";			//爆発(プレイヤー)
	const char* EXPLOSION_ENEMY_NAME = "enemyExplosion";		//爆発(エネミー)
}