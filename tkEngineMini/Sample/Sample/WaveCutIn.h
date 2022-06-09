#pragma once
class WaveCutIn
{
public:
	/// @brief �J�b�g�C������X�v���C�g�̍\����
	struct SCutinSprite {
		SpriteRender* cutinSprite = nullptr;	//�J�b�g�C������X�v���C�g
		Vector3 pos = Vector3::Zero;			//�ʒu
		float alpha = 0.0f;						//�����x�W��
		float moveFraction = 0.0f;				//�ړ��̕�ԗ�
		float waitCounter = 0.0f;				//�ړ��J�E���^
		bool isValidMove = true;				//�ړ��\�t���O
		bool isMoveFinishToCenter = false;		//�����Ɉړ��������ǂ����̃t���O
	};


	WaveCutIn(){}
	~WaveCutIn();

	/// @brief ����������
	void Init();

	/// @brief �X�v���C�g�̈ړ�����
	/// @param sCutinSprite �J�b�g�C���X�v���C�g�̍\����
	void SpriteMove(SCutinSprite* sCutinSprite);

	/// @brief �X�v���C�g�̃t�F�[�h�C���A�A�E�g����
	/// @param sCutinSprite �J�b�g�C���X�v���C�g�̍\����
	void SpriteFade(SCutinSprite* sCutinSprite);

	/// @brief �X�V����
	void Update();
private:

	std::vector<SCutinSprite*> m_cutinSprites;	//�J�b�g�C���X�v���C�g�̉ϒ��z��
	int m_waveNumPrevFrame = 1;					//�O�t���[����wave��
	bool m_isStart = false;						//�Q�[���J�n�t���O
};

