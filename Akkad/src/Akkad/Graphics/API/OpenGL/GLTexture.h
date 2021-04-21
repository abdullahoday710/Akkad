#pragma once
#include "Akkad/Graphics/Texture.h"

namespace Akkad {
	namespace Graphics {

		class GLTexture : public Texture
		{
		public:
			GLTexture(const char* path);
			GLTexture(TextureDescriptor desc);
			~GLTexture();
			virtual void Bind(unsigned int unit) override;
			virtual void Unbind() override;
			virtual unsigned int GetID() override { return m_ResourceID; }
			virtual void SetSubData(int x, int y, unsigned int width, unsigned int height, void* data) override;

			static unsigned int TextureFormatToGLFormat(TextureFormat format);
			static unsigned int TextureTypeToGLType(TextureType type);
		private:
			unsigned int m_ResourceID;
			TextureDescriptor m_desc;

		};
	}
}


