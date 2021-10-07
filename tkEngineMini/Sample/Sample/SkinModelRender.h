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

	//セッター
	void SetPosition(const Vector3& pos);
	void SetScale(const Vector3& scale);
	void SetRotation(const Quaternion& rot);

	/// @brief シャドウキャスターフラグをセット
	/// @param isShadowchaster シャドウキャスターにするかどうか
	void SetShadowChastarFlag(const bool isShadowcaster) 
	{
		m_isShadowCaster = isShadowcaster;
	}
	
	/// @brief	シャドウレシーバーフラグをセット 
	/// @param isShadowReciever シャドウレシーバーにするかどうか
	void SetShadowRecieverFlag(const bool isShadowReciever)
	{
		m_isShadowReciever = isShadowReciever;
	}

	//ゲッター
	Vector3 GetPosition() { return m_position; }
	Vector3 GetScale() { return m_scale; }
	Quaternion GetRotation() { return m_rot; }

private:
	RenderingEngine* m_renderingEngine = nullptr;	//レンダリングエンジン
	
	Model m_model;								//通常描画用モデル
	ModelInitData m_modelInitData;				//通常モデル用の初期化データ

	Model m_shadowModel;						//影用モデル
	ModelInitData m_shadowModelInitData;		//影用モデルの初期化データ

	Skeleton m_skeleton;						//スケルトン
	CharacterController m_charaCon;				//キャラコン

	Vector3 m_position = Vector3::Zero;			//座標
	Vector3 m_scale = Vector3::One;				//拡大率
	Quaternion m_rot = Quaternion::Identity;	//回転

	bool m_isShadowCaster = false;				//シャドウキャスターフラグ
	bool m_isShadowReciever = false;			//シャドウレシーバーフラグ
};