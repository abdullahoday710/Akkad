#include "GLTexture.h"
#include "Akkad/core.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Akkad {
	namespace Graphics {
		GLTexture::GLTexture(const char* path)
		{
			TextureDescriptor desc = LoadFile(path, true);
			m_desc = desc;

			glGenTextures(1, &m_ResourceID);
			glBindTexture(GL_TEXTURE_2D, m_ResourceID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (desc.nChannels == 3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, desc.Width, desc.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, desc.Data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else if (desc.nChannels == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, desc.Width, desc.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, desc.Data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				AK_ASSERT((false), "number of channels is not supported !");
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(desc.Data);
		}

		GLTexture::~GLTexture()
		{
		}

		void GLTexture::Bind(unsigned int unit)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, m_ResourceID);
		}

		void GLTexture::Unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		unsigned int GLTexture::TextureFormatToGLFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB8:
				return GL_RGB8;
			case TextureFormat::RGB16:
				return GL_RGB16;
			case TextureFormat::RGB16_FLOAT:
				return GL_RGB16F;
			case TextureFormat::RGB32_FLOAT:
				return GL_RGB32F;
			case TextureFormat::RGBA16:
				return GL_RGBA16;
			case TextureFormat::RGBA16_FLOAT:
				return GL_RGBA16F;
			case TextureFormat::RGBA32_FLOAT:
				return GL_RGBA32F;
			default:
				break;
			}
		}
	}
}