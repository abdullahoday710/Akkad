#pragma once
#include "Akkad/core.h"
#include "Akkad/Graphics/Rect.h"

#include <glm/glm.hpp>

#include <map>
#include <vector>

#include <stb_truetype.h>

namespace Akkad {

	// forward declarations
	namespace Graphics {
		class Texture;
	}

	namespace GUI {

		class Font {

		public:
			struct FontCharacter
			{
				Graphics::Rect CharacterRect; // returns a textured rect, call Renderer2D::DrawRect() and pass the font atlas to it.
			};

			Font(std::string ttfpath);
			Font(std::string ttfpath, unsigned int sizePixels);
			~Font();

			SharedPtr<Graphics::Texture> GetTextureAtlas() { return m_FontAtlas; };
			glm::vec2 GetTextureAtlasSize() { return m_FontAtlasSize; };
			FontCharacter GetASCIICharacter(char character, float& xpos, float& ypos);
			unsigned int GetFontSize() { return m_FontPixelSize; }
		private:
			void LoadFontFile(std::string ttfpath);
			void SetFontPixelScaling(unsigned int sizePixels);
			void BakeFontTextureAtlas();

			SharedPtr<Graphics::Texture> m_FontAtlas;
			glm::vec2 m_FontAtlasSize = {0,0};

			unsigned int m_FontPixelSize = 64;

			bool m_hasLoadedFont = false;
			std::vector<unsigned char> m_FontFileBuffer;
			stbtt_fontinfo m_FontInfo;
			stbtt_packedchar* m_PackedFontChars;
			friend class GUIText;

		};
	}
}