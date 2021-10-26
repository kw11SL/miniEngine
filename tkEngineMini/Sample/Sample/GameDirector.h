#pragma once
class GameDirector
{
private:
	//ゲームの状態を取得
	enum EnGameState {
		enTitle,
		enGame,
		enPause,
		enStateNum
	};

public:
	/// @brief ゲームディレクターの作成
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

	/// @brief ゲームディレクターの削除
	static void DeleteInstance()
	{
		delete m_gameDirector;
		m_gameDirector = nullptr;
	}

	/// @brief スコアを取得
	/// @return 
	int GetScore()
	{
		return m_score;
	}

	/// @brief スコアを加算
	/// @param addScore 加算するスコア
	void AddScore(const int addScore)
	{
		m_score += addScore;
	}
	
	/// @brief ゲーム状態を設定
	/// @param gameState 
	void SetGameState(const EnGameState& gameState)
	{
		m_enGameState = gameState;
	}

	/// @brief ゲーム状態を取得 
	/// @return 
	EnGameState GetGameState()
	{
		return m_enGameState;
	}

private:
	GameDirector() {}
	~GameDirector() {}


private:
	static GameDirector* m_gameDirector;		//唯一のインスタンス
	
	int m_score = 0;							//スコア
	EnGameState m_enGameState = enGame;			//ゲーム状態
};

