#include "stdafx.h"
#include "Wave.h"

namespace {
	//�t�@�C���p�X��
	const char* TEXT_SPRITE_FILEPATH = "Assets/sprite/ui/wave.dds";							//wave�̕����X�v���C�g�̃t�@�C���p�X
	const char* NUM_SPRITE_FILEPATH = "Assets/sprite/ui/1.dds";								//wave���̃X�v���C�g�̃t�@�C���p�X
	const char* NUM_SPRITE_FRAME_FILEPATH = "Assets/sprite/ui/weaponIcon_frame.dds";		//wave���̘g�̃X�v���C�g

	//wave���X�v���C�g�������p�t�@�C���p�X�̑O���Ɗg���q����
	const std::string FILEPATH_BASE_1 = "Assets/sprite/ui/";
	const std::string FILEPATH_BASE_2 = ".dds";
	
	//�����ʒu
	const Vector3 TEXT_SPRITE_POS = { -580.0f,320.0f,0.0f };
	const Vector3 TEXT_SPRITE_SHADOW_OFFSET = { 7.0f,-7.0f,0.0f };
	const Vector3 NUM_SPRITE_INIT_POS = { -520.0f,280.0f,0.0f };
	const Vector3 NUM_SPRITE_SHADOW_OFFSET = { 7.0f,-7.0f,0.0f };
	
	//�g�嗦�Ɗg�嗦�W��
	const Vector3 TEXT_SPRITE_SCALE = Vector3::One;
	const float TEXT_SCALE_RATE = 0.3f;
	const Vector3 NUM_SPRITE_SCALE = Vector3::One;
	const float NUM_SCALE_RATE = 0.5f;
	const float NUM_FRAME_SCALE_RATE = 0.35f;
	
	//�s�{�b�g
	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };
	
	//�J���[
	const Vector4 TEXT_SPRITE_INIT_COLOR = { 0.0f,1.0f,0.85f,1.0f };
	const Vector4 TEXT_SPRITE_SHADOW_INIT_COLOR = { 0.0f*0.3f,1.0f*0.3f,0.85f*0.3f,1.0f*0.7f };
	const Vector4 NUM_SPRITE_INIT_COLOR = { 0.0f,1.0f,0.85f,0.0f };
	const Vector4 NUM_SPRITE_INIT_SHADOW_COLOR = { 0.0f*0.3f,1.0f*0.3f,0.85f*0.3f,0.0f };
	
	//�X�v���C�g�̕��ƍ���
	const int TEXT_SPRITE_WIDTH = 256;
	const int TEXT_SPRITE_HEIGHT = 128;
	const int NUM_SPRITE_WIDTH = 256;
	const int NUM_SPRITE_HEIGHT = 256;
}

Wave::~Wave()
{
	DeleteGO(m_waveTextSprite);
	DeleteGO(m_waveTextShadowSprite);
	DeleteGO(m_waveNumSprite);
	DeleteGO(m_waveNumShadowSprite);
}

void Wave::Init()
{
	/////////////////////////////////////////////////////////////
	//�����X�v���C�g�̏�����
	//wave�̕����̉e�X�v���C�g�̏�����
	m_waveTextShadowSprite = NewGO<SpriteRender>(0);
	m_waveTextShadowSprite->Init(
		TEXT_SPRITE_FILEPATH,
		TEXT_SPRITE_WIDTH,
		TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveTextShadowSprite->SetPivot(SPRITE_PIVOT);
	m_waveTextShadowSprite->SetScale(TEXT_SPRITE_SCALE * TEXT_SCALE_RATE);
	//������Ƃ��炷
	m_waveTextShadowSprite->SetPosition(TEXT_SPRITE_POS + TEXT_SPRITE_SHADOW_OFFSET);
	m_waveTextShadowSprite->SetColor(TEXT_SPRITE_SHADOW_INIT_COLOR);

	//wave�̕����X�v���C�g�̏�����
	m_waveTextSprite = NewGO<SpriteRender>(0);
	m_waveTextSprite->Init(
		TEXT_SPRITE_FILEPATH,
		TEXT_SPRITE_WIDTH,
		TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveTextSprite->SetPivot(SPRITE_PIVOT);
	m_waveTextSprite->SetScale(TEXT_SPRITE_SCALE * TEXT_SCALE_RATE);
	m_waveTextSprite->SetPosition(TEXT_SPRITE_POS);
	m_waveTextSprite->SetColor(TEXT_SPRITE_INIT_COLOR);
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	//wave���X�v���C�g�̏�����
	//�X�v���C�g�̊g�嗦�W�����L�^
	m_numSpriteScaleRate = NUM_SCALE_RATE;

	//wave���̉e�X�v���C�g�̏�����
	m_waveNumShadowSprite = NewGO<SpriteRender>(0);
	//wave���X�v���C�g�̏�����
	m_waveNumSprite = NewGO<SpriteRender>(0);

	//wave������X�v���C�g�����肵�ď���������
	SwitchingWaveSprite();

	/////////////////////////////////////////////////////////////
}

void Wave::SwitchingWaveSprite()
{
	//���݂�wave�����擾
	int waveNum = GameDirector::GetInstance()->GetWaveNumber();
	//wave������xstring�^�ɕϊ�
	std::string numStr = std::to_string(waveNum);

	//string�^�Ńt�@�C���p�X���\�����邽�߁A�p�X + (wave��) + �g���q�����ŕ�������������
	std::string convFilePath = FILEPATH_BASE_1 + numStr + FILEPATH_BASE_2;
	//�ł���string�^�̃t�@�C���p�X��const char*�^�ɕϊ�
	const char* newFilePath = convFilePath.c_str();

	//�g�嗦�ϐ������Z�b�g
	m_numSpriteScaleRate = NUM_SCALE_RATE;

	//�ϊ������t�@�C���p�X�ŏ�����
	//�e�X�v���C�g�̏�����
	m_waveNumShadowSprite->Init(
		newFilePath,
		NUM_SPRITE_WIDTH,
		NUM_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveNumShadowSprite->SetPivot(SPRITE_PIVOT);
	m_waveNumShadowSprite->SetScale(NUM_SPRITE_SCALE * m_numSpriteScaleRate);
	m_waveNumShadowSprite->SetPosition(NUM_SPRITE_INIT_POS + NUM_SPRITE_SHADOW_OFFSET);
	m_waveNumShadowSprite->SetColor(NUM_SPRITE_INIT_SHADOW_COLOR);

	//�{�̃X�v���C�g�̏�����
	m_waveNumSprite->Init(
		newFilePath,
		NUM_SPRITE_WIDTH,
		NUM_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	m_waveNumSprite->SetPivot(SPRITE_PIVOT);
	m_waveNumSprite->SetScale(NUM_SPRITE_SCALE * m_numSpriteScaleRate);
	m_waveNumSprite->SetPosition(NUM_SPRITE_INIT_POS);
	m_waveNumSprite->SetColor(NUM_SPRITE_INIT_COLOR);
}

void Wave::FadeinWithScalingWaveSprite()
{
	//�t�F�[�h�C��
	if (m_waveNumSprite->GetColor().w < 1.0f) {
		m_waveNumSprite->FadeIn(0.025f);
		m_waveNumShadowSprite->FadeIn(0.025f);
	}
	
	//�X�v���C�g�̊g�嗦�W���������Ă����A���l�ŃX�g�b�v
	m_numSpriteScaleRate -= 0.03f;
	if (m_numSpriteScaleRate <= 0.25) {
		m_numSpriteScaleRate = 0.25;
	}
	
	//�k��
	//�g�嗦�W�����|���ăX�P�[�����O
	m_waveNumSprite->SetScale(Vector3::One * m_numSpriteScaleRate);
	m_waveNumShadowSprite->SetScale(Vector3::One * m_numSpriteScaleRate);
}

void Wave::Update()
{
	//���̃t���[���Ńf�B���N�^�[��wave����؂�ւ��Ă�����wave���X�v���C�g��؂�ւ�
	if (GameDirector::GetInstance()->GetIsSwitchedWave() == true) {
		SwitchingWaveSprite();
	}
	
	//wave�����t�F�[�h�C�����Ȃ���k�����\������
	FadeinWithScalingWaveSprite();

}