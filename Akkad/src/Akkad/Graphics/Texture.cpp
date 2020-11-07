#include "Texture.h"
#include "Akkad/core.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Akkad {
	namespace Graphics {

		TextureDescriptor Texture::LoadFile(const char* path, bool flip)
		{
			TextureDescriptor desc;

			stbi_set_flip_vertically_on_load(flip);
			unsigned char* data = stbi_load(path, &desc.Width, &desc.Height, &desc.nChannels, 0);

			AK_ASSERT((data != nullptr), "unable to load image");
			desc.Data = data;

			return desc;

		}
	}
}