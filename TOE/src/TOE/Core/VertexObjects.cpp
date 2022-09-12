#include "pch.h"
#include <TOE/Graphics/VertexObjects.h>

namespace TOE
{
	void VertexLayout::AddAttribute(Type type, int count)
	{
		TOE_PROFILE_FUNCTION();

		int offset = m_Stride;
		m_TotalCount += count;
		switch (type)
		{
		case Type::Byte:
			m_Stride += count * sizeof(char);
			break;
		case Type::UnsignedByte:
			m_Stride += count * sizeof(unsigned char);
			break;
		case Type::Short:
			m_Stride += count * sizeof(short);
			break;
		case Type::UnsignedShort:
			m_Stride += count * sizeof(unsigned short);
			break;
		case Type::Int:
			m_Stride += count * sizeof(int);
			break;
		case Type::UnsignedInt:
			m_Stride += count * sizeof(unsigned int);
			break;
		case Type::Float:
			m_Stride += count * sizeof(float);
			break;
		default:
			break;
		}
		m_Attributes.push_back({ type, count, offset });
	}
	unsigned int VertexLayout::GetTotalCount() const
	{
		return m_TotalCount;
	}
	unsigned int VertexLayout::GetStride() const
	{
		return m_Stride;
	}
	std::vector<VertexLayout::Attribute> TOE::VertexLayout::GetAttributes() const
	{
		return m_Attributes;
	}

	VAO::VAO()
	{
		TOE_PROFILE_FUNCTION();

		glGenVertexArrays(1, &m_ID);
		glGenBuffers(1, &m_VBO);
	}
	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &m_ID);
		glDeleteBuffers(1, &m_VBO);
	}
	unsigned int VAO::GetID() const
	{
		return m_ID;
	}
	void VAO::SetData(void* data, int size, const VertexLayout& layout)
	{
		TOE_PROFILE_FUNCTION();

		glBindVertexArray(m_ID);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		for (int i = 0; i < layout.GetAttributes().size(); i++)
		{
			VertexLayout::Attribute attribute = layout.GetAttributes()[i];
			glVertexAttribPointer(i, attribute.Count, (int)attribute.Type, GL_FALSE, layout.GetStride(), (void*)attribute.Offset);
			glEnableVertexAttribArray(i);
		}
	}
	void VAO::Use() const
	{
		glBindVertexArray(m_ID);
	}
	unsigned int VAO::GetVertexCount() const
	{
		return m_VertexCount;
	}

	EBO::EBO()
	{
		glGenBuffers(1, &m_ID);
	}
	EBO::~EBO()
	{
		glDeleteBuffers(1, &m_ID);
	}
	unsigned int EBO::GetID() const
	{
		return m_ID;
	}
	unsigned int EBO::GetCount() const
	{
		return m_Count;
	}
	void EBO::SetData(const std::vector<unsigned int>& indices)
	{
		TOE_PROFILE_FUNCTION();

		m_Count = (unsigned int)indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}
	void EBO::Use() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
}