#include "GLESFrameBuffer.h"
#include "GLESTexture.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
namespace Akkad {
	namespace Graphics {

		GLESFrameBuffer::GLESFrameBuffer(FrameBufferDescriptor desc)
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

		GLESFrameBuffer::~GLESFrameBuffer()
		{
		}

		void GLESFrameBuffer::UpdateTexture()
		{
			if (m_desc.hasColorAttachment)
			{
				Bind();
				glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentTextureID);

				glTexImage2D(GL_TEXTURE_2D, 0, GLESTexture::TextureFormatToGLFormat(m_desc.ColorAttachmentFormat), m_desc.width, m_desc.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentTextureID, 0);

				glBindTexture(GL_TEXTURE_2D, 0);
				Unbind();
			}

		}

		void GLESFrameBuffer::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_ResourceID);
			glViewport(0, 0, m_desc.width, m_desc.height);
		}

		void GLESFrameBuffer::Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void GLESFrameBuffer::SetSize(unsigned int width, unsigned int height)
		{
			m_desc.width = width;
			m_desc.height = height;

			UpdateTexture();
		}

		unsigned int GLESFrameBuffer::GetColorAttachmentTexture()
		{
			return m_ColorAttachmentTextureID;
		}

		glm::vec4 GLESFrameBuffer::ReadPixels(int x, int y)
		{
			glm::vec4 pixelData;

			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ResourceID);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, glm::value_ptr(pixelData));

			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			return pixelData;
		}

		FrameBufferDescriptor& GLESFrameBuffer::GetDescriptor()
		{
			return m_desc;
		}

	}
}


