#pragma once
class WaveCutIn
{
public:
	WaveCutIn(){}
	~WaveCutIn();

	/// @brief ����������
	void Init();

	/// @brief wave���ɉ������X�v���C�g�̐؂�ւ�
	void SwitchingSprite();

	/// @brief �X�v���C�g�̈ړ�����
	void SpriteMove();

	//�X�v���C�g�̃t�F�[�h�C���ƃt�F�[�h�A�E�g����
	void SpriteFade();

	/// @brief �X�V����
	void Update();
private:
	SpriteRender* m_waveSprite = nullptr;		//�J�b�g�C�����Ă���X�v���C�g

	Vector3 m_waveSpritePos = Vector3::Zero;	//�X�v���C�g�̏����ʒu
	float m_waveSpriteAlpha = 0.0f;				//�X�v���C�g�̕s�����x
	
	float m_waveSpriteMoveFraction = 0.0f;		//�X�v���C�g�̈ړ��p��ԗ�
	float m_waveSpriteWaitCounter = 0.0f;		//�X�v���C�g�������Œ�~���鎞��

	bool m_isValidMove = true;					//�ړ��\���ǂ����t���O
	bool m_isFinishMoveToCenter = false;		//�����Ɉړ��������ǂ����t���O

	bool m_isStart = false;						//�Q�[���J�n�t���O
};

