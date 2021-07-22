#pragma once
#include "Akkad/Graphics/Texture.h"

namespace Akkad {
	namespace Graphics {

		class GLESTexture : public Texture
		{
		public:
			GLESTexture(const char* path);
			GLESTexture(TextureDescriptor desc);
			GLESTexture(const char* path, float tileWidth, float tileHeight);
			~GLESTexture();
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


