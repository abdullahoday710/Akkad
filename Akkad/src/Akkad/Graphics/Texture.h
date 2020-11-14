#pragma once

namespace Akkad {
	namespace Graphics {

		enum class TextureType {
			TEXTURE2D
		};

		struct TextureDescriptor {
			TextureType Type;
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


