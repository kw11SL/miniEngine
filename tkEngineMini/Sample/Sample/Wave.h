#pragma once
class Wave
{
public:
	/// @brief �����X�v���C�g�p�̍\����
	struct SNumSprite {
		SpriteRender* numSprite = nullptr;			//�����X�v���C�g
		SpriteRender* numShadowSprite = nullptr;	//�e�X�v���C�g
		float spriteScaleRate = 0.5f;				//�g�嗦
	};


	Wave(){}
	~Wave();

	/// @brief ����������
	void Init();

	/// @brief wave���X�v���C�g���t�F�[�h�C�����Ȃ���k�����Ă��鏈��
	/// @param sNumSprite �X�v���C�g�̍\����
	void FadeinWithScalingWaveSprite(SNumSprite* sNumSprite);

	/// @brief wave���X�v���C�g�𓧖��ɂ���(=�����Ȃ�����)����
	/// @param sNumSprite �X�v���C�g�̍\����
	void NumSpriteClear(SNumSprite* sNumSprite);

	/// @brief �X�V����
	void Update();

private:
	std::vector<SNumSprite*> m_numSprites;				//�����̍\���̂��i�[����ϒ��z��

	SpriteRender* m_waveTextSprite = nullptr;			//wave�̕����X�v���C�g
	SpriteRender* m_waveTextShadowSprite = nullptr;		//wave�̕����̉e�X�v���C�g
	
	float m_numSpriteScaleRate = 0.0f;					//wave���X�v���C�g�̊g�嗦
	int m_waveNumPrevFrame = 1;							//�O�t���[����wave�����擾
};

