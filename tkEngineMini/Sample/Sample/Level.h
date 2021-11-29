#pragma once
#include "TklFile.h"
#include "MapChip.h"
#include "TklFile.h"

struct ObjectData
{
	Vector3 Pos;
	Vector3 Scale;
	Quaternion Rot;
	const wchar_t* name;
	int number = 0;

	/// @brief �����œn�����I�u�W�F�N�g���̃I�u�W�F�N�g�����ׂ�
	/// @param objName ���ׂ閼�O
	/// @return ���O����v�����ꍇtrue��Ԃ�
	bool EqualObjectName(const wchar_t* objName)
	{
		return wcscmp(objName, name) == 0;
	}

	/// @brief ���O�̑O����v�𒲂ׂ�
	/// @param n ��r���閼�O 
	/// @return ��v���Ă�����true��Ԃ�
	bool ForwardMatchName(const wchar_t* n)
	{
		auto len = wcslen(n);
		auto nameLen = wcslen(name);

		if (len > nameLen)
		{
			//���O�������̂ŕs��v
			return false;
		}
		return wcsncmp(n, name, len) == 0;

	}
};

class Level : public IGameObject
{
public:
	~Level(){}

	/// <summary>
	/// function<�߂�l�^�i�����j> �֐��̖��O
	/// </summary>
	void Init(const char* name, std::function<bool(ObjectData& objectData) > Hookfunc);

	/// @brief �X�V����
	void Update();

	/// <summary>
	/// �}�b�v�`�b�v�̐���for�����񂵂ă}�b�v�`�b�v��`��
	/// </summary>
	void Draw();

	/// <summary>
	/// Tkl�̃}�g���b�N�X��Level�ɕϊ�����B
	/// </summary>
	void MatrixTklToLevel();

	//bool EqualObjectName(const wchar_t* objName);

private:
	bool m_isInited = false;
	using BonePtr = std::unique_ptr<Bone>;
	Skeleton m_skeleton;
	std::vector<std::unique_ptr<MapChip>> m_mapChipList;
	TklFile m_tkl;
	std::vector<BonePtr> m_bonelist;
	std::unique_ptr<Matrix[]> m_matrixlist;
};