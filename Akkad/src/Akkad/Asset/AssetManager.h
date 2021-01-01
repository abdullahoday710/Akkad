#pragma once
#include "Akkad/core.h"
#include "Akkad/Graphics/Texture.h"

#include <vector>
#include <string>

namespace Akkad {

	struct AssetDescriptor
	{
		std::string absolutePath;
		std::string assetID;
	};

	template<typename T>
	struct Asset
	{
		AssetDescriptor descriptor;
		T asset;
	};

	class AssetManager
	{
	public:
		AssetManager() {};
		~AssetManager() {};

		void RegisterAsset(AssetDescriptor& asset);
		bool IsRegistered(std::string assetID);
		void Clear();

		AssetDescriptor GetDescriptorByID(std::string assetID);

		SharedPtr<Graphics::Texture> GetTexture(std::string assetID);

	private:
		std::vector<AssetDescriptor> m_RegisteredAssets;

		std::vector<Asset<SharedPtr<Graphics::Texture>>> m_LoadedTextures;
	};
}


