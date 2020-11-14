#include "GLFrameBuffer.h"
#include <glad/glad.h>
namespace Akkad {
	namespace Graphics {

		GLFrameBuffer::GLFrameBuffer(FrameBufferDescriptor desc)
		{
			m_desc = desc;
			glGenFramebuffers(1, &m_ResourceID);
			Bind();
			if (desc.hasColorAttachment)
			{
				glGenTextures(1, &m_ColorAttachmentTextureID);
			}
			UpdateTexture();
			Unbind();
		}

		GLFrameBuffer::~GLFrameBuffer()
		{
		}

		void GLFrameBuffer::UpdateTexture()
		{
			if (m_desc.hasColorAttachment)
			{
				Bind();
				glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentTextureID);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_desc.width, m_desc.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentTextureID, 0);

				glBindTexture(GL_TEXTURE_2D, 0);
				Unbind();
			}

		}

		void GLFrameBuffer::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_ResourceID);
			glViewport(0, 0, m_desc.width, m_desc.height);
		}

		void GLFrameBuffer::Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void GLFrameBuffer::SetSize(unsigned int width, unsigned int height)
		{
			m_desc.width = width;
			m_desc.height = height;

			UpdateTexture();
		}

		unsigned int GLFrameBuffer::GetColorAttachmentTexture()
		{
			return m_ColorAttachmentTextureID;
		}

		FrameBufferDescriptor& GLFrameBuffer::GetDescriptor()
		{
			return m_desc;
		}

	}
}


