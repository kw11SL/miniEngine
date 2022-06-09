#include "stdafx.h"
#include "WaveCutIn.h"

namespace {

	//wave最大数
	const int WAV_NUM_MAX = 9;

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
	//フェードイン、アウト関連
	const float FADEIN_RATE = 0.06f;	//フェードインの割合
	const float FADEOUT_RATE = 0.06f;	//フェードアウトの割合

	//SE関連
	//カットイン時のSE
	const wchar_t* SE_CUTIN_FILEPATH = L"Assets/wav/wave_change_2.wav";	//ファイルパス
	const float SE_CUTIN_VOLUME = 1.0f;									//ボリューム
}

WaveCutIn::~WaveCutIn()
{
	//可変長配列内のスプライトとオブジェクト自身を削除
	for (auto& sCutinSprite : m_cutinSprites) {
		//スプライトを削除
		DeleteGO(sCutinSprite->cutinSprite);

		delete sCutinSprite;
		sCutinSprite = nullptr;
	}
}

void WaveCutIn::Init()
{
	//スプライトの作成
	for (int i = 0; i < WAV_NUM_MAX; i++) {
		
		//wave数を一度string型に変換
		std::string numStr = std::to_string(i+1);
		//string型でファイルパスを構成するため、パス + (wave数) + 拡張子部分で文字を結合する
		std::string convFilePath = FILEPATH_BASE_1 + numStr + FILEPATH_BASE_2;
		//できたstring型のファイルパスをconst char*型に変換
		const char* newFilePath = convFilePath.c_str();

		//カットインするスプライトを可変長配列内に作成
		m_cutinSprites.push_back(new SCutinSprite);

		//スプライトを作成
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite = NewGO<SpriteRender>(0);
		//作成したものを初期化
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->Init(
			newFilePath,
			WAVE_SPRITE_WIDTH,
			WAVE_SPRITE_HEIGHT,
			AlphaBlendMode_Trans
		);

		//ピボット、拡大率、位置、色を設定
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->SetPivot(SPRITE_PIVOT);
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->SetScale(WAVE_SPRITE_SCALE);
		m_cutinSprites[m_cutinSprites.size() - 1]->pos = WAVE_SPRITE_INIT_POS;
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->SetPosition(m_cutinSprites[m_cutinSprites.size() - 1]->pos);
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->SetColor(WAVE_SPRITE_INIT_COLOR);

		//中央で停止する時間を設定
		m_cutinSprites[m_cutinSprites.size() - 1]->waitCounter = 1.0f;
	}
}

void WaveCutIn::SpriteMove(SCutinSprite* sCutinSprite)
{
	//引数の構造体がnullだったら処理しない
	if (sCutinSprite == nullptr) {
		return;
	}

	//移動可能なら移動用の補間率を上昇
	if (sCutinSprite->isValidMove == true) {
		sCutinSprite->moveFraction += 0.025f;
	}

	//中間地点で停止
	if (sCutinSprite->moveFraction > 0.5f
		&& sCutinSprite->isMoveFinishToCenter == false) {
	
		sCutinSprite->moveFraction = 0.5f;

		//中央に移動したフラグをオン
		sCutinSprite->isMoveFinishToCenter = true;
		//移動可能フラグをオフ
		sCutinSprite->isValidMove = false;
	}

	//中央に移動が完了していたら、停止カウンタを減少、
	if (sCutinSprite->isMoveFinishToCenter == true) {
		sCutinSprite->waitCounter -= g_gameTime->GetFrameDeltaTime();
	}

	//停止カウンタが0以下になったら再度、移動可能にする
	if (sCutinSprite->waitCounter <= 0.0f) {
		sCutinSprite->waitCounter = 0.0f;
		
		if (sCutinSprite->isValidMove == false) {
			sCutinSprite->isValidMove = true;
		}
	}

	//右端まで移動したら
	if (sCutinSprite->moveFraction >= 1.0f) {
		
		//移動が完了したのでカウンターとフラグをリセット
		sCutinSprite->moveFraction = 0.0f;
		sCutinSprite->isMoveFinishToCenter = false;
		sCutinSprite->waitCounter = 1.0f;
		sCutinSprite->isValidMove = false;
	}

	//位置を線形補完
	sCutinSprite->pos.Lerp(
		sCutinSprite->moveFraction,
		WAVE_SPRITE_INIT_POS,
		WAVE_SPRITE_END_POS
	);

	//位置を更新
	sCutinSprite->cutinSprite->SetPosition(sCutinSprite->pos);
}

void WaveCutIn::SpriteFade(SCutinSprite* sCutinSprite)
{
	//引数の構造体がnullだったら処理しない
	if (sCutinSprite == nullptr) {
		return;
	}

	//移動可能かつ停止カウンターが上昇中なら
	if (sCutinSprite->waitCounter > 0.0f
		&& sCutinSprite->isValidMove == true) {
		
		//フェードイン
		sCutinSprite->cutinSprite->FadeIn(FADEIN_RATE);
	}
	//移動可能かつ停止カウンターが0以下なら(=停止処理が終わっていたら)
	else if (sCutinSprite->waitCounter <= 0.0f
		&& sCutinSprite->isValidMove == true) {

		//フェードアウト
		sCutinSprite->cutinSprite->FadeOut(FADEOUT_RATE);
	}
}


void WaveCutIn::Update()
{
	//ゲーム中以外なら処理しない
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//現在のwave数を取得
	int waveNum = GameDirector::GetInstance()->GetWaveNumber();
	//更新対象の配列番号を設定
	//例：wave1のとき配列0番目のスプライトを更新
	int updateTargetNum = waveNum - 1;

	//ゲームが開始時、ゲーム開始フラグがオフなら
	if (GameDirector::GetInstance()->GetGameState() == enGame 
		&& m_isStart == false) {
		
		//seを再生
		CSoundSource* ssCutInSe = NewGO<CSoundSource>(0);
		ssCutInSe->Init(SE_CUTIN_FILEPATH);
		ssCutInSe->SetVolume(SE_CUTIN_VOLUME);
		ssCutInSe->Play(false);

		//開始フラグをオン
		m_isStart = true;
	}

	//waveが切り替わっていたら
	if (GameDirector::GetInstance()->GetIsSwitchedWave()) {
		
		//seを再生
		CSoundSource* ssCutInSe = NewGO<CSoundSource>(0);
		ssCutInSe->Init(SE_CUTIN_FILEPATH);
		ssCutInSe->SetVolume(SE_CUTIN_VOLUME);
		ssCutInSe->Play(false);

	}

	//更新対象のカットインスプライトの移動
	SpriteMove(m_cutinSprites[updateTargetNum]);
	//更新対象のカットインスプライトのフェード処理
	SpriteFade(m_cutinSprites[updateTargetNum]);

	//現フレームのwave数を記録
	m_waveNumPrevFrame = waveNum;
}