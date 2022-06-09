#include "stdafx.h"
#include "Wave.h"

namespace {

	//wave���̍ő�l�B�X�v���C�g�̏������Ɏg�p�B
	const int WAVE_NUM_MAX = 9;

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
	//�����X�v���C�g�̍폜
	//�ϒ��z��̃X�v���C�g���폜���A�\���̎��̂��폜����
	for (auto& sNumSprites : m_numSprites) {
		DeleteGO(sNumSprites->numSprite);
		DeleteGO(sNumSprites->numShadowSprite);

		delete sNumSprites;
		sNumSprites = nullptr;
	}
	
	//�����X�v���C�g���폜
	DeleteGO(m_waveTextSprite);
	DeleteGO(m_waveTextShadowSprite);
}

void Wave::Init()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�����X�v���C�g�̏�����
	
	//wave�̕����̉e�X�v���C�g�̏�����
	m_waveTextShadowSprite = NewGO<SpriteRender>(0);
	m_waveTextShadowSprite->Init(
		TEXT_SPRITE_FILEPATH,
		TEXT_SPRITE_WIDTH,
		TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//�s�{�b�g�A�g�嗦�A�ʒu�A�F��ݒ�
	m_waveTextShadowSprite->SetPivot(SPRITE_PIVOT);
	m_waveTextShadowSprite->SetScale(TEXT_SPRITE_SCALE * TEXT_SCALE_RATE);
	//������Ƃ��炷
	m_waveTextShadowSprite->SetPosition(TEXT_SPRITE_POS + TEXT_SPRITE_SHADOW_OFFSET);
	m_waveTextShadowSprite->SetColor(TEXT_SPRITE_SHADOW_INIT_COLOR);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//wave�̕����X�v���C�g�̏�����
	m_waveTextSprite = NewGO<SpriteRender>(0);
	m_waveTextSprite->Init(
		TEXT_SPRITE_FILEPATH,
		TEXT_SPRITE_WIDTH,
		TEXT_SPRITE_HEIGHT,
		AlphaBlendMode_Trans
	);

	//�s�{�b�g�A�g�嗦�A�ʒu�A�F��ݒ�
	m_waveTextSprite->SetPivot(SPRITE_PIVOT);
	m_waveTextSprite->SetScale(TEXT_SPRITE_SCALE * TEXT_SCALE_RATE);
	m_waveTextSprite->SetPosition(TEXT_SPRITE_POS);
	m_waveTextSprite->SetColor(TEXT_SPRITE_INIT_COLOR);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�����X�v���C�g�̗p��
	//�ϒ��z����ɐ����X�v���C�g�̍\���̂��쐬���Ă���
	for (int i = 0; i < WAVE_NUM_MAX; i++) {
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// �t�@�C���p�X���쐬
		std::string numStr = std::to_string(i);
		//string�^�Ńt�@�C���p�X���\�����邽�߁A�p�X + (wave��) + �g���q�����ŕ�������������
		std::string convFilePath = FILEPATH_BASE_1 + numStr + FILEPATH_BASE_2;
		//�ł���string�^�̃t�@�C���p�X��const char*�^�ɕϊ�
		const char* newFilePath = convFilePath.c_str();

		//�ϒ��z����ɍ\���̂��쐬
		m_numSprites.push_back(new SNumSprite);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//�����̉e�X�v���C�g�̏�����
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite = NewGO<SpriteRender>(0);
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->Init(
			newFilePath,
			NUM_SPRITE_WIDTH,
			NUM_SPRITE_HEIGHT,
			AlphaBlendMode_Trans
		);

		//�g�嗦�ϐ������Z�b�g
		m_numSprites[m_numSprites.size() - 1]->spriteScaleRate = NUM_SCALE_RATE;
		
		//�s�{�b�g�A�g�嗦�A�ʒu�A�F��ݒ�
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->SetPivot(SPRITE_PIVOT);
		//������Ƃ��炷
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->SetPosition(NUM_SPRITE_INIT_POS + NUM_SPRITE_SHADOW_OFFSET);
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->SetScale(NUM_SPRITE_SCALE * m_numSprites[m_numSprites.size() - 1]->spriteScaleRate);
		m_numSprites[m_numSprites.size() - 1]->numShadowSprite->SetColor(NUM_SPRITE_INIT_SHADOW_COLOR);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//�����X�v���C�g�̏�����
		m_numSprites[m_numSprites.size() - 1]->numSprite = NewGO<SpriteRender>(0);
		m_numSprites[m_numSprites.size() - 1]->numSprite->Init(
			newFilePath,
			NUM_SPRITE_WIDTH,
			NUM_SPRITE_HEIGHT,
			AlphaBlendMode_Trans
		);

		//�g�嗦�ϐ������Z�b�g
		m_numSprites[m_numSprites.size() - 1]->spriteScaleRate = NUM_SCALE_RATE;
		
		//�s�{�b�g�A�g�嗦�A�ʒu�A�F��ݒ�
		m_numSprites[m_numSprites.size() - 1]->numSprite->SetPivot(SPRITE_PIVOT);
		m_numSprites[m_numSprites.size() - 1]->numSprite->SetPosition(NUM_SPRITE_INIT_POS);
		m_numSprites[m_numSprites.size() - 1]->numSprite->SetScale(NUM_SPRITE_SCALE * m_numSprites[m_numSprites.size() - 1]->spriteScaleRate);
		m_numSprites[m_numSprites.size() - 1]->numSprite->SetColor(NUM_SPRITE_INIT_COLOR);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Wave::FadeinWithScalingWaveSprite(SNumSprite* sNumSprite)
{
	//�����̍\���̂�null�Ȃ珈�����Ȃ�
	if (sNumSprite == nullptr) {
		return;
	}

	//�����x��1�����Ȃ�t�F�[�h�C��
	if (sNumSprite->numSprite->GetColor().w < 1.0f) {
		sNumSprite->numSprite->FadeIn(0.025f);
		sNumSprite->numShadowSprite->FadeIn(0.025f);
	}

	//�X�v���C�g�̊g�嗦�W���������Ă����A���l�ŃX�g�b�v
	sNumSprite->spriteScaleRate -= 0.03f;
	if (sNumSprite->spriteScaleRate <= 0.25) {
		sNumSprite->spriteScaleRate = 0.25;
	}

	//�k��
	//�g�嗦�W�����|���ăX�P�[�����O
	sNumSprite->numSprite->SetScale(Vector3::One * sNumSprite->spriteScaleRate);
	sNumSprite->numShadowSprite->SetScale(Vector3::One * sNumSprite->spriteScaleRate);
}

void Wave::NumSpriteClear(SNumSprite* sNumSprite)
{
	//�����̍\���̂�null�Ȃ珈�����Ȃ�
	if (sNumSprite == nullptr) {
		return;
	}

	//�����X�v���C�g�A�e�X�v���C�g�Ƃ��ɓ����ɂ���
	sNumSprite->numSprite->SetColor({ 0.0f,0.0f,0.0f,0.0f });
	sNumSprite->numShadowSprite->SetColor({ 0.0f,0.0f,0.0f,0.0f });
}

void Wave::Update()
{
	//wave�����擾
	int waveNum = GameDirector::GetInstance()->GetWaveNumber();
	//���̃t���[���ōX�V����ϒ��z��̔z��ԍ�������
	int updateTargetNum = waveNum;
	
	//�t�F�[�h�C���Ək��
	FadeinWithScalingWaveSprite(m_numSprites[updateTargetNum]);

	//wave�����O�t���[���ƕς���Ă�����
	if (waveNum != m_waveNumPrevFrame) {
		//�ς��O�̃X�v���C�g�̐F�𓧖��ɂ���
		//��Fwave1 �� wave2 �������� wave1�̃X�v���C�g�𓧖��ɂ���
		NumSpriteClear(m_numSprites[updateTargetNum - 1]);
	}

	//���t���[����wave�����L�^
	m_waveNumPrevFrame = waveNum;
}