#include "stdafx.h"
#include "BulletManager.h"
#include "Bullet.h"

//オブジェクトの初期化
BulletManager* BulletManager::m_bulletManager = nullptr;

void BulletManager::InitBullets(
	const Vector3& initPoint,
	const Vector3& initUp,
	const Vector3& direction,
	const EnBulletType bulletType
)
{
	//弾のタイプで名前を振り分け
	m_bullets.push_back(NewGO<Bullet>(0, Naming(bulletType)));

	m_bullets[m_bullets.size() - 1]->Init(
		initPoint,
		initUp,
		direction,
		bulletType
	);
}

void BulletManager::DeleteBullets()
{
	//配列内の弾を全てDeleteGOする
	for (auto& bullet : m_bullets) {
		DeleteGO(bullet);
	}

	//配列内の要素を全て消去
	m_bullets.erase(m_bullets.begin(), m_bullets.end());
}

void BulletManager::ExecuteUpdate()
{
	//存在フラグを調べて、オフだったら破棄
	for (auto& bullet : m_bullets) {
		if (bullet->GetIsExist() == false) {
			DeleteGO(bullet);
		}
	}

	//弾を消すための条件を記述した関数オブジェクト
	auto func = [&](Bullet* bullet)->bool {
		//存在フラグがfalseだったらtrueを返す(=削除対象にする)
		if (bullet->GetIsExist() == false) {
			return true;
		}
		//それ以外はfalse
		return false;
	};

	//eraseとremove_ifを組み合わせ
	//remove_ifで配列内の先頭から終端までを調査し、関数オブジェクトがtrueを返してきた要素(=弾の存在フラグがfalse、つまり削除対象)を末尾へ移動させていく。
	//remove_ifの戻り値は末尾に移動させた削除対象たちの先頭イテレータなのでそこから終端までをeraseすることで配列から削除される
	m_bullets.erase(
		std::remove_if(m_bullets.begin(),m_bullets.end(), func),
		m_bullets.end()
	);
	
}

const char* BulletManager::Naming(const EnBulletType& type)
{
	//プレイヤーの弾を指定していれば
	if (type == enPlayerNormal || 
		type == enPlayerSpreadBomb) {
		
		//プレイヤーの弾という名前を返す
		return BULLET_PLAYER_NAME;
	}
	//エネミーの弾を指定していれば
	else if (type == enEnemyNormal) {
		
		//エネミーの弾という名前を返す
		return BULLET_ENEMY_NAME;
	}
}