#pragma once

class DirectionLight;
class PointLight;
class SpotLight;

class SkinModelRender : public IGameObject
{
public:
	/// @brief コンストラクタ
	SkinModelRender() {}
	
	/// @brief デストラクタ
	~SkinModelRender();
	
	/// @brief	NewGOした1フレーム後に呼び出される処理 
	/// @return	初期化終了フラグ
	bool Start() override;
	
	/// @brief	毎フレーム実行される処理
	void Update() override;
	
	/// @brief	毎フレーム呼ばれる描画処理
	/// @param rc	レンダリングコンテキスト
	void Render(RenderContext& rc) override;

	/// @brief 初期化処理
	/// @param modelFilePath	モデルのファイルパス 
	/// @param upAxis	上方向
	/// @param renderingEngine	レンダリングエンジン
	/// @param shadowCasterFlag	シャドウキャスターにするかどうか
	/// @param shadowRecieverFlag	シャドウレシーバーにするかどうか
	void Init(
		const char* modelFilePath, 
		EnModelUpAxis upAxis, 
		bool shadowCasterFlag = false, 
		bool shadowRecieverFlag = false,
		const char* skeletonFilePath = nullptr
	);
	
	/// @brief ディレクションライトを定数バッファに渡す処理
	/// @param dirLight ディレクションライト
	void InitDirectionLight(DirectionLight* dirLight);

	/// @brief ポイントライトを定数バッファに渡す処理
	/// @param ptLight ポイントライト
	void InitPointLight(PointLight* ptLight);
	
	/// @brief スポットライトを定数バッファに渡す処理
	/// @param spLight スポットライト
	void InitSpotLight(SpotLight* spLight);

	/// @brief モデルを初期化情報で初期化
	void InitModel();

	//セッター
	
	/// @brief 座標の設定
	/// @param pos 座標
	void SetPosition(const Vector3& pos);

	/// @brief 拡大率の設定
	/// @param scale 拡大率
	void SetScale(const Vector3& scale);

	/// @brief 回転の設定
	/// @param rot 回転
	void SetRotation(const Quaternion& rot);

	/// @brief 描画フラグを設定
	/// @param isDraw 描画するかどうか
	void SetIsDraw(bool isDraw) 
	{
		m_model.SetIsDraw(isDraw);

		//シャドウキャスターフラグがオンだったら影も描画
		if (m_isShadowCaster) {
			m_shadowModel.SetIsDraw(isDraw);
		}
	}

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

	//追加のモデルの初期化情報を設定
	void SetModelInitDataAdditional(const ModelInitData& modelInitData);

	//ゲッター

	/// @brief 座標を取得
	/// @return 座標
	Vector3& GetPosition() { return m_position; }
	
	/// @brief 拡大率を取得
	/// @return 拡大率
	Vector3& GetScale() { return m_scale; }
	
	/// @brief 回転を取得
	/// @return 回転
	Quaternion& GetRotation() { return m_rot; }

	/// @brief モデルの描画フラグを取得
	/// @return 描画フラグ
	const bool GetIsDraw()
	{
		return m_model.GetIsDraw();
	}

	/// @brief モデルを取得
	/// @return モデル
	Model& GetModel() { return m_model; }

	/// @brief モデルのワールド行列を取得
	/// @return ワールド行列
	const Matrix& GetMatrix() const
	{
		return m_model.GetWorldMatrix();
	}

	/// @brief ワールド行列の更新(手動)
	void UpdateWorldMatrix() 
	{
		m_model.UpdateWorldMatrix(m_position, m_rot, m_scale); 
	}
	
	/// @brief ワールド行列の更新(手動)
	/// @param pos 座標
	/// @param rot 回転クォータニオン
	/// @param scale 拡大率
	void UpdateWorldMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		m_model.UpdateWorldMatrix(pos, rot, scale);
	}

private:
	RenderingEngine* m_renderingEngine = nullptr;	//レンダリングエンジン
	
	Model m_model;								//通常描画用モデル
	ModelInitData m_modelInitData;				//通常モデル用の初期化データ

	Model m_shadowModel;						//影用モデル
	ModelInitData m_shadowModelInitData;		//影用モデルの初期化データ

	Skeleton m_skeleton;						//スケルトン

	Vector3 m_position = Vector3::Zero;			//座標
	Vector3 m_scale = Vector3::One;				//拡大率
	Quaternion m_rot = Quaternion::Identity;	//回転

	bool m_isShadowCaster = false;				//シャドウキャスターフラグ
	bool m_isShadowReciever = false;			//シャドウレシーバーフラグ
};