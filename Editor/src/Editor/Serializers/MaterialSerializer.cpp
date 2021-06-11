#include "MaterialSerializer.h"
#include "Editor/EditorLayer.h"

#include <Akkad/Graphics/Material.h>
#include <Akkad/Graphics/UniformBuffer.h>

#include <json.hpp>

#include <fstream>
namespace Akkad {

	void MaterialSerializer::Serialize(SharedPtr<Graphics::Material> material, std::string outputPath)
	{
		auto& project = EditorLayer::GetActiveProject();

		// Material serialization should be ordered so it can match the underlying property buffer.
		nlohmann::ordered_json materialData;

		materialData["material"]["name"] = material->GetName();

		if (!material->m_ShaderID.empty())
		{
			materialData["material"]["shaderID"] = material->m_ShaderID;
		}
		

		for (auto it : material->m_Textures)
		{
			auto& textureProp = it.second;
			if (!textureProp.assetID.empty())
			{
				materialData["material"]["textures"][textureProp.assetID]["samplerName"] = textureProp.samplerName;
				materialData["material"]["textures"][textureProp.assetID]["bindingUnit"] = textureProp.textureBindingUnit;
			}
			else
			{
				continue;
			}

		}

		if (material->m_PropertyBuffer != nullptr)
		{
			for (auto it : material->m_PropertyBuffer->GetLayout().GetElements())
			{
				auto elementName = it.first;
				auto element = it.second;

				Graphics::ShaderDataType type = element.GetType();

				switch (type)
				{
				case Graphics::ShaderDataType::FLOAT:
				{
					float value = material->m_PropertyBuffer->GetData<float>(elementName);
					materialData["material"]["properties"][elementName]["type"] = "float";
					materialData["material"]["properties"][elementName]["value"] = value;
					break;
				}
				case Graphics::ShaderDataType::FLOAT2:
				{
					glm::vec2 value = material->m_PropertyBuffer->GetData<glm::vec2>(elementName);
					materialData["material"]["properties"][elementName]["type"] = "float2";
					materialData["material"]["properties"][elementName]["value"]["x"] = value.x;
					materialData["material"]["properties"][elementName]["value"]["y"] = value.y;
					break;
				}
				case Graphics::ShaderDataType::FLOAT3:
				{
					glm::vec3 value = material->m_PropertyBuffer->GetData<glm::vec3>(elementName);
					materialData["material"]["properties"][elementName]["type"] = "float3";
					materialData["material"]["properties"][elementName]["value"]["x"] = value.x;
					materialData["material"]["properties"][elementName]["value"]["y"] = value.y;
					materialData["material"]["properties"][elementName]["value"]["z"] = value.z;
					break;
				}
				case Graphics::ShaderDataType::FLOAT4:
				{
					glm::vec4 value = material->m_PropertyBuffer->GetData<glm::vec4>(elementName);
					materialData["material"]["properties"][elementName]["type"] = "float4";
					materialData["material"]["properties"][elementName]["value"]["x"] = value.x;
					materialData["material"]["properties"][elementName]["value"]["y"] = value.y;
					materialData["material"]["properties"][elementName]["value"]["z"] = value.z;
					materialData["material"]["properties"][elementName]["value"]["w"] = value.w;
					break;
				}
				case Graphics::ShaderDataType::UNISGNED_INT:
				{
					unsigned int value = material->m_PropertyBuffer->GetData<unsigned int>(elementName);
					materialData["material"]["properties"][elementName]["type"] = "uint";
					materialData["material"]["properties"][elementName]["value"] = value;
					break;
				}
				}


			}
		}
		

		std::ofstream output;
		output.open(outputPath, std::ios::trunc);
		output << std::setw(4) << materialData << std::endl;
		output.close();
	}

}