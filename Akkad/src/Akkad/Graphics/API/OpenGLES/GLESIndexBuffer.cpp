#include "GLESIndexBuffer.h"
#include <glad/glad.h>

namespace Akkad {
	namespace Graphics {
		// --- Index Buffer ---

		GLESIndexBuffer::GLESIndexBuffer()
		{
			glGenBuffers(1, &m_ResourceID);
		}

		GLESIndexBuffer::~GLESIndexBuffer()
		{
			glDeleteBuffers(1, &m_ResourceID);
		}

		void GLESIndexBuffer::Bind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ResourceID);
		}

		void GLESIndexBuffer::Unbind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ResourceID);
		}

		void GLESIndexBuffer::SetData(const void* data, unsigned int size)
		{
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			Unbind();
		}
	}
}