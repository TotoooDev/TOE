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
		unsigned int GetTotalCount() const;
		unsigned int GetStride() const;
		std::vector<Attribute> GetAttributes() const;

	private:
		std::vector<Attribute> m_Attributes;
		unsigned int m_TotalCount = 0;
		unsigned int m_Stride = 0;
	};

	class VAO
	{
	public:
		VAO();
		~VAO();

		template <typename T>
		void SetData(const std::vector<T>& data, const VertexLayout& layout)
		{
			SetData((void*)data.data(), (int)(data.size() * sizeof(T)), layout);
			m_VertexCount += (unsigned int)data.size() / layout.GetTotalCount();
		}
		void SetData(void* data, int size, const VertexLayout& layout);
		unsigned int GetID() const;
		void Use() const;
		unsigned int GetVertexCount() const;

	private:
		unsigned int m_ID = 0;
		unsigned int m_VBO = 0;
		unsigned int m_VertexCount = 0;
	};

	class EBO
	{
	public:
		EBO();
		~EBO();

		void SetData(const std::vector<unsigned int>& indices);
		unsigned int GetID() const;
		unsigned int GetCount() const;
		void Use() const;

	private:
		unsigned int m_ID = 0;
		unsigned int m_Count = 0;
	};
}