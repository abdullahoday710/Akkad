#include "AssetManager.h"

#include "Akkad/Logging.h"
#include "Akkad/Application/Application.h"
#include "Akkad/ECS/Serializers/InstantiableEntitySerializer.h"

#include "Akkad/Graphics/Texture.h"
#include "Akkad/Graphics/Shader.h"

namespace Akkad {
	AssetInfo::~AssetInfo() {}
	void AssetManager::RegisterAsset(std::string assetID, AssetDescriptor& asset)
	{
		if (!IsRegistered(assetID))
		{
			asset.assetID = assetID;
			m_RegisteredAssets[assetID] = asset;
		}
	}

	bool AssetManager::IsRegistered(std::string assetID)
	{
		auto it = m_RegisteredAssets.find(assetID);

		if (it != m_RegisteredAssets.end())
		{
			return true;
		}

		else
		{
			return false;
		}
	}

	void AssetManager::Clear()
	{
		m_RegisteredAssets.clear();
		m_LoadedTextures.clear();
	}

	AssetDescriptor AssetManager::GetAssetByName(std::string name)
	{
		for (auto it : m_RegisteredAssets)
		{
			if (it.second.assetName == name)
			{
				return it.second;
			}
		}
		AK_ERROR("Can't find asset : {}", name);

		AssetDescriptor err;
		return err;
	}

	AssetDescriptor& AssetManager::GetDescriptorByID(std::string assetID)
	{
		auto it = m_RegisteredAssets.count(assetID);
		if (it > 0)
		{
			return m_RegisteredAssets[assetID];
		}

		else
		{
			AK_ASSERT(false, "Asset does not exist !");
		}
	}

	SharedPtr<Graphics::Texture> AssetManager::GetTexture(std::string assetID)
	{
		auto it = m_LoadedTextures.find(assetID);
		if (it != m_LoadedTextures.end())
		{
			return it->second;
		}

		else
		{
			auto desc = GetDescriptorByID(assetID);
			
			auto textureinfo = std::static_pointer_cast<TextureAssetInfo>(desc.assetInfo);
			if (textureinfo->isTilemap)
			{
				auto texture = Application::GetInstance().GetRenderPlatform()->CreateTexture(desc.absolutePath.c_str(), textureinfo->tileWidth, textureinfo->tileHeight);
				m_LoadedTextures[assetID] = texture;
				return texture;
			}

			else
			{
				auto texture = Application::GetInstance().GetRenderPlatform()->CreateTexture(desc.absolutePath.c_str());
				m_LoadedTextures[assetID] = texture;
				return texture;
			}

		}
	}

	void AssetManager::ReloadTexture(std::string assetID)
	{
		auto desc = GetDescriptorByID(assetID);

		AK_ASSERT(desc.assetType == AssetType::TEXTURE, "Failed to reload asset, asset type mismatch !");
		
		m_LoadedTextures.erase(assetID);
		GetTexture(assetID);
	}

	SharedPtr<Graphics::Shader> AssetManager::GetShader(std::string assetID)
	{
		auto it = m_LoadedShaders.find(assetID);
		if (it != m_LoadedShaders.end())
		{
			return it->second;
		}
		else
		{
			auto desc = GetDescriptorByID(assetID);
			auto shader = Application::GetInstance().GetRenderPlatform()->CreateShader(desc.absolutePath.c_str());
			m_LoadedShaders[assetID] = shader;
			
			return shader;
		}
	}

	AssetDescriptor AssetManager::GetShaderByName(std::string shaderName)
	{
		for (auto it : m_RegisteredAssets)
		{
			if (it.second.assetType == AssetType::SHADER && it.second.assetName == shaderName)
			{
				return it.second;
			}
		}

		AK_ERROR("Unable to get shader : {}", shaderName);
		AssetDescriptor err;
		return err;
		
	}

	void AssetManager::RemoveShader(std::string assetID)
	{
		auto it = m_LoadedShaders.find(assetID);
		if (it != m_LoadedShaders.end())
		{
			m_LoadedShaders.erase(assetID);
		}
	}

	std::vector<AssetDescriptor> AssetManager::GetAllShaders()
	{
		std::vector<AssetDescriptor> result;

		for (auto it : m_RegisteredAssets)
		{
			if (it.second.assetType == AssetType::SHADER)
			{
				result.push_back(it.second);
			}
		}

		return result;
	}

	AssetDescriptor AssetManager::GetFontByName(std::string fontname)
	{
		for (auto it : m_RegisteredAssets)
		{
			if (it.second.assetType == AssetType::FONT && it.second.assetName == fontname)
			{
				return it.second;
			}
		}

		AK_ERROR("Unable to get shader : {}", fontname);
		AssetDescriptor err;
		return err;
	}

	SharedPtr<nlohmann::ordered_json> AssetManager::GetInstantiableEntityByName(std::string name)
	{
		auto desc = GetAssetByName(name);
		auto it = m_LoadedInstantiableEntities.find(desc.assetID);
		if (it != m_LoadedInstantiableEntities.end())
		{
			return it->second;
		}

		else
		{
			auto data = InstantiableEntitySerializer::Deserialize(desc.absolutePath);
			auto instantiableObject = CreateSharedPtr<nlohmann::ordered_json>(data);
			m_LoadedInstantiableEntities[desc.assetID] = instantiableObject;

			return instantiableObject;
		}
	}

	std::string AssetManager::AssetTypeToStr(AssetType type)
	{
		switch (type)
		{
		case AssetType::UNKNOWN:
			return "unkown";
		case AssetType::TEXTURE:
			return "texture";
		case AssetType::SHADER:
			return "shader";
		case AssetType::FONT:
			return "font";
		case AssetType::SPRITE_ANIMATION:
			return "sprite_animation";
		default:
			return "unkown";
		}
	}

	AssetType AssetManager::GetAssetTypeFromFileExtension(std::string extension)
	{
		AssetType assetType = AssetType::UNKNOWN;

		if (extension == ".png" || extension == ".jpg")
		{
			assetType = AssetType::TEXTURE;
		}

		else if (extension == ".ttf")
		{
			assetType = AssetType::FONT;
		}

		else if (extension == ".glsl")
		{
			assetType = AssetType::SHADER;
		}

		else if (extension == ".ak_sprite_anim")
		{
			assetType = AssetType::SPRITE_ANIMATION;
		}

		else if (extension == ".akentity")
		{
			assetType == AssetType::INSTANTIABLE_ENTITY;
		}

		return assetType;
	}
}