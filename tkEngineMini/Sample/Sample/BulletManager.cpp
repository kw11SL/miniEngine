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
	m_bullets.push_back(NewGO<Bullet>(0, "bullet"));

	m_bullets[m_bullets.size() - 1]->Init(
		*RenderingEngine::GetInstance(),
		initPoint,
		initUp,
		direction,
		bulletType
	);
}

void BulletManager::ExecuteUpdate()
{
	for (auto& bullet : m_bullets) {
		if (bullet->GetIsExist() == false) {
			//bullet->DeleteSkinModel();
			DeleteGO(bullet);
		}
	}

	//弾を消すための条件を記述した匿名関数
	auto func = [&](Bullet* bullet)->bool {
		//存在フラグがfalseだったらtrueを返す(=削除対象にする)
		if (bullet->GetIsExist() == false) {
			return true;
		}
		//それ以外はfalse
		return false;
	};

	//eraseとremove_ifを組み合わせ
	//remove_ifで配列内の先頭から終端までを調査し、匿名関数がtrueを返してきた要素(=弾の存在フラグがfalse、つまり削除対象)を末尾へ移動させていく。
	//remove_ifの戻り値は末尾に移動させた削除対象たちの先頭イテレータなのでそこから終端までをeraseすることで配列から削除される
	m_bullets.erase(
		std::remove_if(m_bullets.begin(),m_bullets.end(), func),
		m_bullets.end()
	);
	
}