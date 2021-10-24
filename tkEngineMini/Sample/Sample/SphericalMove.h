#pragma once
/// @brief ���ʈړ��@�\�̃N���X

class SphericalMove
{
public:

	/// @brief �󂯎�����O���A�E�A��̊e�x�N�g�����e���ŏ���������
	/// @param forward �O���x�N�g��
	/// @param right �E�x�N�g��
	/// @param up ��x�N�g��
	void Init(Vector3& forward, Vector3& right, Vector3& up);

	/// @brief ��x�N�g���̍X�V�ƁA����Ɋ�Â����e�x�N�g���̍X�V
	/// @param downVector �������x�N�g���B����������߂��ƂȂ�B
	/// @param forward �O���x�N�g��
	/// @param up ��x�N�g��
	/// @param right �E�x�N�g��
	void UpdateVectorFromUp(const Vector3& downVector, Vector3& forward, Vector3& up, Vector3& right);

	/// @brief �O���A�E�A��̊e�x�N�g�������]�N�H�[�^�j�I�����v�Z����
	/// @param rot �N�H�[�^�j�I��
	void Rotation(const Vector3& forward, const Vector3& right, const Vector3& up, Quaternion& rot);

private:
	//�����Ŏg�p����֐�

private:

};

