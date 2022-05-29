#include "stdafx.h"
#include "Result.h"

namespace {
	//ファイルパス
	const char* SCREEN_SPRITE_FILEPATH = "Assets/sprite/ui/screen_black.dds";
	const char* RESULT_SPRITE_FILEPATH = "Assets/sprite/ui/result.dds";
	const char* REMAIN_PLAYER_SPRITE_FILEPATH = "Assets/sprite/ui/remainPlayer.dds";
	const char* DESTRUCTION_RATE_SPRITE_FILEPATH = "Assets/sprite/ui/destructionRate.dds";
	const char* FINAL_SCORE_SPRITE_FILEPATH = "Assets/sprite/ui/finalScore.dds";

	//スプライトの幅、高さ
	const int SCREEN_SPRITE_WIDTH = 1280;
	const int SCREEN_SPRITE_HEIGHT = 720;
	const int RESULT_SPRITE_WIDTH = 512;
	const int RESULT_SPRITE_HEIGHT = 256;
	//共通の幅、高さ
	const int RESULT_TEXT_SPRITE_WIDTH = 512;
	const int RESULT_TEXT_SPRITE_HEIGHT = 256;

	//スプライトの位置
	const Vector3 SCREEN_SPRITE_INIT_POS = { 0.0f,0.0f,0.0f, };
	const Vector3 RESULT_SPRITE_INIT_POS = { 0.0f,250.0f,0.0f };
	//以下、各項目ごとの相対位置
	const Vector3 RELATIVE_DISTANCE = { 0.0f,-150.0f,0.0f };

	//スプライトのスケール
	const Vector3 RESULT_SPRITE_SCALE = { 0.5f,0.5f,1.0f };
	//スプライトのカラー
	const Vector4 SCREEN_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };
	const Vector4 RESULT_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };
	
	//スプライトのピボット
	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };

	//フェードイン関係
	const float SCREEN_SPRITE_FADEIN_RATE = 0.02f;
	const float TEXT_SPRITE_FADEIN_RATE = 0.03f;

	//テキスト関連
	//設定するテキスト
	const wchar_t* REMAIN_PLAYER_BONUS_TEXT = L"REMAIN BONUS + ";
	const wchar_t* DESTRUCTION_BONUS_TEXT = L"DESTRUCTION BONUS + ";

	//撃破率の文字の後につける%の文字
	const wchar_t* TEXT_PERCENT = L" %";
	const wchar_t* TEXT_PTS = L" pts.";

	//テキストの相対位置(上下)
	const Vector2 RELATIVE_DISTANCE_TEXT = { 0.0f,-50.0f };

	//テキスト部分の初期カラー(見えない)
	const Vector4 TEXT_INIT_COLOR = { 0.0f,0.0f,0.0f,0.0f };
	//テキストのフェードインの速さ
	const float TEXT_FADEIN_RATE = 0.04f;

}


Result::Result()
{

}

Result::~Result()
{
	//スプライト(オレンジ色の文字)
	DeleteGO(m_screenSprite);
	DeleteGO(m_resultSprite);
	DeleteGO(m_pressButtonSprite);

	//残機
	DeleteGO(m_remainPlayerTextSprite);
	DeleteGO(m_remainPlayerNum);
	DeleteGO(m_remainPlayerBonusText);
	DeleteGO(m_remainPlayerBonusNum);

	//撃破率
	DeleteGO(m_destructionRateTextSprite);
	DeleteGO(m_destructionRateNum);
	DeleteGO(m_destructionBonusText);
	DeleteGO(m_destructionBonusNum);

	//最終スコア
	DeleteGO(m_finalScoreTextSprite);
	DeleteGO(m_finalScoreNum);

	//フェードアウトスプライト
	DeleteGO(m_screenTopSprite);
}

void Result::Init()
{
	////////////////////////////////////////////////////////////////////////////////
	//スプライトの初期化処理
	//背景を暗くするスプライトの初期化
	m_screenSprite = NewGO<SpriteRender>(0);
	m_screenSprite->Init(
		SCREEN_SPRITE_FILEPATH,
		SCREEN_SPRITE_WIDTH,
		SCREEN_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);
	m_screenSprite->SetPivot(SPRITE_PIVOT);
	m_screenSprite->SetPosition(SCREEN_SPRITE_INIT_POS);
	m_screenSprite->SetColor(SCREEN_SPRITE_INIT_COLOR);

	//リザルトの文字スプライトの初期化
	m_resultSprite = NewGO<SpriteRender>(0);
	m_resultSprite->Init(
		RESULT_SPRITE_FILEPATH,
		RESULT_SPRITE_WIDTH,
		RESULT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);
	m_resultSprite->SetPivot(SPRITE_PIVOT);
	m_resultSprite->SetPosition(RESULT_SPRITE_INIT_POS);
	m_resultSprite->SetScale(RESULT_SPRITE_SCALE);
	m_resultSprite->SetColor(RESULT_SPRITE_INIT_COLOR);

	//残機の文字スプライトの初期化
	m_remainPlayerTextSprite = NewGO<SpriteRender>(0);
	m_remainPlayerTextSprite->Init(
		REMAIN_PLAYER_SPRITE_FILEPATH,
		RESULT_TEXT_SPRITE_WIDTH,
		RESULT_TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);
	m_remainPlayerTextSprite->SetPivot(SPRITE_PIVOT);
	m_remainPlayerTextSprite->SetPosition(m_resultSprite->GetPosition() + RELATIVE_DISTANCE);
	m_remainPlayerTextSprite->SetScale(RESULT_SPRITE_SCALE);
	m_remainPlayerTextSprite->SetColor(RESULT_SPRITE_INIT_COLOR);

	//撃破率の文字スプライトの初期化
	m_destructionRateTextSprite = NewGO<SpriteRender>(0);
	m_destructionRateTextSprite->Init(
		DESTRUCTION_RATE_SPRITE_FILEPATH,
		RESULT_TEXT_SPRITE_WIDTH,
		RESULT_TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);
	m_destructionRateTextSprite->SetPivot(SPRITE_PIVOT);
	m_destructionRateTextSprite->SetPosition(m_remainPlayerTextSprite->GetPosition() + RELATIVE_DISTANCE);
	m_destructionRateTextSprite->SetScale(RESULT_SPRITE_SCALE);
	m_destructionRateTextSprite->SetColor(RESULT_SPRITE_INIT_COLOR);

	//最終スコアの文字スプライトの初期化
	m_finalScoreTextSprite = NewGO<SpriteRender>(0);
	m_finalScoreTextSprite->Init(
		FINAL_SCORE_SPRITE_FILEPATH,
		RESULT_TEXT_SPRITE_WIDTH,
		RESULT_TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);
	m_finalScoreTextSprite->SetPivot(SPRITE_PIVOT);
	m_finalScoreTextSprite->SetPosition(m_destructionRateTextSprite->GetPosition() + RELATIVE_DISTANCE);
	m_finalScoreTextSprite->SetScale(RESULT_SPRITE_SCALE);
	m_finalScoreTextSprite->SetColor(RESULT_SPRITE_INIT_COLOR);

	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	//テキストの初期化処理
	//残機数を表示するテキストを初期化
	m_remainPlayerNum = NewGO<FontRender>(0);
	m_remainPlayerNum->Init(L"");
	m_remainPlayerNum->SetText(L"");
	m_remainPlayerNum->SetPivot({ 0.5f,0.5f });
	//remain playerスプライトからの相対位置
	Vector2 remainPlayerNumPos = {
		m_remainPlayerTextSprite->GetPosition().x + RELATIVE_DISTANCE_TEXT.x - 10.0f ,
		m_remainPlayerTextSprite->GetPosition().y + RELATIVE_DISTANCE_TEXT.y + 20.0f
	};

	m_remainPlayerNum->SetPosition(remainPlayerNumPos);
	m_remainPlayerNum->SetColor(TEXT_INIT_COLOR);
	m_remainPlayerNum->SetScale(1.0f);

	//残機数ボーナスのテキストを初期化
	m_remainPlayerBonusText = NewGO<FontRender>(0);
	m_remainPlayerBonusText->Init(L"");
	m_remainPlayerBonusText->SetText(REMAIN_PLAYER_BONUS_TEXT);
	m_remainPlayerBonusText->SetPivot({ 0.5f,0.5f });
	//残機数テキストからの相対位置
	Vector2 remainPlayerBonusTextPos = {
		m_remainPlayerNum->GetPosition().x + RELATIVE_DISTANCE_TEXT.x - 100.0f ,
		m_remainPlayerNum->GetPosition().y + RELATIVE_DISTANCE_TEXT.y
	};

	m_remainPlayerBonusText->SetPosition(remainPlayerBonusTextPos);
	m_remainPlayerBonusText->SetColor(TEXT_INIT_COLOR);
	m_remainPlayerBonusText->SetScale(1.0f);

	//残機数ボーナスの数値を初期化
	m_remainPlayerBonusNum = NewGO<FontRender>(0);
	m_remainPlayerBonusNum->Init(L"");
	m_remainPlayerBonusNum->SetPivot({ 0.5f,0.5f });

	//横にずらすだけでいいのでyは+0
	Vector2 remainPlayerBonusNumPos = {
		m_remainPlayerBonusText->GetPosition().x + 250.0f ,
		m_remainPlayerBonusText->GetPosition().y + 0.0f
	};

	m_remainPlayerBonusNum->SetPosition(remainPlayerBonusNumPos);
	m_remainPlayerBonusNum->SetColor(TEXT_INIT_COLOR);
	m_remainPlayerBonusNum->SetScale(1.0f);

	//敵撃破率の値を初期化
	m_destructionRateNum = NewGO<FontRender>(0);
	m_destructionRateNum->Init(L"");
	m_destructionRateNum->SetPivot({ 0.5f,0.5f });

	//destruction rate スプライトからの相対位置
	Vector2 destructionRateNumPos = {
		m_destructionRateTextSprite->GetPosition().x + RELATIVE_DISTANCE_TEXT.x - 30.0f ,
		m_destructionRateTextSprite->GetPosition().y + RELATIVE_DISTANCE_TEXT.y + 20.0f
	};

	m_destructionRateNum->SetPosition(destructionRateNumPos);
	m_destructionRateNum->SetColor(TEXT_INIT_COLOR);
	m_destructionRateNum->SetScale(1.0f);

	//撃破率ボーナスの文字を初期化
	m_destructionBonusText = NewGO<FontRender>(0);
	m_destructionBonusText->Init(L"");
	m_destructionBonusText->SetText(DESTRUCTION_BONUS_TEXT);
	m_destructionBonusText->SetPivot({ 0.5f,0.5f });

	//撃破率数値からの相対位置
	Vector2 destructionBonusTextPos = {
		m_destructionRateNum->GetPosition().x + RELATIVE_DISTANCE_TEXT.x -80.0f ,
		m_destructionRateNum->GetPosition().y + RELATIVE_DISTANCE_TEXT.y
	};

	m_destructionBonusText->SetPosition(destructionBonusTextPos);
	m_destructionBonusText->SetColor(TEXT_INIT_COLOR);
	m_destructionBonusText->SetScale(1.0f);

	//撃破率ボーナスの数値を初期化
	m_destructionBonusNum = NewGO<FontRender>(0);
	m_destructionBonusNum->Init(L"");
	m_destructionBonusNum->SetPivot({ 0.5f,0.5f });

	//撃破率ボーナスの文字から横にずらす
	Vector2 destructionBonusNumPos = {
		m_destructionBonusText->GetPosition().x + RELATIVE_DISTANCE_TEXT.x + 320.0f ,
		m_destructionBonusText->GetPosition().y + 0.0f
	};

	m_destructionBonusNum->SetPosition(destructionBonusNumPos);
	m_destructionBonusNum->SetColor(TEXT_INIT_COLOR);
	m_destructionBonusNum->SetScale(1.0f);

	//最終スコアの数値を初期化
	m_finalScoreNum = NewGO<FontRender>(0);
	m_finalScoreNum->Init(L"");
	m_finalScoreNum->SetPivot({ 0.5f,0.5f });

	//final scoreスプライトからの相対位置
	Vector2 finalScoreNumPos = {
		m_finalScoreTextSprite->GetPosition().x + RELATIVE_DISTANCE_TEXT.x - 50.0f ,
		m_finalScoreTextSprite->GetPosition().y + RELATIVE_DISTANCE_TEXT.y + 20.0f
	};

	m_finalScoreNum->SetPosition(finalScoreNumPos);
	m_finalScoreNum->SetColor(TEXT_INIT_COLOR);
	m_finalScoreNum->SetScale(1.0f);

	////////////////////////////////////////////////////////////////////////////////

	//フェードアウト用スプライトの初期化
	m_screenTopSprite = NewGO<SpriteRender>(0);
	m_screenTopSprite->Init(
		SCREEN_SPRITE_FILEPATH,
		SCREEN_SPRITE_WIDTH,
		SCREEN_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);
	m_screenTopSprite->SetPivot(SPRITE_PIVOT);
	m_screenTopSprite->SetPosition(SCREEN_SPRITE_INIT_POS);
	m_screenTopSprite->SetColor(SCREEN_SPRITE_INIT_COLOR);

}

void Result::FadeInSprite()
{
	//スプライトのフェードイン処理
	//リザルトの文字スプライトをフェードイン
	m_resultSprite->FadeIn(TEXT_SPRITE_FADEIN_RATE);

	//背景を暗くするためのスプライトは途中でフェードインを止める
	if (m_screenSprite->GetColor().w < 0.85f) {
		m_screenSprite->FadeIn(SCREEN_SPRITE_FADEIN_RATE);
	}

	//項目それぞれのスプライトをフェードイン
	//リザルトの文字がフェードインし終わったらフェードイン開始
	if (m_resultSprite->GetColor().w >= 1.0f) {
		m_remainPlayerTextSprite->FadeIn(TEXT_SPRITE_FADEIN_RATE);
	}
	//前の項目がフェードイン次第フェードイン開始
	if (m_remainPlayerTextSprite->GetColor().w >= 1.0f) {
		m_destructionRateTextSprite->FadeIn(TEXT_SPRITE_FADEIN_RATE);
	}
	//前の項目がフェードイン次第フェードイン開始
	if (m_destructionRateTextSprite->GetColor().w >= 1.0f) {
		m_finalScoreTextSprite->FadeIn(TEXT_SPRITE_FADEIN_RATE);
	}

	//final scoreのスプライトがフェードインしたらスプライトのフェードインが完了
	if (m_finalScoreTextSprite->GetColor().w >= 1.0f) {
		m_isFinishFadeInSprite = true;
	}

}

void Result::FadeInText()
{
	//残機の表示
	int plRemainNum = GameDirector::GetInstance()->GetPlayerLife();
	m_remainPlayerNumWs = std::to_wstring(plRemainNum);
	const wchar_t* remainNumWc = m_remainPlayerNumWs.c_str();
	m_remainPlayerNum->SetText(remainNumWc);

	if (m_remainPlayerNum->GetColor().w < 1.0f) {
		m_remainPlayerNum->SetColor(
			{
				m_remainPlayerNum->GetColor().x + TEXT_FADEIN_RATE,
				m_remainPlayerNum->GetColor().y + TEXT_FADEIN_RATE,
				m_remainPlayerNum->GetColor().z + TEXT_FADEIN_RATE,
				m_remainPlayerNum->GetColor().w + TEXT_FADEIN_RATE
			}
		);
	}

	//残機ボーナス文字の表示
	if (m_remainPlayerBonusText->GetColor().w < 1.0f) {
		//残機数のフェードインが終わっていたらフェードイン開始
		if (m_remainPlayerNum->GetColor().w >= 1.0f) {
			m_remainPlayerBonusText->SetColor(
				{
					m_remainPlayerBonusText->GetColor().x + TEXT_FADEIN_RATE,
					m_remainPlayerBonusText->GetColor().y + TEXT_FADEIN_RATE,
					m_remainPlayerBonusText->GetColor().z + TEXT_FADEIN_RATE,
					m_remainPlayerBonusText->GetColor().w + TEXT_FADEIN_RATE
				}
			);
		}
	}

	//残機ボーナス値の表示
	int remainBonus = GameDirector::GetInstance()->CalcRemainBonus();
	m_remainPlayerBonusNumWs = std::to_wstring(remainBonus);
	//数字の後にpts.をつける
	m_remainPlayerBonusNumWs += TEXT_PTS;
	const wchar_t* remainBonusWc = m_remainPlayerBonusNumWs.c_str();
	m_remainPlayerBonusNum->SetText(remainBonusWc);

	//残機ボーナス文字のフェードインが終わっていたらフェードイン開始
	if (m_remainPlayerBonusText->GetColor().w >= 1.0f) {
		if (m_remainPlayerBonusNum->GetColor().w < 1.0f) {
			m_remainPlayerBonusNum->SetColor(
				{
					m_remainPlayerBonusNum->GetColor().x + TEXT_FADEIN_RATE,
					m_remainPlayerBonusNum->GetColor().y + TEXT_FADEIN_RATE,
					m_remainPlayerBonusNum->GetColor().z + TEXT_FADEIN_RATE,
					m_remainPlayerBonusNum->GetColor().w + TEXT_FADEIN_RATE
				}
			);
		}
	}

	//敵撃破率の表示
	float destructionRate = GameDirector::GetInstance()->CalcDestructionRate();
	std::wstringstream dRwss;
	dRwss.precision(4);
	dRwss << destructionRate;

	m_destructionRateWs = dRwss.str();
	//数字の後に%をつける
	m_destructionRateWs += TEXT_PERCENT;
	const wchar_t* destructionRateWc = m_destructionRateWs.c_str();
	m_destructionRateNum->SetText(destructionRateWc);

	//残機ボーナス文字のフェードインが終わっていたらフェードイン開始
	if (m_remainPlayerBonusNum->GetColor().w >= 1.0f) {
		if (m_destructionRateNum->GetColor().w < 1.0f) {
			m_destructionRateNum->SetColor(
				{
					m_destructionRateNum->GetColor().x + TEXT_FADEIN_RATE,
					m_destructionRateNum->GetColor().y + TEXT_FADEIN_RATE,
					m_destructionRateNum->GetColor().z + TEXT_FADEIN_RATE,
					m_destructionRateNum->GetColor().w + TEXT_FADEIN_RATE
				}
			);
		}
	}

	//撃破率ボーナスの文字の表示

	//残機ボーナス数値のフェードインが終わっていたらフェードイン開始
	if (m_destructionRateNum->GetColor().w >= 1.0f) {
		if (m_destructionBonusText->GetColor().w < 1.0f) {
			m_destructionBonusText->SetColor(
				{
					m_destructionBonusText->GetColor().x + TEXT_FADEIN_RATE,
					m_destructionBonusText->GetColor().y + TEXT_FADEIN_RATE,
					m_destructionBonusText->GetColor().z + TEXT_FADEIN_RATE,
					m_destructionBonusText->GetColor().w + TEXT_FADEIN_RATE
				}
			);
		}
	}

	//撃破率ボーナスの値の表示
	int destructionBonus = GameDirector::GetInstance()->CalcDestructionBonus();
	m_destructionBonusWs = std::to_wstring(destructionBonus);
	//数字の後にpts.をつける
	m_destructionBonusWs += TEXT_PTS;
	const wchar_t* destructionBonusWc = m_destructionBonusWs.c_str();
	m_destructionBonusNum->SetText(destructionBonusWc);

	//残機ボーナス文字のフェードインが終わっていたらフェードイン開始
	if (m_destructionBonusText->GetColor().w >= 1.0f) {
		if (m_destructionBonusNum->GetColor().w < 1.0f) {
			m_destructionBonusNum->SetColor(
				{
					m_destructionBonusNum->GetColor().x + TEXT_FADEIN_RATE,
					m_destructionBonusNum->GetColor().y + TEXT_FADEIN_RATE,
					m_destructionBonusNum->GetColor().z + TEXT_FADEIN_RATE,
					m_destructionBonusNum->GetColor().w + TEXT_FADEIN_RATE
				}
			);
		}
	}

	//最終スコアの値の表示
	int finalScore = GameDirector::GetInstance()->CalcFinalScore();
	m_finalScoreNumWs = std::to_wstring(finalScore);
	//数字の後にpts.をつける
	m_finalScoreNumWs += TEXT_PTS;
	const wchar_t* finalScoreWc = m_finalScoreNumWs.c_str();
	m_finalScoreNum->SetText(finalScoreWc);

	//撃破率ボーナス値のフェードインが終わっていたらフェードイン開始
	if (m_destructionBonusNum->GetColor().w >= 1.0f) {
		if (m_finalScoreNum->GetColor().w < 1.0f) {
			m_finalScoreNum->SetColor(
				{
					m_finalScoreNum->GetColor().x + TEXT_FADEIN_RATE,
					m_finalScoreNum->GetColor().y + TEXT_FADEIN_RATE,
					m_finalScoreNum->GetColor().z + TEXT_FADEIN_RATE,
					m_finalScoreNum->GetColor().w + TEXT_FADEIN_RATE
				}
			);
		}
	}

	//最終スコア数値がフェードインし終わったらテキストのフェードインは完了
	if (m_finalScoreNum->GetColor().w >= 1.0f) {
		m_isFinishFadeInText = true;
	}

}

void Result::FadeOutText(const float fadeOutRate)
{
	//残機数
	m_remainPlayerNum->SetColor(
		{
			m_remainPlayerNum->GetColor().x - fadeOutRate,
			m_remainPlayerNum->GetColor().y - fadeOutRate,
			m_remainPlayerNum->GetColor().z - fadeOutRate,
			m_remainPlayerNum->GetColor().w - fadeOutRate
		}
	);
	//残機ボーナス文字
	m_remainPlayerBonusText->SetColor(
		{
			m_remainPlayerBonusText->GetColor().x - fadeOutRate,
			m_remainPlayerBonusText->GetColor().y - fadeOutRate,
			m_remainPlayerBonusText->GetColor().z - fadeOutRate,
			m_remainPlayerBonusText->GetColor().w - fadeOutRate
		}
	);
	//残機ボーナス値
	m_remainPlayerBonusNum->SetColor(
		{
			m_remainPlayerBonusNum->GetColor().x - fadeOutRate,
			m_remainPlayerBonusNum->GetColor().y - fadeOutRate,
			m_remainPlayerBonusNum->GetColor().z - fadeOutRate,
			m_remainPlayerBonusNum->GetColor().w - fadeOutRate
		}
	);
	//撃破率
	m_destructionRateNum->SetColor(
		{
			m_destructionRateNum->GetColor().x - fadeOutRate,
			m_destructionRateNum->GetColor().y - fadeOutRate,
			m_destructionRateNum->GetColor().z - fadeOutRate,
			m_destructionRateNum->GetColor().w - fadeOutRate
		}
	);
	//撃破率ボーナス文字
	m_destructionBonusText->SetColor(
		{
			m_destructionBonusText->GetColor().x - fadeOutRate,
			m_destructionBonusText->GetColor().y - fadeOutRate,
			m_destructionBonusText->GetColor().z - fadeOutRate,
			m_destructionBonusText->GetColor().w - fadeOutRate
		}
	);
	//撃破率ボーナス値
	m_destructionBonusNum->SetColor(
		{
			m_destructionBonusNum->GetColor().x - fadeOutRate,
			m_destructionBonusNum->GetColor().y - fadeOutRate,
			m_destructionBonusNum->GetColor().z - fadeOutRate,
			m_destructionBonusNum->GetColor().w - fadeOutRate
		}
	);
	//最終スコア値
	m_finalScoreNum->SetColor(
		{
			m_finalScoreNum->GetColor().x - fadeOutRate,
			m_finalScoreNum->GetColor().y - fadeOutRate,
			m_finalScoreNum->GetColor().z - fadeOutRate,
			m_finalScoreNum->GetColor().w - fadeOutRate
		}
	);

}

void Result::Update()
{
	if (GameDirector::GetInstance()->GetGameState() == enResult) {

		if (m_exitFlag == false) {
			/////////////////////////////////////////////////////////////////////////
			//スプライトのフェードイン
			FadeInSprite();

			/////////////////////////////////////////////////////////////////////////
			//テキストの表示
			//文字スプライトのフェードインが終わっていたら開始
			if (m_isFinishFadeInSprite == true) {
				FadeInText();
			}
			/////////////////////////////////////////////////////////////////////////
		}

		/////////////////////////////////////////////////////////////////////////
		//タイトル画面への遷移
		//ボタンを押すとフェードアウト開始
		if (g_pad[0]->IsTrigger(enButtonA) && m_isFinishFadeInText == true) {
			m_exitFlag = true;
		}
		if (m_exitFlag == true) {
			//フェードアウト(黒スプライトをフェードイン)
			m_screenTopSprite->FadeIn(0.01f);

			FadeOutText(0.01f);

		}
		//フェードアウトしきったら
		if (m_screenTopSprite->GetColor().w >= 1.0f) {
			
			//ゲームを削除し、タイトルをNewGOさせる
			QueryGOs<Game>("game", [&](Game* gameScene) {

				//ゲームを削除
				DeleteGO(gameScene);

				//弾を削除
				BulletManager::GetInstance()->DeleteBullets();
				//爆発を削除
				ExplosionManager::GetInstance()->DeleteExplosions();
				//生成されたエネミーを削除
				EnemyManager::GetInstance()->DeleteEnemies();

				//タイトルをNewGO
				NewGO<Title>(0, "title");

				//問い合わせ終了
				return false;
			});
		}

	}
}

