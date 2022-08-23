#include <TOE/Core/VertexObjects.h>

namespace TOE
{
	void VertexLayout::AddAttribute(Type type, int count)
	{
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
	int VertexLayout::GetTotalCount()
	{
		return m_TotalCount;
	}
	int VertexLayout::GetStride()
	{
		return m_Stride;
	}
	std::vector<VertexLayout::Attribute> TOE::VertexLayout::GetAttributes()
	{
		return m_Attributes;
	}

	VAO::VAO()
	{
		glGenVertexArrays(1, &m_ID);
		glGenBuffers(1, &m_VBO);
	}
	unsigned int VAO::GetID()
	{
		return m_ID;
	}
	void VAO::SetData(void* data, int size, VertexLayout layout)
	{
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
	void VAO::Use()
	{
		glBindVertexArray(m_ID);
	}

	EBO::EBO()
	{
		glGenBuffers(1, &m_ID);
	}
	unsigned int EBO::GetID()
	{
		return m_ID;
	}
	int EBO::GetCount()
	{
		return m_Count;
	}
	void EBO::SetData(std::vector<unsigned int> indices)
	{
		m_Count = indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}
	void EBO::Use()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
}