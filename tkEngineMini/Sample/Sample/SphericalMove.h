#pragma once
/// @brief 球面移動機能のクラス

class SphericalMove
{
public:

	/// @brief 受け取った前方、右、上の各ベクトルを各軸で初期化する
	/// @param forward 前方ベクトル
	/// @param right 右ベクトル
	/// @param up 上ベクトル
	void Init(Vector3& forward, Vector3& right, Vector3& up);

	/// @brief 上ベクトルの更新と、それに基づいた各ベクトルの更新
	/// @param downVector 下方向ベクトル。上方向を決める基準となる。
	/// @param forward 前方ベクトル
	/// @param up 上ベクトル
	/// @param right 右ベクトル
	void UpdateVectorFromUp(const Vector3& downVector, Vector3& forward, Vector3& up, Vector3& right);

	/// @brief 前方、右、上の各ベクトルから回転クォータニオンを計算する
	/// @param rot クォータニオン
	void Rotation(const Vector3& forward, const Vector3& right, const Vector3& up, Quaternion& rot);

private:
	//内部で使用する関数

private:

};

