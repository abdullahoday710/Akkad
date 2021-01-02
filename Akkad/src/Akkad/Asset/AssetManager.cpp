#include "AssetManager.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Texture.h"

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

			auto asset = Application::GetInstance().GetRenderPlatform()->CreateTexture(desc.absolutePath.c_str());
			m_LoadedTextures[assetID] = asset;

			return asset;
		}
	}
}