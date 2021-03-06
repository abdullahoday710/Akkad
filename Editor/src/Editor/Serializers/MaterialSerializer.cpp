#include "MaterialSerializer.h"
#include "Editor/EditorLayer.h"
#include <Akkad/Graphics/Material.h>

#include <json.hpp>

#include <fstream>
namespace Akkad {

	void MaterialSerializer::Serialize(Graphics::Material& material, std::string outputPath)
	{
		auto& project = EditorLayer::GetActiveProject();
		nlohmann::json materialData;
		materialData["material"]["name"] = material.GetName();

		if (!material.m_ShaderID.empty())
		{
			materialData["material"]["shaderID"] = material.m_ShaderID;
		}
		

		for (auto textureProp : material.m_Textures)
		{
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

		std::ofstream output;
		output.open(outputPath, std::ios::trunc);
		output << std::setw(4) << materialData << std::endl;
		output.close();
	}

}