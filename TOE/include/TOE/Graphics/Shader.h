#pragma once

#include <string>
#include <glm/glm.hpp>

namespace TOE
{
	class Shader
	{
	public:
		~Shader();

		// This method expects the shader files to be named this way:
		// <name>.vert for vertex shaders
		// <name>.frag for fragment shaders
		// <name>.geom for geometry shaders
		void LoadFromFolder(const std::string& folderPath, const std::string& name);
		void LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
		void LoadFromString(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometryPath = "");

		// Recompile the shader source
		void Reload();
		void Delete();

		void Bind() const;
		unsigned int GetID() const;

		// Uniform utility methods
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;

	private:
		unsigned int m_ID;
		std::string m_VertexPath, m_FragmentPath, m_GeometryPath;
		std::string m_VertexSource, m_FragmentSource, m_GeometrySource;
	};
}