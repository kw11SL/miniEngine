#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MiniEngine.h"
#include "ExEngine.h"
#include "RenderingEngine.h"
#include "effect/Effect.h"
#include "SkinModelRender.h"
#include "SpriteRender.h"
//#include "GaussianBlur.h"
#include "Game.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"



//シャドウマップ周りの定数
namespace {

	//シャドウマップの数
	const int NUM_SHADOW_MAP = 3;

}