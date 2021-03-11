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
			void ClearResources();
			bool isValid();

			std::string GetName() { return m_Name; }


			static Material LoadFile(std::string filePath);

		private:
			std::string m_Name = "material";
			void SerializeShader();

			SharedPtr<Shader> m_Shader;

			//std::vector<SharedPtr<UniformBuffer>> m_PropertyBuffers;

			SharedPtr<UniformBuffer> m_PropertyBuffer;

			std::vector<TextureProps> m_Textures;
			std::string m_ShaderID;

			static std::string DEFAULT_PROPERTY_BUFFER_NAME;

			friend class MaterialSerializer;
			friend class MaterialEditorPanel;

		};
	}

}


