#pragma once
class WaveCutIn
{
public:
	WaveCutIn(){}
	~WaveCutIn();

	/// @brief 初期化処理
	void Init();

	/// @brief 画像の切り替え
	void SwitchingSprite();

	/// @brief スプライトの移動処理
	void SpriteMove();

	//スプライトのフェードインとフェードアウト処理
	void SpriteFade();

	/// @brief 更新処理
	void Update();
private:
	SpriteRender* m_waveSprite = nullptr;

	Vector3 m_waveSpritePos = Vector3::Zero;
	float m_waveSpriteAlpha = 0.0f;
	
	float m_waveSpriteMoveFraction = 0.0f;		//スプライトの移動用補間率
	float m_waveSpriteWaitCounter = 0.0f;

	bool m_isValidMove = true;	//移動可能かどうかフラグ
	bool m_isFinishMoveToCenter = false;		//中央に移動したかどうか
};

