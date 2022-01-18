#include "stdafx.h"
#include "Life.h"

namespace{
	//ライフアイコンの数(for文回す用)
	const int LIFE_ICON_NUM = 3;

	//ファイルパス
	const char* LIFE_TEXT_SPRITE_FILEPATH = "Assets/sprite/ui/life.dds";						//LIFEアイコン画像のファイルパス
	const char* LIFE_ICON_SPRITE_FILEPATH = "Assets/sprite/ui/lifeIcon_noneFrame.dds";			//LIFEアイコンの枠画像のファイルパス
	const char* LIFE_ICON_HALO_SPRITE_FILEPATH = "Assets/sprite/ui/lifeIconHalo.dds";			//発光画像のファイルパス
	const char* LIFE_ICON_FRAME_FILEPATH = "Assets/sprite/ui/lifeIcon_frame.dds";				//枠スプライトのファイルパス

	//位置、拡大率、カラー、ピボット
	const Vector3 LIFE_TEXT_POS = { -600.0f,120.0f,0.0f };										//LIFEの字のスプライト位置
	const Vector3 LIFE_TEXT_SHADOW_OFFSET = { 7.0f,-7.0f,0.0f };
	
	const Vector3 LIFE_TEXT_SCALE = { 0.6f,0.6f,1.0f };											//LIFEの字のスプライトの拡大率
	const Vector3 LIFE_ICON_SCALE = { 0.2f,0.2f,1.0f };											//アイコンのスプライトの拡大率
	const Vector3 LIFE_ICON_FRAME_SCALE = { 0.2f,0.2f,1.0f };									//枠スプライトの拡大率
	const Vector3 LIFE_ICON_HALO_SCALE = { 1.0f,1.0f,1.0f };									//発光スプライトの拡大率

	const Vector4 LIFE_TEXT_COLOR = { 0.9f * 1.5f,0.25f * 1.5f,0.25f * 1.5f,1.0f };				//LIFEの字のスプライトの色
	const Vector4 LIFE_TEXT_SHADOW_COLOR = { 0.9f * 0.3f,0.25f * 0.3f,0.25f * 0.3f,1.0f * 0.7f };
	const Vector4 LIFE_ICON_COLOR = { 0.95f,0.95f,0.95f,1.0f };									//アイコンの色
	const Vector4 LIFE_ICON_FRAME_COLOR = { 1.0f,1.0f,1.0f,1.0f };								//枠スプライトの色
	const Vector4 LIFE_ICON_HALO_COLOR = { 0.9f,0.3f,0.2f,1.0f };								//発光の色

	const Vector3 LIFE_TEXT_TO_LIFE_ICON_0 = { 80.0f,40.0f,0.0f };								//LIFEの字からライフアイコン1への相対位置
	const Vector3 LIFE_ICON_0_TO_LIFE_ICON_1 = { 5.0f,-40.0f,0.0f };							//ライフアイコン1からライフアイコン2への相対位置
	const Vector3 LIFE_ICON_1_TO_LIFE_ICON_2 = { -5.0f,-40.0f,0.0f };							//ライフアイコン2からライフアイコン3への相対位置

	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };													//スプライトのピボット

	//幅、高さ
	const int LIFE_TEXT_WIDTH = 128;															//LIFEの字の幅
	const int LIFE_TEXT_HEIGHT = 256;															//FIFEの字の高さ
	const int LIFE_ICON_WIDTH = 256;															//ライフアイコンの幅
	const int LIFE_ICON_HEIGHT = 256;															//ライフアイコンの高さ
	const int LIFE_ICON_FRAME_WIDTH = 256;														//枠スプライトの幅
	const int LIFE_ICON_FRAME_HEIGHT = 256;														//枠スプライトの高さ
	const int LIFE_ICON_HALO_WIDTH = 128;														//発光の高さ
	const int LIFE_ICON_HALO_HEIGHT = 128;														//発光の幅

	//発光関係
	const float X_SCALE_RATE = 0.1f;															//横に延びる量
	const float Y_SCALE_RATE = 0.03f;															//縦に縮む量
}

Life::Life()
{
	m_prevPlayerLife = LIFE_ICON_NUM;

	int iconNum = 0;
	//ライフアイコン、枠スプライトの初期化
	for (; iconNum < LIFE_ICON_NUM; iconNum++) {
		m_lifeIconSpritePos[iconNum] = Vector3::Zero;
		m_lifeIconFramePos[iconNum] = Vector3::Zero;
	}
}

Life::~Life()
{
	int iconNum = 0;
	for (; iconNum < LIFE_ICON_NUM; iconNum++){
		DeleteGO(m_lifeIconSprite[iconNum]);
		DeleteGO(m_lifeIconFrame[iconNum]);
	}

	DeleteGO(m_lifeIconHalo);
	DeleteGO(m_lifeTextSprite);
	DeleteGO(m_lifeTextShadowSprite);
}

void Life::Init()
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	//文字の影スプライトの初期化。先にしておく。
	m_lifeTextShadowSprite = NewGO<SpriteRender>(0);
	m_lifeTextShadowSprite->Init(
		LIFE_TEXT_SPRITE_FILEPATH,
		LIFE_TEXT_WIDTH,
		LIFE_TEXT_HEIGHT,
		AlphaBlendMode_Trans
	);
	
	//文字スプライトの初期化
	m_lifeTextSprite = NewGO<SpriteRender>(0);
	m_lifeTextSprite->Init(
		LIFE_TEXT_SPRITE_FILEPATH,
		LIFE_TEXT_WIDTH,
		LIFE_TEXT_HEIGHT,
		AlphaBlendMode_Trans
	);

	//文字スプライトの位置、拡大率、色、ピボットを設定
	m_lifeTextSpritePos = LIFE_TEXT_POS;
	m_lifeTextSprite->SetPosition(m_lifeTextSpritePos);
	m_lifeTextSprite->SetScale(LIFE_TEXT_SCALE);
	m_lifeTextSprite->SetColor(LIFE_TEXT_COLOR);
	m_lifeTextSprite->SetPivot(SPRITE_PIVOT);

	//影スプライトのパラメータを設定
	//少しずらす
	m_lifeTextShadowSprite->SetPosition(m_lifeTextSpritePos + LIFE_TEXT_SHADOW_OFFSET);
	m_lifeTextShadowSprite->SetScale(LIFE_TEXT_SCALE);
	m_lifeTextShadowSprite->SetColor(LIFE_TEXT_SHADOW_COLOR);
	m_lifeTextShadowSprite->SetPivot(SPRITE_PIVOT);

	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	//アイコンスプライト,アイコン枠スプライトの初期化
	//位置を先に決めておく
	m_lifeIconSpritePos[0] = m_lifeTextSpritePos;
	m_lifeIconSpritePos[0] += LIFE_TEXT_TO_LIFE_ICON_0;

	m_lifeIconSpritePos[1] = m_lifeIconSpritePos[0];
	m_lifeIconSpritePos[1] += LIFE_ICON_0_TO_LIFE_ICON_1;

	m_lifeIconSpritePos[2] = m_lifeIconSpritePos[1];
	m_lifeIconSpritePos[2] += LIFE_ICON_1_TO_LIFE_ICON_2;

	//枠スプライトの位置をアイコンと同じ位置にする
	int iconFrameNum = 0;
	for (; iconFrameNum < LIFE_ICON_NUM; iconFrameNum++) {
		m_lifeIconFramePos[iconFrameNum] = m_lifeIconSpritePos[iconFrameNum];
	}

	//アイコンスプライト,枠スプライトをそれぞれNewGOし、初期化
	int iconNum = 0;
	for (; iconNum < LIFE_ICON_NUM; iconNum++) {
		m_lifeIconSprite[iconNum] = NewGO<SpriteRender>(0);
		m_lifeIconFrame[iconNum] = NewGO<SpriteRender>(0);

		//アイコンの初期化
		m_lifeIconSprite[iconNum]->Init(
			LIFE_ICON_SPRITE_FILEPATH,
			LIFE_ICON_WIDTH,
			LIFE_ICON_HEIGHT,
			AlphaBlendMode_Trans
		);

		//枠スプライトの初期化
		m_lifeIconFrame[iconNum]->Init(
			LIFE_ICON_FRAME_FILEPATH,
			LIFE_ICON_FRAME_WIDTH,
			LIFE_ICON_FRAME_HEIGHT,
			AlphaBlendMode_Trans
		);

		//アイコン、枠スプライトをあらかじめ決めた位置に設定
		m_lifeIconSprite[iconNum]->SetPosition(m_lifeIconSpritePos[iconNum]);
		m_lifeIconFrame[iconNum]->SetPosition(m_lifeIconFramePos[iconNum]);

		//アイコン、枠スプライトの拡大率を設定
		m_lifeIconSprite[iconNum]->SetScale(LIFE_ICON_SCALE);
		m_lifeIconFrame[iconNum]->SetScale(LIFE_ICON_FRAME_SCALE);

		//アイコン、枠スプライトの2つめのアイコンは左右反転させたいのでxの拡大率に-1を掛ける
		if (iconNum == 1) {
			m_lifeIconSprite[iconNum]->SetScale({ -LIFE_ICON_SCALE.x ,LIFE_ICON_SCALE.y,LIFE_ICON_SCALE.z });
			m_lifeIconFrame[iconNum]->SetScale({ -LIFE_ICON_FRAME_SCALE.x,LIFE_ICON_FRAME_SCALE.y,LIFE_ICON_FRAME_SCALE.z });
		}
		
		//色を設定
		m_lifeIconSprite[iconNum]->SetColor(LIFE_ICON_COLOR);
		m_lifeIconFrame[iconNum]->SetColor(LIFE_ICON_FRAME_COLOR);

		//ピボットを設定
		m_lifeIconSprite[iconNum]->SetPivot(SPRITE_PIVOT);
		m_lifeIconFrame[iconNum]->SetPivot(SPRITE_PIVOT);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	//発光スプライトの初期化
	m_lifeIconHalo = NewGO<SpriteRender>(0);
	m_lifeIconHalo->Init(
		LIFE_ICON_HALO_SPRITE_FILEPATH,
		LIFE_ICON_HALO_WIDTH,
		LIFE_ICON_HALO_HEIGHT,
		AlphaBlendMode_Trans
	);
	//位置を3つ目のライフアイコン位置にしておく
	m_lifeIconHalo->SetPosition(m_lifeIconSpritePos[2]);
	//拡大率を設定
	m_lifeIconHaloScale = LIFE_ICON_HALO_SCALE;
	m_lifeIconHalo->SetScale(m_lifeIconHaloScale);
	//色を設定
	m_lifeIconHaloColor = LIFE_ICON_HALO_COLOR;
	m_lifeIconHalo->SetColor(m_lifeIconHaloColor * 1.2f);
	//ピボットを設定
	m_lifeIconHalo->SetPivot(SPRITE_PIVOT);
	//////////////////////////////////////////////////////////////////////////////////////////////

}

void Life::LifeDisp()
{
	//残機2のとき、3つ目のアイコンをフェードアウトさせる
	if (GameDirector::GetInstance()->GetPlayerLife() <= 2) {
		m_lifeIconSprite[2]->FadeOut(0.04f);
	}
	//残機1のとき、2つ目のアイコンをフェードアウトさせる
	if (GameDirector::GetInstance()->GetPlayerLife() <= 1) {
		m_lifeIconSprite[1]->FadeOut(0.04f);
	}
	//残機0のとき、1つ目のアイコンをフェードアウトさせる
	if (GameDirector::GetInstance()->GetPlayerLife() <= 0) {
		m_lifeIconSprite[0]->FadeOut(0.04f);
	}

}

void Life::HaloDisp()
{
	//発光スプライトの位置決め
	//残機3の場合
	if (GameDirector::GetInstance()->GetPlayerLife() == 3) {
		m_lifeIconHaloPos = m_lifeIconSpritePos[2];
	}
	//残機2の場合はそのアイコンへ移動
	if (GameDirector::GetInstance()->GetPlayerLife() == 2
		&& m_isHaloFadeOutReady == false) {
		m_lifeIconHaloPos = m_lifeIconSpritePos[1];
		m_lifeIconHaloScale = LIFE_ICON_HALO_SCALE;
	}
	//残機1の場合はそのアイコンへ移動
	if (GameDirector::GetInstance()->GetPlayerLife() == 1
		&& m_isHaloFadeOutReady == false) {
		m_lifeIconHaloPos = m_lifeIconSpritePos[0];
		m_lifeIconHaloScale = LIFE_ICON_HALO_SCALE;
	}

	//変更後の座標を反映させる
	m_lifeIconHalo->SetPosition(m_lifeIconHaloPos);

}

void Life::VariableHalo()
{
	//色を線形補完させることで明滅させる
	//色の最小値、最大値を決める
	Vector4 minColor, maxColor;
	minColor = LIFE_ICON_HALO_COLOR * 0.7f;
	maxColor = LIFE_ICON_HALO_COLOR * 1.4f;

	//明るさの折り返しフラグを設定
	//発光補間率が1以上ならフラグオン
	if (m_lifeIconHaloColorRate >= 1.0f) {
		m_lifeIconHaloMaxFlag = true;
	}
	else if (m_lifeIconHaloColorRate <= 0.0f) {
		//0以下ならオフ
		m_lifeIconHaloMaxFlag = false;
	}

	//補間率を変更
	//フラグオンなら補間率を上昇
	if (m_lifeIconHaloMaxFlag == false) {
		m_lifeIconHaloColorRate += 0.03f;
	}
	else {
		//オフなら減少
		m_lifeIconHaloColorRate -= 0.01f;
	}

	//明るさを変更
	//補間率で線形補完
	m_lifeIconHaloColor.Lerp(
		m_lifeIconHaloColorRate,
		minColor,
		maxColor
	);

	//色を設定
	m_lifeIconHalo->SetColor(m_lifeIconHaloColor);
}

void Life::HaloVanish()
{
	//前フレームからライフの値に変化があったら消える準備フラグをオン
	if (m_prevPlayerLife - GameDirector::GetInstance()->GetPlayerLife() >= 1) {
		m_isHaloFadeOutReady = true;
	}

	//消える準備フラグがオンのとき、変形
	if (m_isHaloFadeOutReady == true) {
		//色を最大の明るさにする
		m_lifeIconHaloColorRate = 1.0f;
		//横方向に広げる
		m_lifeIconHaloScale.x += X_SCALE_RATE;
		//縦方向に縮めて消えたように見せる
		m_lifeIconHaloScale.y -= Y_SCALE_RATE;
	}

	if (m_lifeIconHaloScale.y <= 0.0f) {
		m_lifeIconHaloScale.y = 0.0f;
		//縦のスケールが0以下になったら(=見た目上消えたら)0にして、消える準備フラグをオフ
		m_isHaloFadeOutReady = false;
	}

	//発光スプライトに拡大率を反映させる
	m_lifeIconHalo->SetScale(m_lifeIconHaloScale);
}

void Life::Update()
{
	LifeDisp();
	VariableHalo();
	HaloVanish();
	HaloDisp();
	
	//現フレームでのプレイヤーのライフを記録
	m_prevPlayerLife = GameDirector::GetInstance()->GetPlayerLife();
}