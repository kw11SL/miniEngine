#pragma once

//ゲームの状態を取得
enum EnGameState {
	enTitle,			//タイトル画面
	enStart,			//ゲーム開始中
	enGame,				//ゲーム中
	enStop,				//停止中
	enGameOver,			//ゲームオーバー
	enTimeUp,			//タイムアップ時
	enResult,			//リザルト画面
	enPause,			//ポーズ中
	enStateNum			//定義した状態の総数
};


class GameDirector
{

public:
	/// @brief インスタンスの作成
	static void CreateInstance()
	{
		if (m_gameDirector == nullptr) {
			m_gameDirector = new GameDirector;
		}
	}

	/// @brief インスタンスの取得
	/// @return 
	static GameDirector* GetInstance()
	{
		return m_gameDirector;
	}

	/// @brief インスタンスの削除
	static void DeleteInstance()
	{
		delete m_gameDirector;
		m_gameDirector = nullptr;
	}

	/// @brief スコアを取得
	/// @return 
	const int& GetScore() const
	{
		return m_score;
	}

	/// @brief スコアを加算
	/// @param addScore 加算するスコア
	void AddScore(const int addScore)
	{
		m_score += addScore;
	}
	
	/// @brief 	エネミー数を1増やす
	void AddEnemyCount()
	{
		m_enemyNum++;
	}

	/// @brief 	出現エネミー総数を1増やす
	void AddEnemyTotalCount()
	{
		m_totalSpawnEnemyNum++;
	}

	/// @brief エネミーの撃破総数を1増やす
	void AddDestroyedEnemyCount()
	{
		m_totalDestroyedEnemyNum++;
	}

	/// @brief waveを1進める
	void AddWaveNumber()
	{
		m_waveNumber++;
		//wave数が変わったのでフラグをオン
		m_isSwitchedWave = true;
	}

	/// @brief エネミー数を1減らす
	void DecEnemyCount()
	{
		m_enemyNum--;
	}

	/// @brief 残時間を減らす
	void DecTime()
	{
		if (m_time <= 0.0f) {
			return;
		}

		m_time -= g_gameTime->GetFrameDeltaTime();

		//0を下回ったら0にする
		if (m_time < 0.0f){
			m_time = 0.0f;
		}
	}

	/// @brief プレイヤーのライフを1減らす
	void DecPlayerLife()
	{
		m_playerLife -= 1;

		if (m_playerLife <= 0) {
			m_playerLife = 0;
		}

	}

	/// @brief 存在しているエネミーの数を取得
	/// @return エネミー存在数
	const int GetEnemyCount() const
	{
		return m_enemyNum;
	}

	/// @brief 出現したエネミーの総数を取得
	/// @return エネミーの総数
	const int GetTotalEnemyCount() const
	{
		return m_totalSpawnEnemyNum;
	}

	/// @brief エネミーの撃破総数を取得
	/// @return エネミーの撃破総数
	const int GetTotalDestroyedCount() const
	{
		return m_totalDestroyedEnemyNum;
	}

	/// @brief エネミー上限数を取得
	/// @return エネミー上限数
	const int GetMaxEnemyNum() const
	{
		return m_maxEnemy;
	}

	/// @brief ゲーム状態を設定
	/// @param gameState 設定する状態
	void SetGameState(const EnGameState& gameState)
	{
		m_enGameState = gameState;
	}


	/// @brief ゲーム状態を取得 
	/// @return ゲーム状態
	const EnGameState& GetGameState()
	{
		return m_enGameState;
	}

	/// @brief 前フレームのゲーム状態を取得 
	/// @return 前フレームのゲーム状態
	const EnGameState& GetGameStatePrev()
	{
		return m_enGameStatePrevFrame;
	}

	/// @brief ステージの残時間を取得
	/// @return 残時間
	const float GetTime() const
	{
		return m_time;
	}

	/// @brief プレイヤーのライフを取得
	/// @return プレイヤーのライフ
	const int GetPlayerLife() const
	{
		return m_playerLife;
	}

	/// @brief 現在のwave数を取得
	/// @return 現在のwave数
	const int GetWaveNumber() const
	{
		return m_waveNumber;
	}

	const int GetWaveNumberPrev() const
	{
		return m_waveNumberPrev;
	}

	/// @brief waveが切り替わったかどうか
	/// @return 
	const bool GetIsSwitchedWave() const
	{
		return m_isSwitchedWave;
	}

	/// @brief 残機ボーナスを取得
	/// @return 
	const int CalcRemainBonus()
	{
		//残機×10000pt
		return m_playerLife * 10000;
	}

	/// @brief 敵の撃破率を取得
	/// @return 
	const float CalcDestructionRate()
	{
		if (m_totalDestroyedEnemyNum <= 0) {
			return 0;
		}

		m_destructionRate = m_totalDestroyedEnemyNum;
		m_destructionRate /= m_totalSpawnEnemyNum;
		m_destructionRate *= 100.0f;

		return m_destructionRate;
	}

	/// @brief 撃破率ボーナスを取得
	/// @return 
	const int CalcDestructionBonus();

	/// @brief 最終スコアを取得
	/// @return 
	const int CalcFinalScore();

	/// @brief ゲームを初期状態に戻す処理
	void ResetGame();

	/// @brief エネミー最大数を変更する処理
	void ChangeMaxEnemy();

	/// @brief 更新処理
	void ExecuteUpdate();

	/// @brief 現フレームでのフラグや数値を記録
	void RecordCurrentFrame();

private:
	GameDirector() {}
	~GameDirector() {}


private:
	static GameDirector* m_gameDirector;		//唯一のインスタンス
	
	//初期値
	int m_score = 0;							//スコア
	int m_playerLife = 3;						//プレイヤーのライフ
	int m_waveNumber = 1;						//現在のwave。1から開始する
	int m_waveNumberPrev = m_waveNumber;		//前フレームのwave数
	int m_enemyNum = 0;							//現在のエネミー数
	int m_totalSpawnEnemyNum = 0;				//スポーンしたエネミーの総計
	int m_totalDestroyedEnemyNum = 0;			//倒したエネミーの総数
	float m_time = 60.0f;						//ステージの残時間
	float m_destructionRate = 0.0f;				//敵撃破率
	int m_finalScore = 0;						//最終スコア
	float m_startToGameCounter = 3.0f;			//スタートからゲーム中に移行するための時間
	float m_timeUpToResultCounter = 4.0f;		//タイムアップからリザルトに移行するための時間
	bool m_isSwitchedWave = false;				//このフレームでwaveの切り替えが発生したかどうか
	int m_maxEnemy = MAX_ENEMY_NUM;

	EnGameState m_enGameState = enTitle;			//ゲーム状態
	EnGameState m_enGameStatePrevFrame = enTitle;	//前フレームのゲーム状態

	//EnUseWeapon m_playerWeapon = enNormalShot;	//プレイヤーの使用している武器

	//定数
	const int MAX_ENEMY_NUM = 25;				//エネミーの最大数
	const int MAX_ENEMY_NUM_NEAR_TIMEUP = 40;	//エネミーの最大数(タイムアップ前)

	


};

