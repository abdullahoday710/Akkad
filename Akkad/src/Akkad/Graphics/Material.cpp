#include "Material.h"
#include "Renderer2D.h"
#include "RenderPlatform.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

#include <spirv_cross.hpp>
#include <json.hpp>
#include <fstream>

namespace Akkad {

	namespace Graphics {

		void Material::SetShader(std::string assetID, bool serialize)
		{
			m_ShaderID = assetID;

			if (serialize)
			{
				SerializeShader();
			}

			auto assetManager = Application::GetAssetManager();
			m_Shader = assetManager->GetShader(assetID);

			m_Shader->SetUniformBuffer(Renderer2D::GetSystemUniforms());

		}

		void Material::BindTextures()
		{
			for (auto textureProp : m_Textures)
			{
				if (!textureProp.assetID.empty())
				{
					auto texture = Application::GetAssetManager()->GetTexture(textureProp.assetID);
					texture->Bind(textureProp.textureBindingUnit);
				}
			}
		}

		void Material::BindShaders()
		{
			if (m_Shader != nullptr)
			{
				m_Shader->Bind();
			}
		}

		bool Material::isValid()
		{
			if (m_Shader == nullptr)
			{
				return false;
			}

			else
			{
				return true;
			}
		}

		Material Material::LoadFile(std::string filePath)
		{
				std::ifstream file;
				file.open(filePath);
				nlohmann::json data;
				file >> data;

				Material material;

				std::string name = data["material"]["name"];
				std::string shaderID = data["material"]["shaderID"];

				material.m_Name = name;
				material.SetShader(shaderID, false);

				for (auto texture : data["material"]["textures"].items())
				{
					std::string assetID = texture.key();
					std::string samplerName = data["material"]["textures"][assetID]["samplerName"];
					unsigned int bindingUnit = data["material"]["textures"][assetID]["bindingUnit"];

					Graphics::TextureProps props;
					props.assetID = assetID;
					props.samplerName = samplerName;
					props.textureBindingUnit = bindingUnit;
					material.m_Textures.push_back(props);
				}

				return material;
		}

		void Material::SerializeShader()
		{
			m_Textures.clear();

			auto assetManager = Application::GetAssetManager();
			auto assetDesc = assetManager->GetDescriptorByID(m_ShaderID);
			auto shaderDesc = Shader::LoadShader(assetDesc.absolutePath.c_str());

			UniformBufferLayout layout;

			spirv_cross::Compiler vertexShader(std::move(shaderDesc.VertexData));
			spirv_cross::Compiler fragmentShader(std::move(shaderDesc.FragmentData));

			spirv_cross::ShaderResources resources = fragmentShader.get_shader_resources();

			unsigned int index = 0;
			for (auto resource : resources.sampled_images)
			{
				TextureProps props;
				props.textureBindingUnit = index;
				props.samplerName = resource.name;
				m_Textures.push_back(props);

				index += 1;
			}
		/* TODO : Support additional material properties using uniform buffers */
		}
	}
}

