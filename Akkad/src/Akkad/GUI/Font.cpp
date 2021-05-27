#include "Font.h"
#include "Akkad/Logging.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Texture.h"

#include <glad/glad.h>
#include <algorithm>

#include <ft2build.h>
#include <freetype/freetype.h>

namespace Akkad {
	namespace GUI {

		FT_Library Font::s_FTLib;

		Font::Font()
		{
			InitFreeType();
		}

		Font::Font(std::string filePath)
		{
			InitFreeType();
			SetFontFile(filePath, 48);
		}

		Font::Font(std::string filePath, unsigned int pixelSize)
		{
			InitFreeType();
			SetFontFile(filePath, pixelSize);
			m_FontSize = pixelSize;
		}

		bool Font::SetFontFile(std::string filePath, unsigned int pixelSize)
		{
			m_FontSize = pixelSize;
			if (FT_New_Face(s_FTLib, filePath.c_str(), 0, &m_Face))
			{
				return false;
			}
			FT_Set_Pixel_Sizes(m_Face, 0, pixelSize);
			GenerateTextureAtlas();
			m_LineSpacing = m_Face->size->metrics.height / 64;
			FT_Done_Face(m_Face);
			return true;
		}

		unsigned int Font::GetTextureAtlasID()
		{
			return m_FontTextureAtlas->GetID();
		}

		glm::vec2 Font::GetTextureAtlasSize()
		{
			return m_TextureAtlasSize;
		}

		Font::FontCharacter Font::GetCharacter(unsigned long code)
		{
			return m_FontCharacters[code];
		}

		unsigned int Font::GetLineSpacing()
		{
			return m_LineSpacing;
		}

		void Font::InitFreeType()
		{
			static bool hasInitializedFreeType = false;

			if (!hasInitializedFreeType)
			{
				FT_Init_FreeType(&s_FTLib);
				hasInitializedFreeType = true;
			}
		}

		glm::vec2 Font::CalculateTextureAtlasSize()
		{
			unsigned int index = 0;
			FT_Char character = FT_Get_First_Char(m_Face, &index);
			unsigned int width = 0;
			unsigned int height = 0;
			while (true)
			{
				character = FT_Get_Next_Char(m_Face, character, &index);
				if (!index) break;

				// load character glyph 
				FT_Int32 flags = FT_LOAD_BITMAP_METRICS_ONLY;
				if (FT_Load_Char(m_Face, character, flags))
				{
					AK_ERROR("Failed to load font glyph");
					continue;
				}

				width += m_Face->glyph->bitmap.width;
				height = std::max(height, m_Face->glyph->bitmap.rows);
			}

			glm::vec2 size{ width, height };
			m_TextureAtlasSize = size;
			return size;

		}

		void Font::GenerateTextureAtlas()
		{

			glm::vec2 AtlasSize = CalculateTextureAtlasSize();
			using namespace Graphics;

			TextureDescriptor desc;
			desc.Type = TextureType::TEXTURE2D;
			desc.Width = AtlasSize.x;
			desc.Height = AtlasSize.y;
			desc.Format = TextureFormat::R16;

			m_FontTextureAtlas = Application::GetRenderPlatform()->CreateTexture(desc);

			unsigned int index = 0;
			int xPos = 0;
			auto character = FT_Get_First_Char(m_Face, &index);
			while (true)
			{
				character = FT_Get_Next_Char(m_Face, character, &index);
				if (!index) break;

				// load character glyph 
				FT_Int32 flags = FT_LOAD_RENDER;
				if (FT_Load_Char(m_Face, character, flags))
				{
					AK_ERROR("Failed to load font glyph");
					continue;
				}
				m_FontTextureAtlas->SetSubData(xPos, 0, m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows, m_Face->glyph->bitmap.buffer);

				FontCharacter ftchar;
				ftchar.Advance = m_Face->glyph->advance.x;
				ftchar.Size = glm::ivec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows);
				ftchar.Bearing = glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top);
				ftchar.xOffset = xPos;
				m_FontCharacters.insert(std::pair<unsigned long, FontCharacter>(character, ftchar));

				xPos += m_Face->glyph->bitmap.width;
			}
		}
	}
}