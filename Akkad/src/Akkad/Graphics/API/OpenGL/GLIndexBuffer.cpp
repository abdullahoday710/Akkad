#include "GLIndexBuffer.h"
#include <glad/glad.h>

namespace Akkad {
	namespace Graphics {
		// --- Index Buffer ---

		GLIndexBuffer::GLIndexBuffer()
		{
			glGenBuffers(1, &m_ResourceID);
		}

		GLIndexBuffer::~GLIndexBuffer()
		{
			glDeleteBuffers(1, &m_ResourceID);
		}

		void GLIndexBuffer::Bind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ResourceID);
		}

		void GLIndexBuffer::Unbind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ResourceID);
		}

		void GLIndexBuffer::SetData(const void* data, unsigned int size)
		{
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			Unbind();
		}
	}
}