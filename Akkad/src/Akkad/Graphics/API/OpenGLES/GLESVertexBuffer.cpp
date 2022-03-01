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
			if (!m_Layout.isStaticBuffer)
			{
				glDeleteVertexArrays(1, &m_VA);
			}
			glDeleteBuffers(1, &m_ResourceID);
		}

		void GLESVertexBuffer::Bind()
		{
			if (!m_Layout.isStaticBuffer)
			{
				glBindVertexArray(m_VA);
			}
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
			if (!layout.isStaticBuffer)
			{
				glGenVertexArrays(1, &m_VA);
			}
			Bind();
			m_Layout = layout;
			auto& elements = m_Layout.GetElements();
			unsigned int offset = 0;

			for (unsigned int i = 0; i < elements.size(); i++)
			{
				auto& element = elements[i];
				auto glType = ElementTypeToGLType(element.type);
				auto glNormalized = element.normalized ? GL_TRUE : GL_FALSE;

				if (!m_Layout.isStaticBuffer)
				{
					glEnableVertexAttribArray(i);
					glVertexAttribPointer(i, element.count, glType, glNormalized, layout.GetStride(), (const void*)offset);
					if (element.isInstanced)
					{
						glVertexAttribDivisor(i, 1);
					}
				}
				element.offset = offset;
				offset += element.count * GetSizeOfType(element.type);
				m_AvailableVertexAttribute++;
			}
			UnBind();
		}

		void GLESVertexBuffer::ExtendLayout(SharedPtr<VertexBuffer> vb)
		{
			auto otherLayout = vb->GetLayout();
			AK_ASSERT(otherLayout.isStaticBuffer, "Other buffer must be a static buffer in order to extend it's layout !");
			AK_ASSERT(!m_Layout.isStaticBuffer, "buffer must not be a static buffer in order to extend it's layout !");

			vb->Bind();
			glBindVertexArray(m_VA);

			auto& elements = otherLayout.GetElements();

			for (size_t i = 0; i < elements.size(); i++)
			{
				auto& element = elements[i];
				auto glType = ElementTypeToGLType(element.type);
				auto glNormalized = element.normalized ? GL_TRUE : GL_FALSE;

				glEnableVertexAttribArray(m_AvailableVertexAttribute);
				glVertexAttribPointer(m_AvailableVertexAttribute, element.count, glType, glNormalized, otherLayout.GetStride(), (const void*)element.offset);
				if (element.isInstanced)
				{
					glVertexAttribDivisor(m_AvailableVertexAttribute, 1);
				}
				m_AvailableVertexAttribute++;
			}



		}

		VertexBufferLayout& GLESVertexBuffer::GetLayout()
		{
			return m_Layout;
		}
	}
}