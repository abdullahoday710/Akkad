#pragma once

namespace Akkad {
	namespace Graphics {

		enum class TextureType {
			TEXTURE2D
		};

		enum class TextureFormat {
			RGB8, RGB16, RGB16_FLOAT, RGB32_FLOAT, RGBA16, RGBA16_FLOAT, RGBA32_FLOAT, R8, R16, R32_FLOAT, SINGLE_CHANNEL
		};

		struct TextureDescriptor {
			TextureType Type;
			TextureFormat Format;
			int Width;
			int Height;
			int nChannels;

			bool IsTilemap = false;
			float TileWidth = 0.0f;
			float TileHeight = 0.0f;

			unsigned char* Data;
		};

		class Texture
		{
		public:
			virtual void Bind(unsigned int unit) = 0;
			virtual void Unbind() = 0;
			virtual unsigned int GetID() = 0;
			virtual void SetSubData(int x, int y, unsigned int width, unsigned int height, void* data) = 0;
			virtual TextureDescriptor GetDescriptor() = 0;
			static TextureDescriptor LoadFile(const char* path, bool flip=false);
		};
	}
}


