#include "stdafx.h"
#include "SphericalMove.h"

void SphericalMove::Init(Vector3& forward, Vector3& right, Vector3& up) 
{
	//前方はz軸
	forward = g_vec3AxisZ;
	//右はx軸
	right = g_vec3AxisX;
	//上はy軸
	up = g_vec3AxisY;
}



void SphericalMove::UpdateVectorFromUp(const Vector3& downVector, Vector3& forward, Vector3& up, Vector3& right)
{
	//新しい上ベクトル
	Vector3 newUp;

	//上ベクトルは受け取った下ベクトルの逆
	newUp = downVector * -1.0f;

	//上ベクトルを更新
	up = newUp;

	//前方ベクトルは右ベクトルと上ベクトルの外積
	forward.Cross(right, up);
	forward.Normalize();

	//右ベクトルは上ベクトルと前方ベクトルの外積
	right.Cross(up, forward);
	right.Normalize();
}

void SphericalMove::Rotation(const Vector3& forward, const Vector3& right, const Vector3& up, Quaternion& rot)
{
	Matrix rotMatrix;

	// 回転行列の1行目は、その座標系のexになる(右)
	rotMatrix.m[0][0] = right.x;
	rotMatrix.m[0][1] = right.y;
	rotMatrix.m[0][2] = right.z;

	// 回転行列の2行目は、その座標系のeyになる(上)
	rotMatrix.m[1][0] = up.x;
	rotMatrix.m[1][1] = up.y;
	rotMatrix.m[1][2] = up.z;

	// 回転行列の3行目は、その座標系のezになる(前方)
	rotMatrix.m[2][0] = forward.x;
	rotMatrix.m[2][1] = forward.y;
	rotMatrix.m[2][2] = forward.z;

	//回転行列からクォータニオンを計算(更新)
	rot.SetRotation(rotMatrix);
}
