#include "AssetManager.h"

#include "Akkad/Application/Application.h"

#include "Akkad/Graphics/Texture.h"
#include "Akkad/Graphics/Shader.h"

namespace Akkad {

	void AssetManager::RegisterAsset(std::string assetID, AssetDescriptor& asset)
	{
		if (!IsRegistered(assetID))
		{
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

	AssetDescriptor AssetManager::GetDescriptorByID(std::string assetID)
	{
		auto it = m_RegisteredAssets.find(assetID);
		if (it != m_RegisteredAssets.end())
		{
			return it->second;
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

			auto texture = Application::GetInstance().GetRenderPlatform()->CreateTexture(desc.absolutePath.c_str());
			m_LoadedTextures[assetID] = texture;

			return texture;
		}
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
}