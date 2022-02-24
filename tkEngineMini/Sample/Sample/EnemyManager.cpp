#include "stdafx.h"
#include "EnemyManager.h"

//オブジェクトの初期化
EnemyManager* EnemyManager::m_enemyManager = nullptr;


void EnemyManager::InitEnemies(
	const Vector3& initPoint,
	const Vector3& initUp,
	const EnEnemyType enemyType
)
{
	//配列内にエネミーを作成
	m_enemies.push_back(NewGO<Enemy>(0, "enemy"));

	//作成時に初期化
	m_enemies[m_enemies.size() - 1]->Init(
		initPoint,
		initUp,
		enemyType
	);
}

void EnemyManager::ExecuteUpdate()
{
	//存在フラグを調べて、オフだったら破棄
	for (auto& enemy : m_enemies) {
		if (enemy->GetIsExist() == false) {
			DeleteGO(enemy);
		}
	}

	//配列からエネミーを消すための条件を記述した関数オブジェクト
	auto func = [&](Enemy* enemy)->bool {
		//存在フラグがfalseだったらtrueを返す(=削除対象にする)
		if (enemy->GetIsExist() == false) {
			return true;
		}
		//それ以外の場合はfalse
		return false;
	};

	//eraseとremove_ifを組み合わせ
	//remove_ifで配列内の先頭から終端までを調査し、関数オブジェクトがtrueを返してきた要素(=弾の存在フラグがfalse、つまり削除対象)を末尾へ移動させていく。
	//remove_ifの戻り値は末尾に移動させた削除対象たちの先頭イテレータなのでそこから終端までをeraseすることで配列から削除される
	m_enemies.erase(
		std::remove_if(m_enemies.begin(), m_enemies.end(), func),
		m_enemies.end()
	);
}