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

	/// @brief 引数で渡したオブジェクト名のオブジェクトか調べる
	/// @param objName 調べる名前
	/// @return 名前が一致した場合trueを返す
	bool EqualObjectName(const wchar_t* objName)
	{
		return wcscmp(objName, name) == 0;
	}

	/// @brief 名前の前方一致を調べる
	/// @param n 比較する名前 
	/// @return 一致していたらtrueを返す
	bool ForwardMatchName(const wchar_t* n)
	{
		auto len = wcslen(n);
		auto nameLen = wcslen(name);

		if (len > nameLen)
		{
			//名前が長いので不一致
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
	/// function<戻り値型（引数）> 関数の名前
	/// </summary>
	void Init(const char* name, std::function<bool(ObjectData& objectData) > Hookfunc);

	/// @brief 更新処理
	void Update();

	/// <summary>
	/// マップチップの数分for文を回してマップチップを描画
	/// </summary>
	void Draw();

	/// <summary>
	/// TklのマトリックスをLevelに変換する。
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