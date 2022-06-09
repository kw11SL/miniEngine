#pragma once
class WaveCutIn
{
public:
	/// @brief カットインするスプライトの構造体
	struct SCutinSprite {
		SpriteRender* cutinSprite = nullptr;	//カットインするスプライト
		Vector3 pos = Vector3::Zero;			//位置
		float alpha = 0.0f;						//透明度係数
		float moveFraction = 0.0f;				//移動の補間率
		float waitCounter = 0.0f;				//移動カウンタ
		bool isValidMove = true;				//移動可能フラグ
		bool isMoveFinishToCenter = false;		//中央に移動したかどうかのフラグ
	};


	WaveCutIn(){}
	~WaveCutIn();

	/// @brief 初期化処理
	void Init();

	/// @brief スプライトの移動処理
	/// @param sCutinSprite カットインスプライトの構造体
	void SpriteMove(SCutinSprite* sCutinSprite);

	/// @brief スプライトのフェードイン、アウト処理
	/// @param sCutinSprite カットインスプライトの構造体
	void SpriteFade(SCutinSprite* sCutinSprite);

	/// @brief 更新処理
	void Update();
private:

	std::vector<SCutinSprite*> m_cutinSprites;	//カットインスプライトの可変長配列
	int m_waveNumPrevFrame = 1;					//前フレームのwave数
	bool m_isStart = false;						//ゲーム開始フラグ
};

