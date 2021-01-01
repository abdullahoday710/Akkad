#include "AssetManager.h"

#include "Akkad/Application/Application.h"
namespace Akkad {

	void AssetManager::RegisterAsset(AssetDescriptor& asset)
	{
		if (!IsRegistered(asset.assetID))
		{
			m_RegisteredAssets.push_back(asset);
		}
	}

	bool AssetManager::IsRegistered(std::string assetID)
	{
		auto it = std::find_if(m_RegisteredAssets.begin(), m_RegisteredAssets.end(), [&assetID](const AssetDescriptor& obj) {return obj.assetID == assetID; });

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
		auto it = std::find_if(m_RegisteredAssets.begin(), m_RegisteredAssets.end(), [&assetID](const AssetDescriptor& obj) {return obj.assetID == assetID; });
		if (it != m_RegisteredAssets.end())
		{
			return *it;
		}

		else
		{
			AK_ASSERT(false, "Asset does not exist !");
		}
	}

	SharedPtr<Graphics::Texture> AssetManager::GetTexture(std::string assetID)
	{
		auto it = std::find_if(m_LoadedTextures.begin(), m_LoadedTextures.end(), [&assetID](const Asset<SharedPtr<Graphics::Texture>>& obj) {return obj.descriptor.assetID == assetID; });
		if (it != m_LoadedTextures.end())
		{
			return it->asset;
		}

		else
		{
			Asset<SharedPtr<Graphics::Texture>> asset;
			asset.descriptor = GetDescriptorByID(assetID);

			asset.asset = Application::GetInstance().GetRenderPlatform()->CreateTexture(asset.descriptor.absolutePath.c_str());
			m_LoadedTextures.push_back(asset);

			return asset.asset;
		}
	}
}