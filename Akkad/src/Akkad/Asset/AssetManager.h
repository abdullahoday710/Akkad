#pragma once
#include "Akkad/core.h"

#include <vector>
#include <map>
#include <string>

namespace Akkad {

	// forward declarations
	namespace Graphics {
		class Texture;
		class Shader;
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
		unsigned int GetSizeOfRegisteredAssets() { return m_RegisteredAssets.size(); }
		void Clear();

		AssetDescriptor GetDescriptorByID(std::string assetID);

		SharedPtr<Graphics::Texture> GetTexture(std::string assetID);
		SharedPtr<Graphics::Shader> GetShader(std::string assetID);

	private:
		std::map<std::string, AssetDescriptor> m_RegisteredAssets;

		std::map<std::string, SharedPtr<Graphics::Texture>> m_LoadedTextures;
		std::map<std::string, SharedPtr<Graphics::Shader>> m_LoadedShaders;

	};
}


