#pragma once

namespace Akkad {
	namespace Graphics {

		enum class TextureType {
			TEXTURE2D
		};

		enum class TextureFormat {
			RGB8, RGB16, RGB16_FLOAT, RGB32_FLOAT, RGBA16, RGBA16_FLOAT, RGBA32_FLOAT
		};

		struct TextureDescriptor {
			TextureType Type;
			TextureFormat Format;
			int Width;
			int Height;
			int nChannels;
			unsigned char* Data;
		};

		class Texture
		{
		public:
			virtual void Bind(unsigned int unit) = 0;
			virtual void Unbind() = 0;
			virtual unsigned int GetID() = 0;

			static TextureDescriptor LoadFile(const char* path, bool flip=false);
		};
	}
}


