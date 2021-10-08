#include "GLESTexture.h"
#include "Akkad/core.h"

#include <GLES3/gl3.h>
#include <stb_image.h>

namespace Akkad {
	namespace Graphics {

		GLESTexture::GLESTexture(const char* path)
		{
			TextureDescriptor desc = LoadFile(path, true);
			m_desc = desc;
			m_desc.Type = TextureType::TEXTURE2D;

			InitilizeTexture();
			SetTextureImageData();
		}

		GLESTexture::GLESTexture(TextureDescriptor desc)
		{
			m_desc = desc;
			unsigned int textureType = TextureTypeToGLType(desc.Type);
			unsigned int textureFormat = TextureFormatToGLFormat(desc.Format);

			InitilizeTexture();

			glBindTexture(textureType, m_ResourceID);
			glTexImage2D(textureType, 0, textureFormat, desc.Width, desc.Height, 0, textureFormat, GL_UNSIGNED_BYTE, 0);
			glBindTexture(textureType, 0);

		}

		GLESTexture::GLESTexture(const char* path, float tileWidth, float tileHeight)
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

		GLESTexture::~GLESTexture()
		{
		}

		void GLESTexture::InitilizeTexture()
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

		void GLESTexture::SetTextureImageData()
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

		void GLESTexture::Bind(unsigned int unit)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, m_ResourceID);
		}

		void GLESTexture::Unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void GLESTexture::SetSubData(int x, int y, unsigned int width, unsigned int height, void* data)
		{
			unsigned int textureType = TextureTypeToGLType(m_desc.Type);
			unsigned int textureFormat = TextureFormatToGLFormat(m_desc.Format);
			glBindTexture(textureType, m_ResourceID);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexSubImage2D(textureType, 0, x, y, width, height, textureFormat, GL_UNSIGNED_BYTE, data);
			glBindTexture(textureType, 0);
		}

		unsigned int GLESTexture::TextureFormatToGLFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGB8:
				return GL_RGB8;
			case TextureFormat::RGB16:
				return GL_RGB8;
			case TextureFormat::RGB16_FLOAT:
				return GL_RGB16F;
			case TextureFormat::RGB32_FLOAT:
				return GL_RGB32F;
			case TextureFormat::RGBA16:
				return GL_RGBA8;
			case TextureFormat::RGBA16_FLOAT:
				return GL_RGBA16F;
			case TextureFormat::RGBA32_FLOAT:
				return GL_RGBA32F;
			case TextureFormat::R8:
				return GL_R8;
			case TextureFormat::R16:
				return GL_R8;
			case TextureFormat::R32_FLOAT:
				return GL_R32F;
			case TextureFormat::SINGLE_CHANNEL:
				return GL_LUMINANCE;
			default:
				break;
			}
		}
		unsigned int GLESTexture::TextureTypeToGLType(TextureType type)
		{
			switch (type)
			{
			case TextureType::TEXTURE2D:
				return GL_TEXTURE_2D;
			}
		}

	}
}