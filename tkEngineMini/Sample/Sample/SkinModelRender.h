#pragma once

class Model;
class AnimationClip;
class DirectionLight;
class PointLight;
class SpotLight;

class SkinModelRender : public IGameObject
{
public:
	SkinModelRender() {}
	~SkinModelRender() {}
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const char* modelFilePath, EnModelUpAxis upAxis , RenderingEngine& renderingEngine, bool shadowCasterFlag = false , bool shadowRecieverFlag = false);
	void InitDirectionLight(DirectionLight* dirLight);
	void InitPointLight(PointLight* ptLight);
	void InitSpotLight(SpotLight* spLight);
	/*void InitAnimation(AnimationClip* animation, int animationNum);
	void PlayAnimation(int animNo, float interpolateTime = 0.0f);*/

	//�Z�b�^�[
	void SetPosition(const Vector3& pos);
	void SetScale(const Vector3& scale);
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
	Vector3 GetPosition() { return m_position; }
	Vector3 GetScale() { return m_scale; }
	Quaternion GetRotation() { return m_rot; }

private:
	RenderingEngine* m_renderingEngine = nullptr;	//�����_�����O�G���W��
	
	Model m_model;								//�ʏ�`��p���f��
	ModelInitData m_modelInitData;				//�ʏ탂�f���p�̏������f�[�^

	Model m_shadowModel;						//�e�p���f��
	ModelInitData m_shadowModelInitData;		//�e�p���f���̏������f�[�^

	Skeleton m_skeleton;						//�X�P���g��
	CharacterController m_charaCon;				//�L�����R��

	Vector3 m_position = Vector3::Zero;			//���W
	Vector3 m_scale = Vector3::One;				//�g�嗦
	Quaternion m_rot = Quaternion::Identity;	//��]

	bool m_isShadowCaster = false;				//�V���h�E�L���X�^�[�t���O
	bool m_isShadowReciever = false;			//�V���h�E���V�[�o�[�t���O
};