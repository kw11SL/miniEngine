#pragma once
class Life
{
public:
	Life();
	~Life();
	
	/// @brief 初期化処理 
	void Init();

	/// @brief ライフアイコンの表示、非表示
	void LifeDisp();

	/// @brief 発光を明滅させる処理
	void VariableHalo();

	/// @brief 発光スプライトを横長にして見えなくする処理
	void HaloVanish();

	/// @brief 発光スプライトの表示位置を決定する処理
	void HaloDisp();

	/// @brief 更新処理
	void Update();

private:
	SpriteRender* m_lifeTextSprite = nullptr;				//ライフの文字スプライト
	Vector3 m_lifeTextSpritePos = Vector3::Zero;			//ライフの文字スプライトの位置

	SpriteRender* m_lifeIconSprite[3] = { nullptr };		//ライフアイコンの配列
	Vector3 m_lifeIconSpritePos[3];							//ライフアイコンの位置

	SpriteRender* m_lifeIconFrame[3] = { nullptr };
	Vector3 m_lifeIconFramePos[3];

	SpriteRender* m_lifeIconHalo = nullptr;					//発光スプライト
	Vector4 m_lifeIconHaloColor = { 1.0f,1.0f,1.0f,1.0f };	//発光スプライトのカラー
	float m_lifeIconHaloColorRate = 0.0f;					//発光スプライトの発光補間率
	bool m_lifeIconHaloMaxFlag = false;						//明るさの折り返しフラグ
	bool m_isHaloFadeOutReady = false;						//スプライトが消える準備フラグ
	Vector3 m_lifeIconHaloPos = Vector3::Zero;				//発光スプライトの位置
	Vector3 m_lifeIconHaloScale = Vector3::One;				//発光スプライトの拡大率
	
	int m_prevPlayerLife = 0;								//前フレームのプレイヤーのライフ
};

