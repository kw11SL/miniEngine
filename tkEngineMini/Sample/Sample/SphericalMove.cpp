#include "stdafx.h"
#include "SphericalMove.h"

void SphericalMove::Init(Vector3& forward, Vector3& right, Vector3& up) 
{
	//�O����z��
	forward = g_vec3AxisZ;
	//�E��x��
	right = g_vec3AxisX;
	//���y��
	up = g_vec3AxisY;
}



void SphericalMove::UpdateVectorFromUp(const Vector3& downVector, Vector3& forward, Vector3& up, Vector3& right)
{
	//�V������x�N�g��
	Vector3 newUp;

	//��x�N�g���͎󂯎�������x�N�g���̋t
	newUp = downVector * -1.0f;

	//��x�N�g�����X�V
	up = newUp;

	//�O���x�N�g���͉E�x�N�g���Ə�x�N�g���̊O��
	forward.Cross(right, up);
	forward.Normalize();

	//�E�x�N�g���͏�x�N�g���ƑO���x�N�g���̊O��
	right.Cross(up, forward);
	right.Normalize();
}

void SphericalMove::Rotation(const Vector3& forward, const Vector3& right, const Vector3& up, Quaternion& rot)
{
	Matrix rotMatrix;

	// ��]�s���1�s�ڂ́A���̍��W�n��ex�ɂȂ�(�E)
	rotMatrix.m[0][0] = right.x;
	rotMatrix.m[0][1] = right.y;
	rotMatrix.m[0][2] = right.z;

	// ��]�s���2�s�ڂ́A���̍��W�n��ey�ɂȂ�(��)
	rotMatrix.m[1][0] = up.x;
	rotMatrix.m[1][1] = up.y;
	rotMatrix.m[1][2] = up.z;

	// ��]�s���3�s�ڂ́A���̍��W�n��ez�ɂȂ�(�O��)
	rotMatrix.m[2][0] = forward.x;
	rotMatrix.m[2][1] = forward.y;
	rotMatrix.m[2][2] = forward.z;

	//��]�s�񂩂�N�H�[�^�j�I�����v�Z(�X�V)
	rot.SetRotation(rotMatrix);
}
