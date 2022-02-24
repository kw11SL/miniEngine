#pragma once

#include <memory>

template<class TResource>
class TResourceBank {
public:
	TResource* Get(const char* filePath)
	{
		auto it = m_resourceMap.find(filePath);
		if (it != m_resourceMap.end()) {
			//バンクに登録されている。
			return it->second.get();
		}
		return nullptr;
	}
	void Regist(const char* filePath, TResource* resource)
	{
		auto it = m_resourceMap.find(filePath);
		if (it == m_resourceMap.end()) {
			//未登録。
			m_resourceMap.insert(
				std::pair< std::string, TResourcePtr>(filePath, resource )
			);
		}
	}
	/// @brief バンク内の要素を全消去
	void ClearBank()
	{
		m_resourceMap.erase(m_resourceMap.begin(), m_resourceMap.end());
	}

private:
	using TResourcePtr = std::unique_ptr<TResource> ;
	std::map<std::string, TResourcePtr> m_resourceMap;
};