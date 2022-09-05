#include <TOE/Core/GlobalConfig.h>
#include <fstream>

namespace TOE
{
	void GlobalConfig::Init()
	{
		std::ifstream file;
		file.open(m_FilePath);
		m_JSON = nlohmann::json::parse(file);
		file.close();
	}

	void GlobalConfig::SetConfigFile(const std::string& path)
	{
		m_FilePath = path;
		Init();
	}

	void GlobalConfig::Write()
	{
		std::ofstream file(m_FilePath);
		file << m_JSON;
	}
}