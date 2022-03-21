#pragma once
class StageBackGround : public IGameObject
{
public:
	StageBackGround();
	~StageBackGround();

	bool Start() override;
	void Update() override;
	
	/// @brief 初期化処理
	/// @param renderingEngine	レンダリングエンジン 
	/// @param pos 座標
	/// @param rot 回転
	/// @param scale 拡大率
	void Init(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

	/// @brief 座標を設定
	/// @param pos 
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		m_skinModelRender->SetPosition(m_position);
	}

	/// @brief 回転を設定
	/// @param qRot 
	void SetRotation(const Quaternion& qRot)
	{
		m_rotation = qRot;
		m_skinModelRender->SetRotation(m_rotation);
	}

	/// @brief 拡大率を設定
	/// @param scale 
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_skinModelRender->SetScale(m_scale);
	}

	/// @brief ディレクションライトの受け取り
	/// @param dirLight 
	void RecieveDirectionLight(DirectionLight* dirLight)
	{
		m_skinModelRender->InitDirectionLight(dirLight);
	}

	/// @brief ポイントライトの受け取り
	void RecievePointLight(PointLight* ptLight)
	{
		m_skinModelRender->InitPointLight(ptLight);
	}

	/// @brief スポットライトの受け取り
	void RecieveSpotLight(SpotLight* spLight)
	{
		m_skinModelRender->InitSpotLight(spLight);
	}

	/// @brief モデルの初期化
	void InitModelFromInitData()
	{
		m_skinModelRender->InitModel();
	}

	/// @brief モデルレンダーの取得
	SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }

private:
	RenderingEngine* m_renderingEngine = nullptr;
	SkinModelRender* m_skinModelRender = nullptr;		//モデルレンダー
	Vector3 m_position = Vector3::Zero;					//座標
	Quaternion m_rotation = Quaternion::Identity;		//回転
	Vector3 m_scale = Vector3::One;						//拡大率
	float m_angle = 0.0f;								//角度

	DirectionLight* m_directionLight = nullptr;
	PointLight* m_pointLight = nullptr;
	SpotLight* m_spotLight = nullptr;
};

