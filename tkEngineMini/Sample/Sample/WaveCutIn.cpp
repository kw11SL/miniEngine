#include "stdafx.h"
#include "WaveCutIn.h"

namespace {
	//�t�@�C���p�X
	const char* WAVE_SPRITE_FILEPATH = "Assets/sprite/ui/wave1.dds";

	//wave���X�v���C�g�������p�t�@�C���p�X�̑O���Ɗg���q����
	const std::string FILEPATH_BASE_1 = "Assets/sprite/ui/wave";
	const std::string FILEPATH_BASE_2 = ".dds";

	//�X�v���C�g�̏����ʒu
	const Vector3 WAVE_SPRITE_INIT_POS = { 320,0.0f,0.0f };
	//�X�v���C�g�̍ŏI�ʒu
	const Vector3 WAVE_SPRITE_END_POS = { WAVE_SPRITE_INIT_POS.x * -1.0f,0.0f,0.0f };
	
	//�J���[
	const Vector4 WAVE_SPRITE_INIT_COLOR = { 1.0f,1.0f,1.0f,0.0f };
	
	//�g�嗦
	const Vector3 WAVE_SPRITE_SCALE = Vector3::One;
	const float WAVE_SPRITE_SCALE_RATE = 1.0f;

	//���A����
	const int WAVE_SPRITE_WIDTH = 512;
	const int WAVE_SPRITE_HEIGHT = 256;

	//�s�{�b�g
	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };
}

WaveCutIn::~WaveCutIn()
{
	DeleteGO(m_waveSprite);
}

void WaveCutIn::Init()
{
	//�X�v���C�g�̏�����
	m_waveSprite = NewGO<SpriteRender>(0);
	
	//���݂�wave���ɉ������X�v���C�g�ŏ�����
	SwitchingSprite();

	//�����Œ�~���鎞�Ԃ̃J�E���^��������
	m_waveSpriteWaitCounter = 1.0f;

}

void WaveCutIn::SwitchingSprite()
{
	//���݂�wave�����擾
	int waveNum = GameDirector::GetInstance()->GetWaveNumber();
	//wave������xstring�^�ɕϊ�
	std::string numStr = std::to_string(waveNum);

	//string�^�Ńt�@�C���p�X���\�����邽�߁A�p�X + (wave��) + �g���q�����ŕ�������������
	std::string convFilePath = FILEPATH_BASE_1 + numStr + FILEPATH_BASE_2;
	//�ł���string�^�̃t�@�C���p�X��const char*�^�ɕϊ�
	const char* newFilePath = convFilePath.c_str();

	//�X�v���C�g�̏�����
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
	////�e�X�g
	//if (m_isValidMove == false) {
	//	if (g_pad[0]->IsTrigger(enButtonA)) {
	//		m_isValidMove = true;
	//	}
	//}

	

	//�ړ��p�̕�ԗ����㏸
	if (m_isValidMove == true) {
		m_waveSpriteMoveFraction += 0.025f;
	}

	//���Ԓn�_�Œ�~
	if (m_waveSpriteMoveFraction > 0.5f && m_isFinishMoveToCenter == false) {
		m_waveSpriteMoveFraction = 0.5;

		m_isFinishMoveToCenter = true;
		m_isValidMove = false;
	}

	//�ړ��ĊJ�܂ł̃J�E���^���������A�J�E���^��0�ɂȂ�����ړ��\�ɂ���
	if (m_isFinishMoveToCenter == true) {
		m_waveSpriteWaitCounter -= g_gameTime->GetFrameDeltaTime();
	}
	if (m_waveSpriteWaitCounter <= 0.0f) {
		m_waveSpriteWaitCounter = 0.0f;
		if(m_isValidMove == false){
			m_isValidMove = true;
		}
	}

	//�E�[�܂ňړ�����
	if (m_waveSpriteMoveFraction >= 1.0f) {
		
		//�ړ������������̂ŃJ�E���^�[�ƃt���O�����Z�b�g
		m_waveSpriteMoveFraction = 0.0f;
		m_isFinishMoveToCenter = false;
		m_waveSpriteWaitCounter = 1.0f;
		m_isValidMove = false;
	}


	//�ʒu����`�⊮
	m_waveSpritePos.Lerp(
		m_waveSpriteMoveFraction,
		WAVE_SPRITE_INIT_POS,
		WAVE_SPRITE_END_POS
	);

	//�ʒu���X�V
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
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//wave���؂�ւ���Ă�����
	if (GameDirector::GetInstance()->GetIsSwitchedWave()) {
		//�X�v���C�g��؂�ւ���
		SwitchingSprite();

		//�ړ��\�t���O���I��
		m_isValidMove = true;
	}

	SpriteMove();
	
	SpriteFade();

	//m_waveSprite->FadeIn(0.03f);

}