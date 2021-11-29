#pragma once

	struct LevelObjectData;

	/// <summary>
	/// �}�b�v�`�b�v�B
	/// </summary>
	class MapChip_New
	{
	public:
		MapChip_New(const LevelObjectData& objData, const char* filePath);
		~MapChip_New(){}

		/// <summary>
		/// ���f����`��B
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g�B</param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// ���f�������_���[���擾�B
		/// </summary>
		/// <returns></returns>
		Model& GetSkinModel()
		{
			return m_model;
		}
	private:
		
		Model m_model;
		PhysicsStaticObject m_physicsStaticObject;

	};
