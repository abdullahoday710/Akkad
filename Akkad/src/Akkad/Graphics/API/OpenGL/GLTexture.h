#pragma once
#include "Akkad/Graphics/Texture.h"

namespace Akkad {
	namespace Graphics {

		class GLTexture : public Texture
		{
		public:
			GLTexture(const char* path);
			~GLTexture();
			virtual void Bind(unsigned int unit) override;
			virtual void Unbind() override;
		private:
			unsigned int m_ResourceID;
			TextureDescriptor m_desc;

		};
	}
}


