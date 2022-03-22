#include "stdafx.h"
#include "EnemyGenerator.h"
#include<random>

namespace {
	//エフェクト関連
	const char16_t* ENEMY_SPAWN_EFFECT_FILEPATH = u"Assets/effect/enemySpawn.efk";		//スポーンエフェクトのファイルパス
	const Vector3 ENEMY_SPAWN_EFFECT_SCALE = { 20.0f,20.0f,20.0f };						//スポーンエフェクトの拡大率

	//カウンター関連
	const float BORDER_TIMEUP = 10.0f;						//スポーン周期を早くする時間のボーダー
	//通常時
	const float GENERATE_TIME = 2.0f;						//スポーンを開始するカウンタ
	const float ENEMY_SPAWN_TIME = 2.0f;					//エネミーのスポーン周期
	const float EFFECT_PLAY_TIME = 1.4f;					//スポーン時のエフェクトを再生する周期
	
	//タイムアップ前
	const float GENERATE_TIME_NEAR_TIMEUP = 1.0f;			//タイムアップ前のスポーン開始カウンタ
	const float ENEMY_SPAWN_TIME_NEAR_TIMEUP = 1.0f;		//タイムアップ前のエネミーのスポーン周期
	const float EFFECT_PLAY_TIME_NEAR_TIMEUP = 0.4f;		//タイムアップ前のスポーン時のエフェクトを再生する周期

	//生成器のアクティベートまでの時間
	const float GENERATOR_ACTIVE_COUNT_SHOT = 45.0f;		//射撃型エネミー生成器をアクティブにする時間
	const float GENERATOR_ACTIVE_COUNT_BOMB = 30.0f;		//自爆型エネミー生成器をアクティブにする時間
}

EnemyGenerator::~EnemyGenerator()
{
	//配列内の全エネミーをDeleteGO
	for (auto& enemy : m_enemies) {
		DeleteGO(enemy);
	}

	//配列内の要素を最初から最後まで全て消去
	m_enemies.erase(m_enemies.begin(), m_enemies.end());
}

void EnemyGenerator::Init(const Vector3& pos, const Quaternion& rot, const bool isActive ,const EnEnemyType& enemyType)
{
	////エネミーの管理クラスのポインタを取得
	//m_enemyManager = EnemyManager::GetInstance();

	m_position = pos;
	m_rotation = rot;
	m_spawnEnemyType = enemyType;

	//初期化時にアクティブにするかどうかを選択
	SetActive(isActive);

	//キャラコンを初期化
	m_myCharaCon.Init(m_position);
	m_sphericalMove.Init(m_forward, m_right, m_up);

	//エフェクトを初期化
	m_spawnEffect.Init(ENEMY_SPAWN_EFFECT_FILEPATH);
}

void EnemyGenerator::SpawnEnemy()
{
	//自身の可変長配列の中にNewGOする
	m_enemies.push_back(NewGO<Enemy>(0, ENEMY_NAME));

	//初期化するのは可変長配列のサイズ - 1の要素
	m_enemies[m_enemies.size() - 1]->Init(
		m_position,
		m_up,
		m_spawnEnemyType
	);
}

void EnemyGenerator::PlaySpawnEffect()
{
	//残り時間が0以下だったら生成しない
	if (GameDirector::GetInstance()->GetTime() <= 0.0f) {
		return;
	}

	//アクティブフラグがオンなら発生
	if (m_isActive == true) {
		m_spawnEffect.SetPosition(m_position);
		m_spawnEffect.SetRotation(m_rotation);
		m_spawnEffect.SetScale(ENEMY_SPAWN_EFFECT_SCALE);

		m_spawnEffect.Play(false);
	}
}

void EnemyGenerator::Generate()
{

	//残り時間が0以下だったら生成しない
	if (GameDirector::GetInstance()->GetGameState() != enGame || 
		GameDirector::GetInstance()->GetTime() <= 0.0f) {
		return;
	}

	//最大数を超えていたら処理しない
	if (GameDirector::GetInstance()->GetEnemyCount() > GameDirector::GetInstance()->GetMaxEnemyNum()){
		return;
	}

	//乱数を生成
	std::random_device rnd;
	std::mt19937 mt(rnd());
	//指定した値の範囲でランダムなfloat値を返す
	std::uniform_real_distribution<float> randFloat(0.0f, 0.5f);

	//発生間隔
	float interval = 0.0f;

	//乱数の値をスポーン周期に加える
	//タイムアップが迫ってきたら発生間隔を小さくする
	if (GameDirector::GetInstance()->GetTime() <= BORDER_TIMEUP) {
		interval = GENERATE_TIME_NEAR_TIMEUP;
		interval += randFloat(mt);
	}
	else {
		interval = GENERATE_TIME;
		interval += randFloat(mt);
	}

	//生成準備フラグをオン。アクティブフラグがオンになるまで生成準備しない。
	if (m_generateCounter > interval && m_isActive == true) {

		//スポーン処理中フラグをオン
		m_isSpawning = true;

		if (m_isValidPlayEffect == false) {
			m_isValidPlayEffect = true;
		}

		if (m_isValidSpawnEnemy == false) {
			m_isValidSpawnEnemy = true;
		}

		//生成準備カウンターをリセット
		m_generateCounter = 0.0f;

	}
}

void EnemyGenerator::SpawnEnemyWithEffect()
{
	float effectPlayTime = 0.0f;
	float enemySpawnTime = 0.0f;

	//タイムアップ前ならタイムアップ前の発生間隔に
	if (GameDirector::GetInstance()->GetTime() <= BORDER_TIMEUP) {
		effectPlayTime = EFFECT_PLAY_TIME_NEAR_TIMEUP;
		enemySpawnTime = ENEMY_SPAWN_TIME_NEAR_TIMEUP;
	}
	//そうでなければ通常時の発生間隔に
	else {
		effectPlayTime = EFFECT_PLAY_TIME;
		enemySpawnTime = ENEMY_SPAWN_TIME;
	}


	//エフェクト再生カウンターが一定に達していてエフェクト再生可能なら
	if (m_spawnEffectCounter >= effectPlayTime 
		&& m_isValidPlayEffect == true) {
		
		//エフェクトを再生
		PlaySpawnEffect();
		//カウンターをリセット
		m_spawnEffectCounter = 0.0f;
		//そのスポーン処理中は再生しない
		m_isValidPlayEffect = false;
	}

	//エネミーをスポーンするカウンターが一定に達していて生成可能なら
	if (m_spawnCounter >=  enemySpawnTime
		&& m_isValidSpawnEnemy == true) {
		
		//エネミーを生成
		SpawnEnemy();

		//エネミーがスポーンし終えたのでカウンターをリセット、スポーン処理フラグをオフ
		m_spawnCounter = 0.0f;
		//そのスポーン処理中は生成しない
		m_isValidSpawnEnemy = false;
		m_isSpawning = false;
	}
}

void EnemyGenerator::UpdateEffect()
{
	//エフェクトを更新
	m_spawnEffect.Update();
}

void EnemyGenerator::Activate()
{
	//射撃型エネミー生成器のとき
	//一定時間以下でアクティブフラグをオン
	if (m_spawnEnemyType == enShot &&
		GameDirector::GetInstance()->GetTime() <= GENERATOR_ACTIVE_COUNT_SHOT) {
		SetActive(true);
	}

	//生成器がボムのとき
	//一定時間以下でアクティブフラグをオン
	if (m_spawnEnemyType == enBomb &&
		GameDirector::GetInstance()->GetTime() <= GENERATOR_ACTIVE_COUNT_BOMB) {
		SetActive(true);
	}
}

void EnemyGenerator::Move()
{
	m_myCharaCon.Execute(m_moveSpeed, m_downVector, 0.0f);
	m_sphericalMove.UpdateVectorFromUp(m_downVector, m_forward, m_up, m_right);
}

void EnemyGenerator::Rotation()
{
	m_sphericalMove.Rotation(m_forward,m_right,m_up,m_rotation);
}

void EnemyGenerator::DeleteEnemy()
{
	//エネミーの存在フラグがオフだったらDeleteGO
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

void EnemyGenerator::AddCounter()
{
	//スポーン処理中でなければ各種カウンターを0にする
	if (m_isSpawning != true) {
		m_spawnCounter = 0.0f;
		m_spawnEffectCounter = 0.0f;
	}
	//処理中はカウンターを上昇させる
	else{
		AddSpawnCounter();
		AddSpawnEffectPlayCounter();
	}
}

void EnemyGenerator::Update()
{
	//ゲーム中以外なら処理しない
	if (GameDirector::GetInstance()->GetGameState() != enGame) {
		return;
	}

	//生成器のアクティベート処理
	Activate();
	//移動処理
	Move();
	//回転処理
	Rotation();
	//生成準備カウンターを増加
	AddGenerateCounter();
	//生成準備
	Generate();
	//エフェクト発生、エネミーのスポーンカウンターを増加させる処理
	AddCounter();
	SpawnEnemyWithEffect();
	//生存フラグの立っていないエネミーを消去
	DeleteEnemy();
	//エフェクトを更新
	UpdateEffect();
}