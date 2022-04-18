#include "stdafx.h"
#include "ExplosionManager.h"
#include "Explosion.h"

ExplosionManager* ExplosionManager::m_explosionManager = nullptr;


void ExplosionManager::InitExplosion(
	const Vector3& pos,
	const float scaleRate,
	const EnExplosionType& explosionType
)
{
	//爆発のタイプごとに命名を振り分け
	//プレイヤー自機弾の爆発
	if (explosionType == enPlayer_Spread_Bomb) {
		m_explosions.push_back(NewGO<Explosion>(0, EXPLOSION_PLAYER_NAME));
	}
	//エネミーの爆発
	else if (explosionType == enEnemy_Explosion) {
		m_explosions.push_back(NewGO<Explosion>(0, EXPLOSION_ENEMY_NAME));
	}

	//上で配列に追加したものを初期化
	m_explosions[m_explosions.size() - 1]->Init(
		pos,
		scaleRate,
		explosionType
	);
}

void ExplosionManager::DeleteExplosions()
{
	//配列内の爆発を全てDeleteGOする
	for (auto& explosion : m_explosions) {
		DeleteGO(explosion);
	}

	//配列内の要素を全て消去
	m_explosions.erase(m_explosions.begin(), m_explosions.end());
}

void ExplosionManager::ExecuteUpdate()
{
	//存在フラグを調べて、オフだったら破棄
	for (auto& explosion : m_explosions) {
		if (explosion->GetIsExist() == false) {
			DeleteGO(explosion);
		}
	}

	//弾を消すための条件を記述した関数オブジェクト
	auto func = [&](Explosion* explosion)->bool {
		//存在フラグがfalseだったらtrueを返す(=削除対象にする)
		if (explosion->GetIsExist() == false) {
			return true;
		}
		//それ以外はfalse
		return false;
	};

	//eraseとremove_ifを組み合わせ
	//remove_ifで配列内の先頭から終端までを調査し、関数オブジェクトがtrueを返してきた要素(=弾の存在フラグがfalse、つまり削除対象)を末尾へ移動させていく。
	//remove_ifの戻り値は末尾に移動させた削除対象たちの先頭イテレータなのでそこから終端までをeraseすることで配列から削除される
	m_explosions.erase(
		std::remove_if(m_explosions.begin(), m_explosions.end(), func),
		m_explosions.end()
	);
}