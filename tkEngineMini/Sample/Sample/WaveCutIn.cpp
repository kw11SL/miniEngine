#include "stdafx.h"
#include "WaveCutIn.h"

namespace {
	//ファイルパス
	const char* WAVE_SPRITE_FILEPATH = "Assets/sprite/ui/wave1.dds";

	//wave数スプライト初期化用ファイルパスの前半と拡張子部分
	const std::string FILEPATH_BASE_1 = "Assets/sprite/ui/wave";
	const std::string FILEPATH_BASE_2 = ".dds";

	//スプライトの初期位置
	const Vector3 WAVE_SPRITE_INIT_POS = { 320,0.0f,0.0f };
	//スプライトの最終位置
	const Vector3 WAVE_SPRITE_END_POS = { WAVE_SPRITE_INIT_POS.x * -1.0f,0.0f,0.0f };
	
	//カラー
	const Vector4 WAVE_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };
	
	//拡大率
	const Vector3 WAVE_SPRITE_SCALE = Vector3::One;
	const float WAVE_SPRITE_SCALE_RATE = 1.0f;

	//幅、高さ
	const int WAVE_SPRITE_WIDTH = 512;
	const int WAVE_SPRITE_HEIGHT = 256;

	//ピボット
	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };
}

WaveCutIn::~WaveCutIn()
{
	DeleteGO(m_waveSprite);
}

void WaveCutIn::Init()
{
	//スプライトの初期化
	m_waveSprite = NewGO<SpriteRender>(0);
	
	//現在のwave数に応じたスプライトで初期化
	SwitchingSprite();

	//中央で停止する時間のカウンタを初期化
	m_waveSpriteWaitCounter = 1.0f;

}

void WaveCutIn::SwitchingSprite()
{
	//現在のwave数を取得
	int waveNum = GameDirector::GetInstance()->GetWaveNumber();
	//wave数を一度string型に変換
	std::string numStr = std::to_string(waveNum);

	//string型でファイルパスを構成するため、パス + (wave数) + 拡張子部分で文字を結合する
	std::string convFilePath = FILEPATH_BASE_1 + numStr + FILEPATH_BASE_2;
	//できたstring型のファイルパスをconst char*型に変換
	const char* newFilePath = convFilePath.c_str();

	//スプライトの初期化
	m_waveSprite->Init(
		newFilePath,
		WAVE_SPRITE_WIDTH,
		WAVE_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveSprite->SetPivot(SPRITE_PIVOT);
	m_waveSprite->SetScale(WAVE_SPRITE_SCALE);
	m_waveSprite->SetPosition(WAVE_SPRITE_INIT_POS);
	m_waveSprite->SetColor(WAVE_SPRITE_INIT_COLOR);

}


void WaveCutIn::SpriteMove()
{
	////テスト
	//if (m_isValidMove == false) {
	//	if (g_pad[0]->IsTrigger(enButtonA)) {
	//		m_isValidMove = true;
	//	}
	//}

	

	//移動用の補間率を上昇
	if (m_isValidMove == true) {
		m_waveSpriteMoveFraction += 0.025f;
	}

	//中間地点で停止
	if (m_waveSpriteMoveFraction > 0.5f && m_isFinishMoveToCenter == false) {
		m_waveSpriteMoveFraction = 0.5;

		m_isFinishMoveToCenter = true;
		m_isValidMove = false;
	}

	//移動再開までのカウンタを減少し、カウンタが0になったら移動可能にする
	if (m_isFinishMoveToCenter == true) {
		m_waveSpriteWaitCounter -= g_gameTime->GetFrameDeltaTime();
	}
	if (m_waveSpriteWaitCounter <= 0.0f) {
		m_waveSpriteWaitCounter = 0.0f;
		if(m_isValidMove == false){
			m_isValidMove = true;
		}
	}

	//右端まで移動した
	if (m_waveSpriteMoveFraction >= 1.0f) {
		
		//移動が完了したのでカウンターとフラグをリセット
		m_waveSpriteMoveFraction = 0.0f;
		m_isFinishMoveToCenter = false;
		m_waveSpriteWaitCounter = 1.0f;
		m_isValidMove = false;
	}


	//位置を線形補完
	m_waveSpritePos.Lerp(
		m_waveSpriteMoveFraction,
		WAVE_SPRITE_INIT_POS,
		WAVE_SPRITE_END_POS
	);

	//位置を更新
	m_waveSprite->SetPosition(m_waveSpritePos);
	

}

void WaveCutIn::SpriteFade()
{
	if (m_waveSpriteWaitCounter > 0.0f && m_isValidMove == true) {
		m_waveSprite->FadeIn(0.06f);
	}
	else if (m_waveSpriteWaitCounter <= 0.0f && m_isValidMove == true) {
		m_waveSprite->FadeOut(0.06f);
	}


}

void WaveCutIn::Update()
{
	//ゲーム中以外なら処理しない
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//waveが切り替わっていたら
	if (GameDirector::GetInstance()->GetIsSwitchedWave()) {
		//スプライトを切り替える
		SwitchingSprite();

		//移動可能フラグをオン
		m_isValidMove = true;
	}

	SpriteMove();
	
	SpriteFade();

	//m_waveSprite->FadeIn(0.03f);

}