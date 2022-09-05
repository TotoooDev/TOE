#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace TOE
{
	class GlobalConfig
	{
	public:
		static void Init();
		static void SetConfigFile(const std::string& path);
		static void Write();

		template <typename T>
		static void AddRule(const std::string& name, const T& value)
		{
			m_JSON[name] = value;
		}

		template <typename T>
		static T GetValue(const std::string& name)
		{
			return m_JSON[name];
		}

	private:
		inline static nlohmann::json m_JSON;
		inline static std::string m_FilePath = "TOE_GlobalConfig.json";
	};
}