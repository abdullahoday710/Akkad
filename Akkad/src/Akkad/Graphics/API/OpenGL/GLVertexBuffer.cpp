#include "GLVertexBuffer.h"
#include <glad/glad.h>
namespace Akkad {
	namespace Graphics {
		unsigned int ElementTypeToGLType(ShaderDataType type) {
			switch (type)
			{
			case ShaderDataType::FLOAT:
				return GL_FLOAT;
			case ShaderDataType::FLOAT2:
				return GL_FLOAT;
			case ShaderDataType::FLOAT3:
				return GL_FLOAT;
			case ShaderDataType::FLOAT4:
				return GL_FLOAT;
			case ShaderDataType::UNISGNED_INT:
				return GL_UNSIGNED_INT;
			default:
				break;
			}
		}

		GLVertexBuffer::GLVertexBuffer()
		{
			glGenVertexArrays(1, &m_VA);
			glGenBuffers(1, &m_ResourceID);
		}

		GLVertexBuffer::~GLVertexBuffer()
		{
			glDeleteVertexArrays(1, &m_VA);
			glDeleteBuffers(1, &m_ResourceID);
		}

		void GLVertexBuffer::Bind()
		{
			glBindVertexArray(m_VA);
			glBindBuffer(GL_ARRAY_BUFFER, m_ResourceID);
		}

		void GLVertexBuffer::UnBind()
		{
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void GLVertexBuffer::SetData(const void* data, unsigned int size)
		{
			Bind();
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			UnBind();
		}

		void GLVertexBuffer::SetLayout(VertexBufferLayout layout)
		{
			Bind();
			m_Layout = layout;
			auto& elements = layout.GetElements();
			unsigned int offset = 0;

			for (unsigned int i = 0; i < elements.size(); i++)
			{
				auto element = elements[i];
				auto glType = ElementTypeToGLType(element.type);
				auto glNormalized = element.normalized ? GL_TRUE : GL_FALSE;

				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, element.count, glType, glNormalized, layout.GetStride(), (const void*)offset);
				offset += element.count * BufferElement::GetSizeOfType(element.type);
			}
			UnBind();
		}

		VertexBufferLayout& GLVertexBuffer::GetLayout()
		{
			return m_Layout;
		}
	}
}