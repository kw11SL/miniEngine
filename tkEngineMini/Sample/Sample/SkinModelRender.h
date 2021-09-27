#pragma once

class Model;
class AnimationClip;
class DirectionLight;
class PointLight;
class SpotLight;

//�ǂ̃V�F�[�_�[���g�p���邩
enum EnShadingMode {
	enCommonShading,	//�ʏ�`��
	enShadowMap,		//�V���h�E�}�b�v
	enShadowReciever	//�V���h�E���V�[�o�[
};

class SkinModelRender : public IGameObject
{
public:
	SkinModelRender();
	~SkinModelRender();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void Init(const char* modelFilePath, EnShadingMode shadingMode, EnModelUpAxis upAxis );
	void InitShader(const char* fxFilePath, const char* entryPoint);
	void InitDirectionLight(DirectionLight* dirLight);
	void InitPointLight(PointLight* ptLight);
	void InitSpotLight(SpotLight* spLight);
	/*void InitAnimation(AnimationClip* animation, int animationNum);
	void PlayAnimation(int animNo, float interpolateTime = 0.0f);*/

	//�Z�b�^�[
	void SetPosition(const Vector3& pos);
	void SetScale(const Vector3& scale);
	void SetRotation(const Quaternion& rot);
	//�Q�b�^�[
	Vector3 GetPosition() { return m_position; }
	Vector3 GetScale() { return m_scale; }
	Quaternion GetRotation() { return m_rot; }

private:
	Model m_model;
	ModelInitData m_modelInitData;
	Skeleton m_skeleton;
	CharacterController m_charaCon;
	EnShadingMode m_shadingMode = enCommonShading;		//�g�p����V�F�[�_�̎��

	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rot = Quaternion::Identity;

};