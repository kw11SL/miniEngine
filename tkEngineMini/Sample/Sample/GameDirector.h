#pragma once
class GameDirector
{
private:
	//�Q�[���̏�Ԃ��擾
	enum EnGameState {
		enTitle,
		enGame,
		enPause,
		enStateNum
	};

public:
	/// @brief �Q�[���f�B���N�^�[�̍쐬
	static void CreateInstance()
	{
		if (m_gameDirector == nullptr) {
			m_gameDirector = new GameDirector;
		}
	}

	/// @brief �C���X�^���X�̎擾
	/// @return 
	static GameDirector* GetInstance()
	{
		return m_gameDirector;
	}

	/// @brief �Q�[���f�B���N�^�[�̍폜
	static void DeleteInstance()
	{
		delete m_gameDirector;
		m_gameDirector = nullptr;
	}

	/// @brief �X�R�A���擾
	/// @return 
	int GetScore()
	{
		return m_score;
	}

	/// @brief �X�R�A�����Z
	/// @param addScore ���Z����X�R�A
	void AddScore(const int addScore)
	{
		m_score += addScore;
	}
	
	//�G�l�~�[����1���₷
	void AddEnemyCount()
	{
		m_enemyNum++;
	}

	//�G�l�~�[����1���炷
	void DecEnemyCount()
	{
		m_enemyNum--;
	}

	/// @brief ���݂��Ă���G�l�~�[�̐����擾
	/// @return 
	const int& GetEnemyCount()
	{
		return m_enemyNum;
	}

	/// @brief �G�l�~�[��������擾
	/// @return 
	const int& GetMaxEnemyNum() const
	{
		return MAX_ENEMY_NUM;
	}

	/// @brief �Q�[����Ԃ�ݒ�
	/// @param gameState 
	void SetGameState(const EnGameState& gameState)
	{
		m_enGameState = gameState;
	}

	/// @brief �Q�[����Ԃ��擾 
	/// @return 
	EnGameState GetGameState()
	{
		return m_enGameState;
	}

private:
	GameDirector() {}
	~GameDirector() {}


private:
	static GameDirector* m_gameDirector;		//�B��̃C���X�^���X
	
	int m_score = 0;							//�X�R�A
	int m_enemyNum = 0;							//���݂̃G�l�~�[��
	float m_time = 30.0f;						//����

	const int MAX_ENEMY_NUM = 10;				//�G�l�~�[�̍ő吔

	EnGameState m_enGameState = enGame;			//�Q�[�����
};

