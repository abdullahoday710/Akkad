#pragma once
#include "Akkad/Graphics/Texture.h"

namespace Akkad {
	namespace Graphics {

		class GLTexture : public Texture
		{
		public:
			GLTexture(const char* path);
			GLTexture(TextureDescriptor desc);
			GLTexture(const char* path, float tileWidth, float tileHeight);
			~GLTexture();
			virtual void Bind(unsigned int unit) override;
			virtual void Unbind() override;
			virtual unsigned int GetID() override { return m_ResourceID; }
			virtual void SetSubData(int x, int y, unsigned int width, unsigned int height, void* data) override;
			virtual TextureDescriptor GetDescriptor() override { return m_desc; };

			static unsigned int TextureFormatToGLFormat(TextureFormat format);
			static unsigned int TextureTypeToGLType(TextureType type);
		private:
			void InitilizeTexture();
			void SetTextureImageData();
			unsigned int m_ResourceID;
			TextureDescriptor m_desc;

		};
	}
}


