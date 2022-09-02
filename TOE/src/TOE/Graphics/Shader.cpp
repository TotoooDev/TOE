#include <TOE/Graphics/Shader.h>
#include <TOE/Debug/Instrumentor.h>

#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <spdlog/spdlog.h>

namespace TOE
{
	Shader::~Shader()
	{
		glDeleteProgram(m_ID);
	}

	void Shader::LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
	{
		TOE_PROFILE_FUNCTION();

		m_VertexPath = vertexPath;
		m_FragmentPath = fragmentPath;
		m_GeometryPath = geometryPath;

		std::ifstream vertexFile(m_VertexPath), fragmentFile(m_FragmentPath), geometryFile(m_GeometryPath);
		std::stringstream vertexBuffer, fragmentBuffer, geometryBuffer;
		std::string vertexSource, fragmentSource, geometrySource;

		// Read files as a stringstream
		vertexBuffer << vertexFile.rdbuf();
		fragmentBuffer << fragmentFile.rdbuf();
		geometryBuffer << geometryFile.rdbuf();

		// Read as a string
		vertexSource = vertexBuffer.str();
		fragmentSource = fragmentBuffer.str();
		geometrySource = geometryBuffer.str();
		m_VertexSource = vertexSource;
		m_FragmentSource = fragmentSource;
		m_GeometrySource = geometrySource;

		// Don't forget to close the files
		vertexFile.close();
		fragmentFile.close();
		geometryFile.close();

		LoadFromString(vertexSource, fragmentSource, geometrySource);
	}

	void Shader::LoadFromFolder(const std::string& folderPath, const std::string& name)
	{
		TOE_PROFILE_FUNCTION();

		m_VertexPath = folderPath + name + ".vert";
		m_FragmentPath = folderPath + name + ".frag";
		m_GeometryPath = folderPath + name + ".geom";
		LoadFromFile(m_VertexPath, m_FragmentPath, m_GeometryPath);
	}

	void Shader::LoadFromString(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource)
	{
		TOE_PROFILE_FUNCTION();

		int success;

		// Compile vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexSrc = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		// Error handling
		if (success == GL_FALSE)
		{
			char info[1024];
			glGetShaderInfoLog(vertexShader, 1024, nullptr, info);

			spdlog::error("Failed to compile vertex shader " + m_VertexPath + "!");
			spdlog::error(info);

			glDeleteShader(vertexShader);
		}

		// Compile fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragmentSrc = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		// Error handling
		if (success == GL_FALSE)
		{
			char info[1024];
			glGetShaderInfoLog(fragmentShader, 1024, nullptr, info);

			spdlog::error("Failed to compile fragment shader " + m_FragmentPath + "!");
			spdlog::error(info);

			glDeleteShader(fragmentShader);
		}

		// Compile geometry shader
		unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		const char* geometrySrc = geometrySource.c_str();
		glShaderSource(geometryShader, 1, &geometrySrc, nullptr);
		glCompileShader(geometryShader);
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		// Error handling
		if (success == GL_FALSE)
		{
			char info[1024];
			glGetShaderInfoLog(geometryShader, 1024, nullptr, info);

			spdlog::error("Failed to compile geometry shader " + m_GeometryPath + "!");
			spdlog::error(info);

			glDeleteShader(geometryShader);
		}

		// Shader program
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader);
		glAttachShader(m_ID, fragmentShader);
		if (geometrySource != "")
		{
			glAttachShader(m_ID, geometryShader);
		}
		glLinkProgram(m_ID);
		// Error handling
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			char info[1024];
			glGetProgramInfoLog(m_ID, 1024, nullptr, info);

			spdlog::error("Failed to link shader program!");
			spdlog::error(info);

			glDeleteProgram(m_ID);
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);
	}

	void Shader::Reload()
	{
		LoadFromString(m_VertexSource, m_FragmentSource, m_GeometrySource);
	}

	void Shader::Delete()
	{
		glDeleteShader(m_ID);
	}

	void Shader::Use()
	{
		glUseProgram(m_ID);
	}

	unsigned int Shader::GetID()
	{
		return m_ID;
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		TOE_PROFILE_FUNCTION();
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	void Shader::SetFloat(const std::string& name, float value) const
	{
		TOE_PROFILE_FUNCTION();
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
	{
		TOE_PROFILE_FUNCTION();
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
	}
	void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
	{
		TOE_PROFILE_FUNCTION();
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}
}