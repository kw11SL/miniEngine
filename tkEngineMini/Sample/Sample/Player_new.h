#pragma once
#include "MyCharacterController.h"

class SkinModelRender;
class DirectionLight;
class PointLight;
class SpotLight;

class Player_new : public IGameObject
{
public:
	Player_new(){}
	~Player_new();

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
	SkinModelRender* GetSkinModelRender();

	//�Z�b�^�[
	/// @brief	���W��ݒ� 
	/// @param pos ���W
	void SetPostion(const Vector3& pos);

	/// @brief �g�嗦��ݒ�
	/// @param scale �g�嗦
	void SetScale(const Vector3& scale);

	/// @brief ��]��ݒ�
	/// @param rot	��]
	void SetRotation(const Quaternion rot);
	
	/// @brief �p�x��ݒ�
	/// @param angle �p�x
	void SetAngle(const float& angle);
	
	/// @brief ����������
	/// @param renderingEngine�@�����_�����O�G���W�� 
	void Init(RenderingEngine& renderingEngine);

	//���C�g��n�����߂̊֐�
	
	/// @brief �f�B���N�V�������C�g���󂯂Ƃ�
	/// @param dirLight �f�B���N�V�������C�g
	void RecieveDirectionLight(DirectionLight* dirLight);
	
	/// @brief �|�C���g���C�g���󂯎��
	/// @param ptLight �|�C���g���C�g
	void RecievePointLight(PointLight* ptLight);
	
	/// @brief �X�|�b�g���C�g���󂯎��
	/// @param spLight �X�|�b�g���C�g
	void RecieveSpotLight(SpotLight* spLight);

	/// @brief ���f���̏�����
	void InitModelFromInitData();

private:
	bool Start() override;

	void Update() override;

	/// @brief �ړ�����
	void Move();

	/// @brief ��]����
	void Rotation();

private:
	SkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f�������_�[
	
	//CharacterController m_charaCon;						//�L�����N�^�[�R���g���[��

	MyCharacterController m_myCharaCon;					//����̃L�����N�^�[�R���g���[��
	

	Vector3 m_position = Vector3::Zero;					//���W
	Vector3 m_moveSpeed = Vector3::Zero;				//���x�x�N�g��
	Vector3 m_downVector = { 0.0f,-10.0f,0.0f };		//���C���΂������x�N�g��
	Vector3 m_forward = Vector3::Zero;					//�O��
	Vector3 m_right	= Vector3::Zero;					//�E
	Vector3 m_up = Vector3::Zero;						//��
	Vector3 m_scale = Vector3::One;						//�g�嗦
	Quaternion m_rot = Quaternion::Identity;			//��]
	float m_angle = 0.0f;								//��]�p�x

};

