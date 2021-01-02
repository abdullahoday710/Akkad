#pragma once
#include "Akkad/core.h"

#include <vector>
#include <map>
#include <string>

namespace Akkad {

	// forward declarations
	namespace Graphics {
		class Texture;
	}

	struct AssetDescriptor
	{
		std::string absolutePath;
	};

	class AssetManager
	{
	public:
		AssetManager() {};
		~AssetManager() {};

		void RegisterAsset(std::string assetID, AssetDescriptor& asset);
		bool IsRegistered(std::string assetID);
		void Clear();

		AssetDescriptor GetDescriptorByID(std::string assetID);

		SharedPtr<Graphics::Texture> GetTexture(std::string assetID);

	private:
		std::map<std::string, AssetDescriptor> m_RegisteredAssets;

		std::map<std::string, SharedPtr<Graphics::Texture>> m_LoadedTextures;
	};
}


