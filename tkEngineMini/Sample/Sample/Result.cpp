#include "stdafx.h"
#include "Result.h"

namespace {
	//�t�@�C���p�X
	const char* SCREEN_SPRITE_FILEPATH = "Assets/sprite/ui/screen_black.dds";
	const char* RESULT_SPRITE_FILEPATH = "Assets/sprite/ui/result.dds";
	const char* REMAIN_PLAYER_SPRITE_FILEPATH = "Assets/sprite/ui/remainPlayer.dds";
	const char* DESTRUCTION_RATE_SPRITE_FILEPATH = "Assets/sprite/ui/destructionRate.dds";
	const char* FINAL_SCORE_SPRITE_FILEPATH = "Assets/sprite/ui/finalScore.dds";

	//�X�v���C�g�̕��A����
	const int SCREEN_SPRITE_WIDTH = 1280;
	const int SCREEN_SPRITE_HEIGHT = 720;
	const int RESULT_SPRITE_WIDTH = 512;
	const int RESULT_SPRITE_HEIGHT = 256;
	//���ʂ̕��A����
	const int RESULT_TEXT_SPRITE_WIDTH = 512;
	const int RESULT_TEXT_SPRITE_HEIGHT = 256;

	//�X�v���C�g�̈ʒu
	const Vector3 SCREEN_SPRITE_INIT_POS = { 0.0f,0.0f,0.0f, };
	const Vector3 RESULT_SPRITE_INIT_POS = { 0.0f,250.0f,0.0f };
	//�ȉ��A�e���ڂ��Ƃ̑��Έʒu
	const Vector3 RELATIVE_DISTANCE = { 0.0f,-150.0f,0.0f };

	//�X�v���C�g�̃X�P�[��
	const Vector3 RESULT_SPRITE_SCALE = { 0.5f,0.5f,1.0f };
	//�X�v���C�g�̃J���[
	const Vector4 SCREEN_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };
	const Vector4 RESULT_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };
	
	//�X�v���C�g�̃s�{�b�g
	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };

	//�t�F�[�h�C���֌W
	const float SCREEN_SPRITE_FADEIN_RATE = 0.02f;
	const float TEXT_SPRITE_FADEIN_RATE = 0.03f;

	//�e�L�X�g�֘A
	//�ݒ肷��e�L�X�g
	const wchar_t* REMAIN_PLAYER_BONUS_TEXT = L"REMAIN BONUS + ";
	const wchar_t* DESTRUCTION_BONUS_TEXT = L"DESTRUCTION BONUS + ";

	//���j���̕����̌�ɂ���%�̕���
	const wchar_t* TEXT_PERCENT = L" %";
	const wchar_t* TEXT_PTS = L" pts.";

	//�e�L�X�g�̑��Έʒu(�㉺)
	const Vector2 RELATIVE_DISTANCE_TEXT = { 0.0f,-50.0f };

	//�e�L�X�g�����̏����J���[(�����Ȃ�)
	const Vector4 TEXT_INIT_COLOR = { 0.0f,0.0f,0.0f,0.0f };
	//�e�L�X�g�̃t�F�[�h�C���̑���
	const float TEXT_FADEIN_RATE = 0.04f;

}


Result::Result()
{

}

Result::~Result()
{
	//�X�v���C�g(�I�����W�F�̕���)
	DeleteGO(m_screenSprite);
	DeleteGO(m_resultSprite);
	DeleteGO(m_pressButtonSprite);

	//�c�@
	DeleteGO(m_remainPlayerTextSprite);
	DeleteGO(m_remainPlayerNum);
	DeleteGO(m_remainPlayerBonusText);
	DeleteGO(m_remainPlayerBonusNum);

	//���j��
	DeleteGO(m_destructionRateTextSprite);
	DeleteGO(m_destructionRateNum);
	DeleteGO(m_destructionBonusText);
	DeleteGO(m_destructionBonusNum);

	//�ŏI�X�R�A
	DeleteGO(m_finalScoreTextSprite);
	DeleteGO(m_finalScoreNum);

	//�t�F�[�h�A�E�g�X�v���C�g
	DeleteGO(m_screenTopSprite);
}

void Result::Init()
{
	////////////////////////////////////////////////////////////////////////////////
	//�X�v���C�g�̏���������
	//�w�i���Â�����X�v���C�g�̏�����
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

	//���U���g�̕����X�v���C�g�̏�����
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

	//�c�@�̕����X�v���C�g�̏�����
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

	//���j���̕����X�v���C�g�̏�����
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

	//�ŏI�X�R�A�̕����X�v���C�g�̏�����
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
	//�e�L�X�g�̏���������
	//�c�@����\������e�L�X�g��������
	m_remainPlayerNum = NewGO<FontRender>(0);
	m_remainPlayerNum->Init(L"");
	m_remainPlayerNum->SetText(L"");
	m_remainPlayerNum->SetPivot({ 0.5f,0.5f });
	//remain player�X�v���C�g����̑��Έʒu
	Vector2 remainPlayerNumPos = {
		m_remainPlayerTextSprite->GetPosition().x + RELATIVE_DISTANCE_TEXT.x - 10.0f ,
		m_remainPlayerTextSprite->GetPosition().y + RELATIVE_DISTANCE_TEXT.y + 20.0f
	};

	m_remainPlayerNum->SetPosition(remainPlayerNumPos);
	m_remainPlayerNum->SetColor(TEXT_INIT_COLOR);
	m_remainPlayerNum->SetScale(1.0f);

	//�c�@���{�[�i�X�̃e�L�X�g��������
	m_remainPlayerBonusText = NewGO<FontRender>(0);
	m_remainPlayerBonusText->Init(L"");
	m_remainPlayerBonusText->SetText(REMAIN_PLAYER_BONUS_TEXT);
	m_remainPlayerBonusText->SetPivot({ 0.5f,0.5f });
	//�c�@���e�L�X�g����̑��Έʒu
	Vector2 remainPlayerBonusTextPos = {
		m_remainPlayerNum->GetPosition().x + RELATIVE_DISTANCE_TEXT.x - 100.0f ,
		m_remainPlayerNum->GetPosition().y + RELATIVE_DISTANCE_TEXT.y
	};

	m_remainPlayerBonusText->SetPosition(remainPlayerBonusTextPos);
	m_remainPlayerBonusText->SetColor(TEXT_INIT_COLOR);
	m_remainPlayerBonusText->SetScale(1.0f);

	//�c�@���{�[�i�X�̐��l��������
	m_remainPlayerBonusNum = NewGO<FontRender>(0);
	m_remainPlayerBonusNum->Init(L"");
	m_remainPlayerBonusNum->SetPivot({ 0.5f,0.5f });

	//���ɂ��炷�����ł����̂�y��+0
	Vector2 remainPlayerBonusNumPos = {
		m_remainPlayerBonusText->GetPosition().x + 250.0f ,
		m_remainPlayerBonusText->GetPosition().y + 0.0f
	};

	m_remainPlayerBonusNum->SetPosition(remainPlayerBonusNumPos);
	m_remainPlayerBonusNum->SetColor(TEXT_INIT_COLOR);
	m_remainPlayerBonusNum->SetScale(1.0f);

	//�G���j���̒l��������
	m_destructionRateNum = NewGO<FontRender>(0);
	m_destructionRateNum->Init(L"");
	m_destructionRateNum->SetPivot({ 0.5f,0.5f });

	//destruction rate �X�v���C�g����̑��Έʒu
	Vector2 destructionRateNumPos = {
		m_destructionRateTextSprite->GetPosition().x + RELATIVE_DISTANCE_TEXT.x - 30.0f ,
		m_destructionRateTextSprite->GetPosition().y + RELATIVE_DISTANCE_TEXT.y + 20.0f
	};

	m_destructionRateNum->SetPosition(destructionRateNumPos);
	m_destructionRateNum->SetColor(TEXT_INIT_COLOR);
	m_destructionRateNum->SetScale(1.0f);

	//���j���{�[�i�X�̕�����������
	m_destructionBonusText = NewGO<FontRender>(0);
	m_destructionBonusText->Init(L"");
	m_destructionBonusText->SetText(DESTRUCTION_BONUS_TEXT);
	m_destructionBonusText->SetPivot({ 0.5f,0.5f });

	//���j�����l����̑��Έʒu
	Vector2 destructionBonusTextPos = {
		m_destructionRateNum->GetPosition().x + RELATIVE_DISTANCE_TEXT.x -80.0f ,
		m_destructionRateNum->GetPosition().y + RELATIVE_DISTANCE_TEXT.y
	};

	m_destructionBonusText->SetPosition(destructionBonusTextPos);
	m_destructionBonusText->SetColor(TEXT_INIT_COLOR);
	m_destructionBonusText->SetScale(1.0f);

	//���j���{�[�i�X�̐��l��������
	m_destructionBonusNum = NewGO<FontRender>(0);
	m_destructionBonusNum->Init(L"");
	m_destructionBonusNum->SetPivot({ 0.5f,0.5f });

	//���j���{�[�i�X�̕������牡�ɂ��炷
	Vector2 destructionBonusNumPos = {
		m_destructionBonusText->GetPosition().x + RELATIVE_DISTANCE_TEXT.x + 320.0f ,
		m_destructionBonusText->GetPosition().y + 0.0f
	};

	m_destructionBonusNum->SetPosition(destructionBonusNumPos);
	m_destructionBonusNum->SetColor(TEXT_INIT_COLOR);
	m_destructionBonusNum->SetScale(1.0f);

	//�ŏI�X�R�A�̐��l��������
	m_finalScoreNum = NewGO<FontRender>(0);
	m_finalScoreNum->Init(L"");
	m_finalScoreNum->SetPivot({ 0.5f,0.5f });

	//final score�X�v���C�g����̑��Έʒu
	Vector2 finalScoreNumPos = {
		m_finalScoreTextSprite->GetPosition().x + RELATIVE_DISTANCE_TEXT.x - 50.0f ,
		m_finalScoreTextSprite->GetPosition().y + RELATIVE_DISTANCE_TEXT.y + 20.0f
	};

	m_finalScoreNum->SetPosition(finalScoreNumPos);
	m_finalScoreNum->SetColor(TEXT_INIT_COLOR);
	m_finalScoreNum->SetScale(1.0f);

	////////////////////////////////////////////////////////////////////////////////

	//�t�F�[�h�A�E�g�p�X�v���C�g�̏�����
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
	//�X�v���C�g�̃t�F�[�h�C������
	//���U���g�̕����X�v���C�g���t�F�[�h�C��
	m_resultSprite->FadeIn(TEXT_SPRITE_FADEIN_RATE);

	//�w�i���Â����邽�߂̃X�v���C�g�͓r���Ńt�F�[�h�C�����~�߂�
	if (m_screenSprite->GetColor().w < 0.85f) {
		m_screenSprite->FadeIn(SCREEN_SPRITE_FADEIN_RATE);
	}

	//���ڂ��ꂼ��̃X�v���C�g���t�F�[�h�C��
	//���U���g�̕������t�F�[�h�C�����I�������t�F�[�h�C���J�n
	if (m_resultSprite->GetColor().w >= 1.0f) {
		m_remainPlayerTextSprite->FadeIn(TEXT_SPRITE_FADEIN_RATE);
	}
	//�O�̍��ڂ��t�F�[�h�C������t�F�[�h�C���J�n
	if (m_remainPlayerTextSprite->GetColor().w >= 1.0f) {
		m_destructionRateTextSprite->FadeIn(TEXT_SPRITE_FADEIN_RATE);
	}
	//�O�̍��ڂ��t�F�[�h�C������t�F�[�h�C���J�n
	if (m_destructionRateTextSprite->GetColor().w >= 1.0f) {
		m_finalScoreTextSprite->FadeIn(TEXT_SPRITE_FADEIN_RATE);
	}

	//final score�̃X�v���C�g���t�F�[�h�C��������X�v���C�g�̃t�F�[�h�C��������
	if (m_finalScoreTextSprite->GetColor().w >= 1.0f) {
		m_isFinishFadeInSprite = true;
	}

}

void Result::FadeInText()
{
	//�c�@�̕\��
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

	//�c�@�{�[�i�X�����̕\��
	if (m_remainPlayerBonusText->GetColor().w < 1.0f) {
		//�c�@���̃t�F�[�h�C�����I����Ă�����t�F�[�h�C���J�n
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

	//�c�@�{�[�i�X�l�̕\��
	int remainBonus = GameDirector::GetInstance()->CalcRemainBonus();
	m_remainPlayerBonusNumWs = std::to_wstring(remainBonus);
	//�����̌��pts.������
	m_remainPlayerBonusNumWs += TEXT_PTS;
	const wchar_t* remainBonusWc = m_remainPlayerBonusNumWs.c_str();
	m_remainPlayerBonusNum->SetText(remainBonusWc);

	//�c�@�{�[�i�X�����̃t�F�[�h�C�����I����Ă�����t�F�[�h�C���J�n
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

	//�G���j���̕\��
	float destructionRate = GameDirector::GetInstance()->CalcDestructionRate();
	std::wstringstream dRwss;
	dRwss.precision(4);
	dRwss << destructionRate;

	m_destructionRateWs = dRwss.str();
	//�����̌��%������
	m_destructionRateWs += TEXT_PERCENT;
	const wchar_t* destructionRateWc = m_destructionRateWs.c_str();
	m_destructionRateNum->SetText(destructionRateWc);

	//�c�@�{�[�i�X�����̃t�F�[�h�C�����I����Ă�����t�F�[�h�C���J�n
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

	//���j���{�[�i�X�̕����̕\��

	//�c�@�{�[�i�X���l�̃t�F�[�h�C�����I����Ă�����t�F�[�h�C���J�n
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

	//���j���{�[�i�X�̒l�̕\��
	int destructionBonus = GameDirector::GetInstance()->CalcDestructionBonus();
	m_destructionBonusWs = std::to_wstring(destructionBonus);
	//�����̌��pts.������
	m_destructionBonusWs += TEXT_PTS;
	const wchar_t* destructionBonusWc = m_destructionBonusWs.c_str();
	m_destructionBonusNum->SetText(destructionBonusWc);

	//�c�@�{�[�i�X�����̃t�F�[�h�C�����I����Ă�����t�F�[�h�C���J�n
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

	//�ŏI�X�R�A�̒l�̕\��
	int finalScore = GameDirector::GetInstance()->CalcFinalScore();
	m_finalScoreNumWs = std::to_wstring(finalScore);
	//�����̌��pts.������
	m_finalScoreNumWs += TEXT_PTS;
	const wchar_t* finalScoreWc = m_finalScoreNumWs.c_str();
	m_finalScoreNum->SetText(finalScoreWc);

	//���j���{�[�i�X�l�̃t�F�[�h�C�����I����Ă�����t�F�[�h�C���J�n
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

	//�ŏI�X�R�A���l���t�F�[�h�C�����I�������e�L�X�g�̃t�F�[�h�C���͊���
	if (m_finalScoreNum->GetColor().w >= 1.0f) {
		m_isFinishFadeInText = true;
	}

}

void Result::FadeOutText(const float fadeOutRate)
{
	//�c�@��
	m_remainPlayerNum->SetColor(
		{
			m_remainPlayerNum->GetColor().x - fadeOutRate,
			m_remainPlayerNum->GetColor().y - fadeOutRate,
			m_remainPlayerNum->GetColor().z - fadeOutRate,
			m_remainPlayerNum->GetColor().w - fadeOutRate
		}
	);
	//�c�@�{�[�i�X����
	m_remainPlayerBonusText->SetColor(
		{
			m_remainPlayerBonusText->GetColor().x - fadeOutRate,
			m_remainPlayerBonusText->GetColor().y - fadeOutRate,
			m_remainPlayerBonusText->GetColor().z - fadeOutRate,
			m_remainPlayerBonusText->GetColor().w - fadeOutRate
		}
	);
	//�c�@�{�[�i�X�l
	m_remainPlayerBonusNum->SetColor(
		{
			m_remainPlayerBonusNum->GetColor().x - fadeOutRate,
			m_remainPlayerBonusNum->GetColor().y - fadeOutRate,
			m_remainPlayerBonusNum->GetColor().z - fadeOutRate,
			m_remainPlayerBonusNum->GetColor().w - fadeOutRate
		}
	);
	//���j��
	m_destructionRateNum->SetColor(
		{
			m_destructionRateNum->GetColor().x - fadeOutRate,
			m_destructionRateNum->GetColor().y - fadeOutRate,
			m_destructionRateNum->GetColor().z - fadeOutRate,
			m_destructionRateNum->GetColor().w - fadeOutRate
		}
	);
	//���j���{�[�i�X����
	m_destructionBonusText->SetColor(
		{
			m_destructionBonusText->GetColor().x - fadeOutRate,
			m_destructionBonusText->GetColor().y - fadeOutRate,
			m_destructionBonusText->GetColor().z - fadeOutRate,
			m_destructionBonusText->GetColor().w - fadeOutRate
		}
	);
	//���j���{�[�i�X�l
	m_destructionBonusNum->SetColor(
		{
			m_destructionBonusNum->GetColor().x - fadeOutRate,
			m_destructionBonusNum->GetColor().y - fadeOutRate,
			m_destructionBonusNum->GetColor().z - fadeOutRate,
			m_destructionBonusNum->GetColor().w - fadeOutRate
		}
	);
	//�ŏI�X�R�A�l
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
			//�X�v���C�g�̃t�F�[�h�C��
			FadeInSprite();

			/////////////////////////////////////////////////////////////////////////
			//�e�L�X�g�̕\��
			//�����X�v���C�g�̃t�F�[�h�C�����I����Ă�����J�n
			if (m_isFinishFadeInSprite == true) {
				FadeInText();
			}
			/////////////////////////////////////////////////////////////////////////
		}

		/////////////////////////////////////////////////////////////////////////
		//�^�C�g����ʂւ̑J��
		//�{�^���������ƃt�F�[�h�A�E�g�J�n
		if (g_pad[0]->IsTrigger(enButtonA) && m_isFinishFadeInText == true) {
			m_exitFlag = true;
		}
		if (m_exitFlag == true) {
			//�t�F�[�h�A�E�g(���X�v���C�g���t�F�[�h�C��)
			m_screenTopSprite->FadeIn(0.01f);

			FadeOutText(0.01f);

		}
		//�t�F�[�h�A�E�g����������
		if (m_screenTopSprite->GetColor().w >= 1.0f) {
			
			//�Q�[�����폜���A�^�C�g����NewGO������
			QueryGOs<Game>("game", [&](Game* gameScene) {

				//�Q�[�����폜
				DeleteGO(gameScene);

				//�e���폜
				BulletManager::GetInstance()->DeleteBullets();
				//�������폜
				ExplosionManager::GetInstance()->DeleteExplosions();
				//�������ꂽ�G�l�~�[���폜
				EnemyManager::GetInstance()->DeleteEnemies();

				//�^�C�g����NewGO
				NewGO<Title>(0, "title");

				//�₢���킹�I��
				return false;
			});
		}

	}
}

