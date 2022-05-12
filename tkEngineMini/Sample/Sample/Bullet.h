#pragma once
#include "Explosion.h"

class ExplosionManager;

/// @brief �e�̎��
enum EnBulletType
{
	enPlayerNormal,
	enPlayerSpreadBomb,
	enEnemyNormal,
	enBulletTypeNum
};

class Bullet : public IGameObject
{
public:
	Bullet() {}
	~Bullet();

	/// @brief ����������
	/// @param renderingEngine �����_�����O�G���W��
	/// @param initPoint �����ʒu
	/// @param initUp �����ʒu�̏�x�N�g��
	/// @param direction ����
	/// @param bulletType �e�̎��
	void Init(
		RenderingEngine& renderingEngine,
		const Vector3& initPoint,
		const Vector3& initUp,
		const Vector3& direction,
		const EnBulletType& bulletType);

	//�Q�b�^�[
	/// @brief	���W���擾 
	/// @return ���W
	Vector3 GetPosition() { return m_position; }

	/// @brief �g�嗦���擾
	/// @return �g�嗦
	Vector3 GetScale() { return m_scale; }

	/// @brief ��]���擾
	/// @return ��]
	Quaternion GetRotation() { return m_rot; }

	/// @brief ��]�p�x���擾
	/// @return ��]�p�x
	float GetAngle() { return m_angle; }

	/// @brief �X�L�����f�������_�[���擾
	/// @return �X�L�����f�������_�[
	SkinModelRender* GetSkinModelRender()
	{
		return m_skinModelRender;
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
	void SetAngle(const float& angle)
	{
		m_angle = angle;
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

	/// @brief ���݃t���O���I��
	void SetIsExist()
	{
		m_isExist = true;
	}

	/// @brief ���݃t���O���擾
	/// @return 
	bool GetIsExist()
	{
		return m_isExist;
	}

	/// @brief ���f�����폜���ꂽ���ǂ���
	/// @return 
	bool GetIsModelDeleted()
	{
		return m_isModelDeleted;
	}

	/// @brief �e�̃^�C�v��ݒ�
	/// @param type 
	void SetType(const EnBulletType& type)
	{
		m_enBulletType = type;
	}

	/// @brief �e�̃^�C�v���擾
	/// @return 
	EnBulletType GetType()
	{
		return m_enBulletType;
	}

	/// @brief �e�̃_���[�W���擾
	/// @return 
	float GetPower()
	{
		return m_power;
	}

	float GetLifeTime()
	{
		return m_lifeTime;
	}

	/// @brief �_���[�W�Ԋu���擾
	/// @return 
	float GetDamageInterval()
	{
		return m_damageInterval;
	}

	/// @brief �X�L�����f�����폜
	void DeleteSkinModel()
	{
		DeleteGO(m_skinModelRender);
		m_isModelDeleted = true;
	}

	/// @brief �ϋv�l������
	/// @param decVal ����������l
	void DecLife(float decVal)
	{
		m_life -= decVal;
	}

private:
	//�����Ŏg���֐�
	
	/// @brief ��������1�x�����Ă΂�鏈���Btrue�Ŋ��� 
	/// @return 
	bool Start() override;


	void Update() override;

	/// @brief �ړ�����
	void Move();

	/// @brief ��]����
	void Rotation();

	/// @brief �������Ԃ��������A0�ȉ��Ŕj��
	void DecLifeTime();

	/// @brief �폜����
	void Destroy();

	void Hit();

	/// @brief �G�t�F�N�g�̏�����
	/// @param bulletType �e�̃^�C�v
	void InitEffect(const EnBulletType& bulletType);

private:	
	SkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[
	MyCharacterController m_myCharaCon;					//����̃L�����N�^�[�R���g���[��
	SphericalMove m_sphericalMove;						//���ʈړ��p�N���X
	EnBulletType m_enBulletType = enPlayerNormal;		//�e�̃^�C�v

	Player_new* m_player = nullptr;						//�v���C���[�ւ̃|�C���^
	ExplosionManager* m_explosionManager = nullptr;		//�����}�l�[�W���ւ̃|�C���^
	DirectionLight* m_directionLight = nullptr;			//�f�B���N�V�������C�g�ւ̃|�C���^
	PointLight* m_pointLight = nullptr;					//�|�C���g���C�g�ւ̃|�C���^
	SpotLight* m_spotLight = nullptr;					//�X�|�b�g���C�g�ւ̃|�C���^

	float m_life = 0.0f;								//�ϋv�l
	float m_speed = 0.0f;								//����
	float m_lifeTime = 0.0f;							//���Ԏ���
	float m_power = 0.0f;								//�e���^����_���[�W
	bool m_isExist = true;								//���݃t���O
	float m_damageInterval = 0.5f;						//�_���[�W��^����Ԋu�B���̐��l�����G�l�~�[�ɖ��G���Ԃ�ݒ肷��B

	Vector3 m_position = Vector3::Zero;					//���W
	Vector3 m_moveSpeed = Vector3::Zero;				//���x�x�N�g��
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//���C���΂������x�N�g��
	Vector3 m_forward = Vector3::Zero;					//�O��
	Vector3 m_oldForward = Vector3::Zero;				//���ʈړ��N���X�ɂ��X�V�O�̋L�^�p(��]�Ɏg�p����)
	Vector3 m_right = Vector3::Zero;					//�E
	Vector3 m_up = Vector3::Zero;						//��
	Vector3 m_scale = Vector3::One;						//�g�嗦
	Quaternion m_rot = Quaternion::Identity;			//��]
	float m_angle = 0.0f;								//�p�x

	Vector3 m_direction = Vector3::Zero;				//�ŏ��̔��˕���
	bool m_isDecideDirection = false;					//���˕�����O���x�N�g���ɂ������ǂ���

	//�V���b�g�G�t�F�N�g
	Effect m_shotEffect;								//�ʏ�V���b�g�̃G�t�F�N�g
	Effect m_spreadBurstEffect;							//�X�v���b�h�{���̃G�t�F�N�g
	Effect m_normalBanishEffect;						//�ʏ�V���b�g�̏��ŃG�t�F�N�g

	//�X�L�����f�������_�[�̍폜�t���O
	bool m_isModelDeleted = false;

	
};

