#pragma once

class TklFile
{
public:
	//�I�u�W�F�N�g�f�[�^�[�B
	struct SObject
	{
		std::unique_ptr<char[]> name;	//���̖��O�B
		int parentNo;					//�e�̔ԍ��B
		float bindPose[4][3];			//�o�C���h�|�[�Y�B
		float invBindPose[4][3];		//�o�C���h�|�[�Y�̋t���B
		int no;

		//��������͂���܂肫�ɂ��Ȃ���OK ]
		//todo:�V���h�E������������ς���B
		bool shadowcasterflag = false;
		bool shadowreceiverflag = false;
		std::vector<int> intData;
		std::vector<float> floatData;
		std::vector<char*> charData;
		std::vector<Vector3> Vector3Data;
	};
	//���[�h�֐��B
	void Load(const char* filepath);
	//�N�G���֐��B
	void QuaryObject(std::function<void(SObject& obj)> query)
	{
		for (auto& obj : m_objectlist) {
			query(obj);
		}
	}
private:
	//�o�[�W�����ƍ��̐�
	int m_version = 0;
	int m_objectCout = 0;
	std::vector<SObject> m_objectlist;
};