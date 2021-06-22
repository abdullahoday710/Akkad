#pragma once
#include "Shader.h"
#include "Texture.h"

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
			bool HasTexture(std::string samplerName);
			bool SetTexture(std::string samplerName, std::string assetID);

			SharedPtr<Texture> GetTexture(std::string samplerName);
			std::string GetName() { return m_Name; }
			std::string GetShaderID() { return m_ShaderID; }
			std::string GetTextureID(std::string samplerName);


			static SharedPtr<Material> LoadFile(std::string filePath);
			static SharedPtr<Material> LoadFileFromID(std::string assetID);

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


