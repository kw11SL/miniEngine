#pragma once
#include"Explosion.h"
class Player_new;

/// @brief �G�l�~�[�̃^�C�v
enum EnEnemyType
{
	enCommon,
	enPowered,
	enChaser,
	enBomb,
	enShot,
	enEnemyTypeNum
};

class Enemy : public IGameObject
{
private:
	

public:
	Enemy();
	~Enemy();
	
	/// @brief ����������
	/// @param renderingEngine 
	/// @param initPoint 
	/// @param initUp 
	/// @param enemyType 
	void Init(
		const Vector3& initPoint,
		const Vector3& initUp,
		const EnEnemyType& enemyType);

	//�Q�b�^�[
	/// @brief	���W���擾 
	/// @return ���W
	const Vector3& GetPosition() const { return m_position; }

	/// @brief �g�嗦���擾
	/// @return �g�嗦
	const Vector3& GetScale() const { return m_scale; }

	/// @brief ��]���擾
	/// @return ��]
	const Quaternion& GetRotation() const { return m_rot; }

	/// @brief ��]�p�x���擾
	/// @return ��]�p�x
	const float GetAngle() const { return m_angle; }

	/// @brief �X�L�����f�������_�[���擾
	/// @return �X�L�����f�������_�[
	SkinModelRender* GetSkinModelRender()
	{
		return m_skinModelRender;
	}

	/// @brief �G�l�~�[�̎�ނ��擾
	/// @return 
	EnEnemyType& GetType()
	{
		return m_enEnemyType;
	}

	/// @brief �G�l�~�[�̌��j�X�R�A���擾
	/// @return 
	int GetScore()
	{
		return m_score;
	}

	/// @brief �v���C���[�ɑ΂��铖���蔻�肪�L�����ǂ���
	/// @return �A�N�e�B�u�t���O
	const bool GetIsActive()
	{
		return m_isActive;
	}

	/// @brief ���G��Ԃ��ǂ����H
	/// @return ���G�t���O
	bool GetIsInvincible()
	{
		return m_isInvincible;
	}

	/// @brief ���݂��Ă��邩�ǂ����H
	/// @return ���݃t���O
	bool GetIsExist()
	{
		return m_isExist;
	}

	//�Z�b�^�[
	/// @brief	���W��ݒ� 
	/// @param pos ���W
	void SetPostion(const Vector3& pos) 
	{ 
		m_position = pos;
		m_skinModelRender->SetPosition(m_position);
	}

	/// @brief �g�嗦��ݒ�
	/// @param scale �g�嗦
	void SetScale(const Vector3& scale) 
	{ 
		m_scale = scale;
		m_skinModelRender->SetScale(m_scale);
	}

	/// @brief ��]��ݒ�
	/// @param rot	��]
	void SetRotation(const Quaternion rot) 
	{ 
		m_rot = rot;
		m_skinModelRender->SetRotation(m_rot);
	}

	/// @brief �p�x��ݒ�
	/// @param angle �p�x
	void SetAngle(const float angle) { m_angle = angle;}

	/// @brief �v���C���[�ɑ΂��铖���蔻���L���ɂ��邩�ǂ���
	/// @param isActive 
	void SetActive(const bool isActive) { m_isActive = isActive; }

	/// @brief �G�l�~�[�̎�ނ�ݒ�
	/// @param type 
	void SetType(const EnEnemyType& type)
	{
		m_enEnemyType = type;
	}

	/// @brief ���G���Ԃ�ݒ�
	/// @param invTime ���G����
	void SetInvincibleTime(const float invTime)
	{
		m_invTime = invTime;
	}

	//���C�g��n�����߂̊֐�

	/// @brief �f�B���N�V�������C�g���󂯂Ƃ�
	/// @param dirLight �f�B���N�V�������C�g
	void RecieveDirectionLight(DirectionLight* dirLight) 
	{
		m_skinModelRender->InitDirectionLight(dirLight);
	}

	/// @brief �|�C���g���C�g���󂯎��
	/// @param ptLight �|�C���g���C�g
	void RecievePointLight(PointLight* ptLight)
	{
		m_skinModelRender->InitPointLight(ptLight);
	}

	/// @brief �X�|�b�g���C�g���󂯎��
	/// @param spLight �X�|�b�g���C�g
	void RecieveSpotLight(SpotLight* spLight)
	{
		m_skinModelRender->InitSpotLight(spLight);
	}

	/// @brief ���f���̏�����
	void InitModelFromInitData() 
	{
		m_skinModelRender->InitModel();
	}

	/// @brief �v���C���[�ɓ_�������_���������Ɏ��ł�����
	void SelfDestroy();

private:
	//�����ōs������
	
	/// @brief ��������1�x�����Ă΂�鏈��
	/// @return �������I���t���O 
	bool Start() override;

	/// @brief �X�V����
	void Update() override;

	/// @brief �ړ�����
	void Move();

	/// @brief ��]����
	void Rotation();

	/// @brief �q�b�g����
	void Hit();

	/// @brief ���j����
	void Destroy();

	/// @brief �^�C���A�b�v���ɏ��ł����鏈��
	void DestroyTimeUp();

	/// @brief ���G���Ԃ����炵�A���G���Ԃ��؂ꂽ�疳�G��Ԃ��I�t�ɂ���
	void DecInvTime();

	/// @brief �G�l�~�[�̐������Ԃ����炷
	void DecLifeTime();

	/// @brief �����蔻�肪�L���ɂȂ�܂ł̃J�E���^�[�����炷
	void DecToActivateTime();

private:
	SkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[
	MyCharacterController m_myCharaCon;					//����̃L�����N�^�[�R���g���[��
	SphericalMove m_sphericalMove;						//���ʈړ��p�N���X
	EnEnemyType m_enEnemyType = enCommon;				//�G�l�~�[�̃^�C�v

	Player_new* m_player = nullptr;						//�v���C���[�ւ̃|�C���^
	Explosion* m_explosion = nullptr;

	float m_life = 0.0f;								//�ϋv�l
	float m_lifeTime = 1.0f;							//���Ԏ���
	float m_speed = 0.0f;								//�ړ����x
	int m_score = 0;									//�G�l�~�[�̌��j�X�R�A
	float m_durability = 0.0f;							//�e�ւ̉e���l
	bool m_isExist = false;								//���݃t���O
	bool m_isInvincible = false;						//���G��Ԃ��ǂ���
	bool m_isActive = false;							//�v���C���[�ɑ΂��铖���蔻�肪�L�����ǂ���

	Vector3 m_position = Vector3::Zero;					//���W
	Vector3 m_moveSpeed = Vector3::Zero;				//���x�x�N�g��
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//���C���΂������x�N�g��
	Vector3 m_forward = Vector3::Zero;					//�O��
	Vector3 m_right = Vector3::Zero;					//�E
	Vector3 m_up = Vector3::Zero;						//��
	Vector3 m_scale = Vector3::One;						//�g�嗦
	Quaternion m_rot = Quaternion::Identity;			//��]
	float m_angle = 0.0f;								//�p�x

	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;

	float m_invTime = 0.0f;								//���G����
	float m_toActivateCounter = 0.0f;					//�o�����Ă��瓖���蔻�肪�L���ɂȂ�܂ł̃J�E���^�[

	//���j�G�t�F�N�g
	Effect m_destroyEffect;
	//�q�b�g�G�t�F�N�g
	Effect m_hitEffect;

};

