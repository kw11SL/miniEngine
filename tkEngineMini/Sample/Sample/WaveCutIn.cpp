#include "stdafx.h"
#include "WaveCutIn.h"

namespace {

	//wave�ő吔
	const int WAV_NUM_MAX = 9;

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
	//�t�F�[�h�C���A�A�E�g�֘A
	const float FADEIN_RATE = 0.06f;	//�t�F�[�h�C���̊���
	const float FADEOUT_RATE = 0.06f;	//�t�F�[�h�A�E�g�̊���

	//SE�֘A
	//�J�b�g�C������SE
	const wchar_t* SE_CUTIN_FILEPATH = L"Assets/wav/wave_change_2.wav";	//�t�@�C���p�X
	const float SE_CUTIN_VOLUME = 1.0f;									//�{�����[��
}

WaveCutIn::~WaveCutIn()
{
	//�ϒ��z����̃X�v���C�g�ƃI�u�W�F�N�g���g���폜
	for (auto& sCutinSprite : m_cutinSprites) {
		//�X�v���C�g���폜
		DeleteGO(sCutinSprite->cutinSprite);

		delete sCutinSprite;
		sCutinSprite = nullptr;
	}
}

void WaveCutIn::Init()
{
	//�X�v���C�g�̍쐬
	for (int i = 0; i < WAV_NUM_MAX; i++) {
		
		//wave������xstring�^�ɕϊ�
		std::string numStr = std::to_string(i+1);
		//string�^�Ńt�@�C���p�X���\�����邽�߁A�p�X + (wave��) + �g���q�����ŕ�������������
		std::string convFilePath = FILEPATH_BASE_1 + numStr + FILEPATH_BASE_2;
		//�ł���string�^�̃t�@�C���p�X��const char*�^�ɕϊ�
		const char* newFilePath = convFilePath.c_str();

		//�J�b�g�C������X�v���C�g���ϒ��z����ɍ쐬
		m_cutinSprites.push_back(new SCutinSprite);

		//�X�v���C�g���쐬
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite = NewGO<SpriteRender>(0);
		//�쐬�������̂�������
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->Init(
			newFilePath,
			WAVE_SPRITE_WIDTH,
			WAVE_SPRITE_HEIGHT,
			AlphaBlendMode_Trans
		);

		//�s�{�b�g�A�g�嗦�A�ʒu�A�F��ݒ�
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->SetPivot(SPRITE_PIVOT);
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->SetScale(WAVE_SPRITE_SCALE);
		m_cutinSprites[m_cutinSprites.size() - 1]->pos = WAVE_SPRITE_INIT_POS;
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->SetPosition(m_cutinSprites[m_cutinSprites.size() - 1]->pos);
		m_cutinSprites[m_cutinSprites.size() - 1]->cutinSprite->SetColor(WAVE_SPRITE_INIT_COLOR);

		//�����Œ�~���鎞�Ԃ�ݒ�
		m_cutinSprites[m_cutinSprites.size() - 1]->waitCounter = 1.0f;
	}
}

void WaveCutIn::SpriteMove(SCutinSprite* sCutinSprite)
{
	//�����̍\���̂�null�������珈�����Ȃ�
	if (sCutinSprite == nullptr) {
		return;
	}

	//�ړ��\�Ȃ�ړ��p�̕�ԗ����㏸
	if (sCutinSprite->isValidMove == true) {
		sCutinSprite->moveFraction += 0.025f;
	}

	//���Ԓn�_�Œ�~
	if (sCutinSprite->moveFraction > 0.5f
		&& sCutinSprite->isMoveFinishToCenter == false) {
	
		sCutinSprite->moveFraction = 0.5f;

		//�����Ɉړ������t���O���I��
		sCutinSprite->isMoveFinishToCenter = true;
		//�ړ��\�t���O���I�t
		sCutinSprite->isValidMove = false;
	}

	//�����Ɉړ����������Ă�����A��~�J�E���^�������A
	if (sCutinSprite->isMoveFinishToCenter == true) {
		sCutinSprite->waitCounter -= g_gameTime->GetFrameDeltaTime();
	}

	//��~�J�E���^��0�ȉ��ɂȂ�����ēx�A�ړ��\�ɂ���
	if (sCutinSprite->waitCounter <= 0.0f) {
		sCutinSprite->waitCounter = 0.0f;
		
		if (sCutinSprite->isValidMove == false) {
			sCutinSprite->isValidMove = true;
		}
	}

	//�E�[�܂ňړ�������
	if (sCutinSprite->moveFraction >= 1.0f) {
		
		//�ړ������������̂ŃJ�E���^�[�ƃt���O�����Z�b�g
		sCutinSprite->moveFraction = 0.0f;
		sCutinSprite->isMoveFinishToCenter = false;
		sCutinSprite->waitCounter = 1.0f;
		sCutinSprite->isValidMove = false;
	}

	//�ʒu����`�⊮
	sCutinSprite->pos.Lerp(
		sCutinSprite->moveFraction,
		WAVE_SPRITE_INIT_POS,
		WAVE_SPRITE_END_POS
	);

	//�ʒu���X�V
	sCutinSprite->cutinSprite->SetPosition(sCutinSprite->pos);
}

void WaveCutIn::SpriteFade(SCutinSprite* sCutinSprite)
{
	//�����̍\���̂�null�������珈�����Ȃ�
	if (sCutinSprite == nullptr) {
		return;
	}

	//�ړ��\����~�J�E���^�[���㏸���Ȃ�
	if (sCutinSprite->waitCounter > 0.0f
		&& sCutinSprite->isValidMove == true) {
		
		//�t�F�[�h�C��
		sCutinSprite->cutinSprite->FadeIn(FADEIN_RATE);
	}
	//�ړ��\����~�J�E���^�[��0�ȉ��Ȃ�(=��~�������I����Ă�����)
	else if (sCutinSprite->waitCounter <= 0.0f
		&& sCutinSprite->isValidMove == true) {

		//�t�F�[�h�A�E�g
		sCutinSprite->cutinSprite->FadeOut(FADEOUT_RATE);
	}
}


void WaveCutIn::Update()
{
	//�Q�[�����ȊO�Ȃ珈�����Ȃ�
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//���݂�wave�����擾
	int waveNum = GameDirector::GetInstance()->GetWaveNumber();
	//�X�V�Ώۂ̔z��ԍ���ݒ�
	//��Fwave1�̂Ƃ��z��0�Ԗڂ̃X�v���C�g���X�V
	int updateTargetNum = waveNum - 1;

	//�Q�[�����J�n���A�Q�[���J�n�t���O���I�t�Ȃ�
	if (GameDirector::GetInstance()->GetGameState() == enGame 
		&& m_isStart == false) {
		
		//se���Đ�
		CSoundSource* ssCutInSe = NewGO<CSoundSource>(0);
		ssCutInSe->Init(SE_CUTIN_FILEPATH);
		ssCutInSe->SetVolume(SE_CUTIN_VOLUME);
		ssCutInSe->Play(false);

		//�J�n�t���O���I��
		m_isStart = true;
	}

	//wave���؂�ւ���Ă�����
	if (GameDirector::GetInstance()->GetIsSwitchedWave()) {
		
		//se���Đ�
		CSoundSource* ssCutInSe = NewGO<CSoundSource>(0);
		ssCutInSe->Init(SE_CUTIN_FILEPATH);
		ssCutInSe->SetVolume(SE_CUTIN_VOLUME);
		ssCutInSe->Play(false);

	}

	//�X�V�Ώۂ̃J�b�g�C���X�v���C�g�̈ړ�
	SpriteMove(m_cutinSprites[updateTargetNum]);
	//�X�V�Ώۂ̃J�b�g�C���X�v���C�g�̃t�F�[�h����
	SpriteFade(m_cutinSprites[updateTargetNum]);

	//���t���[����wave�����L�^
	m_waveNumPrevFrame = waveNum;
}