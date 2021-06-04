#include "Font.h"
#include "Akkad/Logging.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Texture.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <algorithm>
#include <fstream>
namespace Akkad {
	namespace GUI {

		Font::Font(std::string ttfpath)
		{
			LoadFontFile(ttfpath);
			BakeFontTextureAtlas();
		}

		Font::Font(std::string ttfpath, unsigned int sizePixels)
		{
			LoadFontFile(ttfpath);
			SetFontPixelScaling(sizePixels);
		}

		Font::~Font()
		{
			free(m_PackedFontChars);
		}

		Font::FontCharacter Font::GetASCIICharacter(char character, float& xpos, float& ypos)
		{
			stbtt_packedchar* info = &m_PackedFontChars[character - 32];

			auto textureAtlasSize = GetTextureAtlasSize();
			stbtt_aligned_quad quad;
			stbtt_GetPackedQuad(info, textureAtlasSize.x, textureAtlasSize.y, 0, &xpos, &ypos, &quad, 0);

			FontCharacter ftcharacter;
			ftcharacter.CharacterRect.SetBounds({ quad.x0, quad.y0 }, { quad.x1, quad.y1 });
			ftcharacter.CharacterRect.SetTextureCoords({ quad.s0, quad.t0 }, { quad.s1, quad.t1 });
			return ftcharacter;
		}

		void Font::LoadFontFile(std::string ttfpath)
		{
			std::ifstream input(ttfpath, std::ios::binary);

			m_FontFileBuffer = std::vector<unsigned char>(std::istreambuf_iterator<char>(input), {});

			if (!stbtt_InitFont(&m_FontInfo, m_FontFileBuffer.data(), 0))
			{
				AK_ASSERT(false, "Could not load font file !");
			}

			m_hasLoadedFont = true;
		}

		void Font::SetFontPixelScaling(unsigned int sizePixels)
		{
			if (m_hasLoadedFont)
			{
				m_FontPixelSize = sizePixels;
				stbtt_ScaleForPixelHeight(&m_FontInfo, sizePixels);
				BakeFontTextureAtlas();
			}
		}

		void Font::BakeFontTextureAtlas()
		{
			int size_chars = 126;  // ASCII characters
			unsigned char* bitmap = nullptr;
			unsigned int texture_size = m_FontPixelSize;

			while (1) {
				bitmap = new unsigned char[(size_t)texture_size * (size_t)texture_size];
				m_PackedFontChars = (stbtt_packedchar*)malloc(sizeof(stbtt_packedchar) * size_chars + 1);

				
				stbtt_pack_context ctx;
				stbtt_PackBegin(&ctx, bitmap, texture_size, texture_size, 0, 1, 0);

				// here we are packing the font characters starting from the ASCII decimal codepoint 32 (which is a white space)
				// up to the maximum chars we want from that codepoint (size_chars is 126 because ASCII has 94 printable chars)
				// for future reference:  https://en.wikipedia.org/wiki/ASCII#Printable_characters
				// TODO: support multiple texture atlases for different codepoints
				if (!stbtt_PackFontRange(&ctx, m_FontFileBuffer.data(), 0, m_FontPixelSize, 32, size_chars, m_PackedFontChars))
				{
					delete[] bitmap;
					stbtt_PackEnd(&ctx);
					texture_size *= 2;
				}
				else
				{
					stbtt_PackEnd(&ctx);
					break;
				}
			}

			/* uploading bitmap atlas to a GPU texture */
			Graphics::TextureDescriptor desc;
			desc.Type = Graphics::TextureType::TEXTURE2D;
			desc.Format = Graphics::TextureFormat::R16;
			desc.Width = texture_size;
			desc.Height = texture_size;
			m_FontAtlasSize.x = texture_size;
			m_FontAtlasSize.y = texture_size;
			m_FontAtlas = Application::GetRenderPlatform()->CreateTexture(desc);
			m_FontAtlas->SetSubData(0, 0, texture_size, texture_size, bitmap);

			delete[] bitmap;
		}

	}
}