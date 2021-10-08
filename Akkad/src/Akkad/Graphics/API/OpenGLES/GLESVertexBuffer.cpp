#include "GLESVertexBuffer.h"
#include "Akkad/core.h"

#include <GLES3/gl3.h>
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

		GLESVertexBuffer::GLESVertexBuffer()
		{
			glGenVertexArrays(1, &m_VA);
			glGenBuffers(1, &m_ResourceID);
		}

		GLESVertexBuffer::~GLESVertexBuffer()
		{
			glDeleteVertexArrays(1, &m_VA);
			glDeleteBuffers(1, &m_ResourceID);
		}

		void GLESVertexBuffer::Bind()
		{
			glBindVertexArray(m_VA);
			glBindBuffer(GL_ARRAY_BUFFER, m_ResourceID);
		}

		void GLESVertexBuffer::UnBind()
		{
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void GLESVertexBuffer::SetData(const void* data, unsigned int size)
		{
			Bind();
			if (m_Layout.isDynamic)
			{
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
			}
			else
			{
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			}
			UnBind();
		}

		void GLESVertexBuffer::SetSubData(unsigned int offset, const void* data, unsigned int size)
		{
			AK_ASSERT(m_Layout.isDynamic, "trying to modify a static buffer subdata");
			Bind();
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
			UnBind();
		}

		void GLESVertexBuffer::SetLayout(VertexBufferLayout layout)
		{
			Bind();
			m_Layout = layout;
			const std::vector<BufferElement>& elements = layout.GetElements();
			unsigned int offset = 0;

			for (unsigned int i = 0; i < elements.size(); i++)
			{
				auto element = elements[i];
				auto glType = ElementTypeToGLType(element.type);
				auto glNormalized = element.normalized ? GL_TRUE : GL_FALSE;

				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, element.count, glType, glNormalized, layout.GetStride(), (const void*)offset);
				offset += element.count * GetSizeOfType(element.type);
			}
			UnBind();
		}

		VertexBufferLayout& GLESVertexBuffer::GetLayout()
		{
			return m_Layout;
		}
	}
}