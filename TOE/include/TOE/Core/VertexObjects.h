#pragma once

#include <vector>
#include <spdlog/spdlog.h>
#include <GL/glew.h>

namespace TOE
{
	enum class Type
	{
		Byte = GL_BYTE,
		UnsignedByte = GL_UNSIGNED_BYTE,
		Short = GL_SHORT,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Int = GL_INT,
		UnsignedInt = GL_UNSIGNED_INT,
		Float = GL_FLOAT,
	};

	class VertexLayout
	{
	public:
		struct Attribute
		{
			TOE::Type Type;
			int Count;
			int Offset;
		};

		void AddAttribute(Type type, int count);
		int GetTotalCount();
		int GetStride();
		std::vector<Attribute> GetAttributes();

	private:
		std::vector<Attribute> m_Attributes;
		int m_TotalCount = 0;
		int m_Stride = 0;
	};

	class VAO
	{
	public:
		VAO();

		unsigned int GetID();
		template <typename T>
		void SetData(std::vector<T> data, VertexLayout layout)
		{
			SetData(data.data(), data.size() * sizeof(T), layout);
			m_VertexCount += data.size() / layout.GetTotalCount();
		}
		void SetData(void* data, int size, VertexLayout layout);
		void Use();
		unsigned int GetVertexCount();

	private:
		unsigned int m_ID = 0;
		unsigned int m_VBO = 0;
		unsigned int m_VertexCount = 0;
	};

	class EBO
	{
	public:
		EBO();

		unsigned int GetID();
		int GetCount();
		void SetData(std::vector<unsigned int> indices);
		void Use();

	private:
		unsigned int m_ID = 0;
		int m_Count = 0;
	};
}