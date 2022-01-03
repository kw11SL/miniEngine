#pragma once
class Life
{
public:
	Life();
	~Life();
	
	/// @brief ���������� 
	void Init();

	/// @brief ���C�t�A�C�R���̕\���A��\��
	void LifeDisp();

	/// @brief �����𖾖ł����鏈��
	void VariableHalo();

	/// @brief �����X�v���C�g�������ɂ��Č����Ȃ����鏈��
	void HaloVanish();

	/// @brief �����X�v���C�g�̕\���ʒu�����肷�鏈��
	void HaloDisp();

	/// @brief �X�V����
	void Update();

private:
	SpriteRender* m_lifeTextSprite = nullptr;				//���C�t�̕����X�v���C�g
	Vector3 m_lifeTextSpritePos = Vector3::Zero;			//���C�t�̕����X�v���C�g�̈ʒu

	SpriteRender* m_lifeIconSprite[3] = { nullptr };		//���C�t�A�C�R���̔z��
	Vector3 m_lifeIconSpritePos[3];							//���C�t�A�C�R���̈ʒu

	SpriteRender* m_lifeIconFrame[3] = { nullptr };
	Vector3 m_lifeIconFramePos[3];

	SpriteRender* m_lifeIconHalo = nullptr;					//�����X�v���C�g
	Vector4 m_lifeIconHaloColor = { 1.0f,1.0f,1.0f,1.0f };	//�����X�v���C�g�̃J���[
	float m_lifeIconHaloColorRate = 0.0f;					//�����X�v���C�g�̔�����ԗ�
	bool m_lifeIconHaloMaxFlag = false;						//���邳�̐܂�Ԃ��t���O
	bool m_isHaloFadeOutReady = false;						//�X�v���C�g�������鏀���t���O
	Vector3 m_lifeIconHaloPos = Vector3::Zero;				//�����X�v���C�g�̈ʒu
	Vector3 m_lifeIconHaloScale = Vector3::One;				//�����X�v���C�g�̊g�嗦
	
	int m_prevPlayerLife = 0;								//�O�t���[���̃v���C���[�̃��C�t
};

