#pragma once
#include "Shader.h"

#include <string>
#include <map>

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

			void SetShader(std::string assetID);
			void BindTextures();
			void BindShaders();
			void ClearResources();
			bool isValid();

			std::string GetName() { return m_Name; }
			std::string GetShaderID() { return m_ShaderID; }


			static Material LoadFile(std::string filePath);
			static Material LoadFileFromID(std::string assetID);

		private:
			std::string m_Name = "material";
			void SerializeShader();

			SharedPtr<Shader> m_Shader;

			SharedPtr<UniformBuffer> m_PropertyBuffer;

			std::map<std::string, TextureProps> m_Textures;
			std::string m_ShaderID;

			static std::string DEFAULT_PROPERTY_BUFFER_NAME;

			friend class MaterialSerializer;
			friend class MaterialEditorPanel;

		};
	}

}


