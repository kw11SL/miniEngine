#pragma once

//ゲームの状態を取得
enum EnGameState {
	enTitle,			//タイトル画面
	enStart,			//ゲーム開始中
	enGame,				//ゲーム中
	enStop,				//停止中
	enGameOver,			//ゲームオーバー
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
		return MAX_ENEMY_NUM;
	}

	/// @brief ゲーム状態を設定
	/// @param gameState 設定する状態
	void SetGameState(const EnGameState& gameState)
	{
		m_enGameState = gameState;
	}

	/// @brief ゲーム状態を取得 
	/// @return ゲーム状態
	EnGameState GetGameState()
	{
		return m_enGameState;
	}

	/// @brief ステージの残時間を取得
	/// @return 残時間
	const float& GetTime() const
	{
		return m_time;
	}

	/// @brief プレイヤーのライフを取得
	/// @return プレイヤーのライフ
	const int& GetPlayerLife() const
	{
		return m_playerLife;
	}

	/// @brief ゲームを初期状態に戻す処理
	void ResetGame();

	/// @brief 更新処理
	void ExecuteUpdate();

private:
	GameDirector() {}
	~GameDirector() {}


private:
	static GameDirector* m_gameDirector;		//唯一のインスタンス
	
	//初期値
	int m_score = 0;							//スコア
	int m_playerLife = 3;						//プレイヤーのライフ
	int m_waveNumber = 1;						//現在のwave。1から開始する
	int m_enemyNum = 0;							//現在のエネミー数
	int m_totalSpawnEnemyNum = 0;				//スポーンしたエネミーの総計
	int m_totalDestroyedEnemyNum = 0;			//倒したエネミーの総数
	float m_time = 60.0f;						//ステージの残時間

	//定数
	const int MAX_ENEMY_NUM = 15;				//エネミーの最大数
	const int MAX_ENEMY_NUM_NEAR_TIMEUP = 25;	//エネミーの最大数(タイムアップ前)

	EnGameState m_enGameState = enTitle;		//ゲーム状態
};

