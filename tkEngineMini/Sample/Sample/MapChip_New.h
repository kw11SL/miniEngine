#pragma once

	struct LevelObjectData;

	/// <summary>
	/// マップチップ。
	/// </summary>
	class MapChip_New
	{
	public:
		MapChip_New(const LevelObjectData& objData, const char* filePath);
		~MapChip_New(){}

		/// <summary>
		/// モデルを描画。
		/// </summary>
		/// <param name="rc">レンダーコンテキスト。</param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// モデルレンダラーを取得。
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
