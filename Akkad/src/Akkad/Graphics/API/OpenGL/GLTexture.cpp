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
			m_desc.Type = TextureType::TEXTURE2D;

			InitilizeTexture();
			SetTextureImageData();
		}

		GLTexture::GLTexture(TextureDescriptor desc)
		{
			m_desc = desc;
			unsigned int textureType = TextureTypeToGLType(desc.Type);
			unsigned int textureFormat = TextureFormatToGLFormat(desc.Format);

			InitilizeTexture();

			glBindTexture(textureType, m_ResourceID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, desc.Width, desc.Height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
			glBindTexture(textureType, 0);

		}

		GLTexture::GLTexture(const char* path, float tileWidth, float tileHeight)
		{
			TextureDescriptor desc = LoadFile(path, true);
			m_desc = desc;
			m_desc.IsTilemap = true;
			m_desc.TileWidth = tileWidth;
			m_desc.TileHeight = tileHeight;
			m_desc.Type = TextureType::TEXTURE2D;
			InitilizeTexture();
			SetTextureImageData();
		}

		GLTexture::~GLTexture()
		{
		}

		void GLTexture::InitilizeTexture()
		{
			unsigned int textureType = TextureTypeToGLType(m_desc.Type);
			glGenTextures(1, &m_ResourceID);
			glBindTexture(textureType, m_ResourceID);
			glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(textureType, 0);

		}

		void GLTexture::SetTextureImageData()
		{
			unsigned int textureType = TextureTypeToGLType(m_desc.Type);
			glBindTexture(textureType, m_ResourceID);
			if (m_desc.nChannels == 3)
			{
				glTexImage2D(textureType, 0, GL_RGB, m_desc.Width, m_desc.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_desc.Data);
				glGenerateMipmap(textureType);
			}
			else if (m_desc.nChannels == 4)
			{
				glTexImage2D(textureType, 0, GL_RGBA, m_desc.Width, m_desc.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_desc.Data);
				glGenerateMipmap(textureType);
			}
			else
			{
				AK_ASSERT((false), "number of channels is not supported !");
			}
			glBindTexture(textureType, 0);
			stbi_image_free(m_desc.Data);
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

		void GLTexture::SetSubData(int x, int y, unsigned int width, unsigned int height, void* data)
		{
			unsigned int textureType = TextureTypeToGLType(m_desc.Type);
			unsigned int textureFormat = TextureFormatToGLFormat(m_desc.Format);
			glBindTexture(textureType, m_ResourceID);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexSubImage2D(textureType, 0, x, y, width, height, GL_RED, GL_UNSIGNED_BYTE, data);
			glBindTexture(textureType, 0);
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
			case TextureFormat::R8:
				return GL_R8;
			case TextureFormat::R16:
				return GL_R16;
			case TextureFormat::R32_FLOAT:
				return GL_R32F;
			default:
				break;
			}
		}
		unsigned int GLTexture::TextureTypeToGLType(TextureType type)
		{
			switch (type)
			{
			case TextureType::TEXTURE2D:
				return GL_TEXTURE_2D;
			}
		}

	}
}