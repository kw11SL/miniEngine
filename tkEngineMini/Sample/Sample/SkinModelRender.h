#pragma once

class Model;
class AnimationClip;
class DirectionLight;
class PointLight;
class SpotLight;

class SkinModelRender : public IGameObject
{
public:
	/// @brief �R���X�g���N�^
	SkinModelRender() {}
	
	/// @brief �f�X�g���N�^
	~SkinModelRender();
	
	/// @brief	NewGO����1�t���[����ɌĂяo����鏈�� 
	/// @return	�������I���t���O
	bool Start() override;
	
	/// @brief	���t���[�����s����鏈��
	void Update() override;
	
	/// @brief	���t���[���Ă΂��`�揈��
	/// @param rc	�����_�����O�R���e�L�X�g
	void Render(RenderContext& rc) override;

	/// @brief ����������
	/// @param modelFilePath	���f���̃t�@�C���p�X 
	/// @param upAxis	�����
	/// @param renderingEngine	�����_�����O�G���W��
	/// @param shadowCasterFlag	�V���h�E�L���X�^�[�ɂ��邩�ǂ���
	/// @param shadowRecieverFlag	�V���h�E���V�[�o�[�ɂ��邩�ǂ���
	void Init(const char* modelFilePath, EnModelUpAxis upAxis , RenderingEngine& renderingEngine, bool shadowCasterFlag = false , bool shadowRecieverFlag = false);
	
	/// @brief �f�B���N�V�������C�g��萔�o�b�t�@�ɓn������
	/// @param dirLight �f�B���N�V�������C�g
	void InitDirectionLight(DirectionLight* dirLight);

	/// @brief �|�C���g���C�g��萔�o�b�t�@�ɓn������
	/// @param ptLight �|�C���g���C�g
	void InitPointLight(PointLight* ptLight);
	
	/// @brief �X�|�b�g���C�g��萔�o�b�t�@�ɓn������
	/// @param spLight �X�|�b�g���C�g
	void InitSpotLight(SpotLight* spLight);

	/// @brief ���f�������������ŏ�����
	void InitModel();

	/*void InitAnimation(AnimationClip* animation, int animationNum);
	void PlayAnimation(int animNo, float interpolateTime = 0.0f);*/

	//�Z�b�^�[
	
	/// @brief ���W�̐ݒ�
	/// @param pos ���W
	void SetPosition(const Vector3& pos);

	/// @brief �g�嗦�̐ݒ�
	/// @param scale �g�嗦
	void SetScale(const Vector3& scale);

	/// @brief ��]�̐ݒ�
	/// @param rot ��]
	void SetRotation(const Quaternion& rot);

	/// @brief �V���h�E�L���X�^�[�t���O���Z�b�g
	/// @param isShadowchaster �V���h�E�L���X�^�[�ɂ��邩�ǂ���
	void SetShadowChastarFlag(const bool isShadowcaster) 
	{
		m_isShadowCaster = isShadowcaster;
	}
	
	/// @brief	�V���h�E���V�[�o�[�t���O���Z�b�g 
	/// @param isShadowReciever �V���h�E���V�[�o�[�ɂ��邩�ǂ���
	void SetShadowRecieverFlag(const bool isShadowReciever)
	{
		m_isShadowReciever = isShadowReciever;
	}

	//�Q�b�^�[

	/// @brief ���W���擾
	/// @return ���W
	Vector3& GetPosition() { return m_position; }
	
	/// @brief �g�嗦���擾
	/// @return �g�嗦
	Vector3& GetScale() { return m_scale; }
	
	/// @brief ��]���擾
	/// @return ��]
	Quaternion& GetRotation() { return m_rot; }

	/// @brief ���f�����擾
	/// @return ���f��
	Model& GetModel() { return m_model; }

	/// @brief ���f���̃��[���h�s����擾
	/// @return ���[���h�s��
	const Matrix& GetMatrix() const
	{
		return m_model.GetWorldMatrix();
	}

	/// @brief ���[���h�s��̍X�V(�蓮)
	void UpdateMatrix() 
	{
		m_model.UpdateWorldMatrix(m_position, m_rot, m_scale); 
	}

private:
	RenderingEngine* m_renderingEngine = nullptr;	//�����_�����O�G���W��
	
	Model m_model;								//�ʏ�`��p���f��
	ModelInitData m_modelInitData;				//�ʏ탂�f���p�̏������f�[�^

	Model m_shadowModel;						//�e�p���f��
	ModelInitData m_shadowModelInitData;		//�e�p���f���̏������f�[�^

	Skeleton m_skeleton;						//�X�P���g��

	Vector3 m_position = Vector3::Zero;			//���W
	Vector3 m_scale = Vector3::One;				//�g�嗦
	Quaternion m_rot = Quaternion::Identity;	//��]

	bool m_isShadowCaster = false;				//�V���h�E�L���X�^�[�t���O
	bool m_isShadowReciever = false;			//�V���h�E���V�[�o�[�t���O
};