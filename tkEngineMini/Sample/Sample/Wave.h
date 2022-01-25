#pragma once
class Wave
{
public:
	Wave(){}
	~Wave();

	/// @brief ����������
	void Init();

	/// @brief ���݂�wave���Ɠ����X�v���C�g�ɐ؂�ւ��鏈��
	void SwitchingWaveSprite();

	/// @brief wave���X�v���C�g���t�F�[�h�C�����Ȃ���k�����Ă��鏈��
	void FadeinWithScalingWaveSprite();

	/// @brief �X�V����
	void Update();

private:
	SpriteRender* m_waveTextSprite = nullptr;			//wave�̕����X�v���C�g
	SpriteRender* m_waveTextShadowSprite = nullptr;		//wave�̕����̉e�X�v���C�g
	
	SpriteRender* m_waveNumSprite = nullptr;			//wave���̃X�v���C�g
	SpriteRender* m_waveNumShadowSprite = nullptr;		//wave���̉e�X�v���C�g

	float m_numSpriteScaleRate = 0.0f;					//wave���X�v���C�g�̊g�嗦
};

