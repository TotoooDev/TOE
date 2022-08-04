#pragma once

#include <string>
#include <glm/glm.hpp>

namespace TOE
{
	class Shader
	{
	public:
		void LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
		void LoadFromFolder(const std::string& folderPath, const std::string& name);
		void LoadFromString(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometryPath = "");
		void Reload();
		void Delete();

		void Use();
		unsigned int GetID();

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetVec3(const std::string& name, glm::vec3 value);
		void SetMat4(const std::string& name, glm::mat4 value);

	private:
		unsigned int m_ID;
		std::string m_VertexPath, m_FragmentPath, m_GeometryPath;
	};
}