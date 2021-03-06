#pragma once
#include "Shader.h"

#include <string>

namespace Akkad {
	namespace Graphics {

		struct TextureProps
		{
			unsigned int textureBindingUnit = -1;
			std::string samplerName;
			std::string assetID;
		};

		class Material
		{
		public:
			Material() {};
			Material(std::string name) { m_Name = name; }

			~Material() {};

			void SetShader(std::string assetID, bool serialize = true);
			void BindTextures();
			void BindShaders();

			std::string GetName() { return m_Name; }

			bool isValid();

			static Material LoadFile(std::string filePath);

		private:
			std::string m_Name = "material";
			void SerializeShader();

			SharedPtr<Shader> m_Shader;
			SharedPtr<UniformBuffer> m_PropertiesBuffer;

			std::vector<TextureProps> m_Textures;
			std::string m_ShaderID;

			friend class MaterialSerializer;
			friend class MaterialEditorPanel;

		};
	}

}


