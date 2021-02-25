#pragma once
#include "Akkad/core.h"

#include <vector>
#include <map>
#include <string>

namespace Akkad {
	enum class AssetType {
		UNKNOWN, TEXTURE, SHADER
	};
	// forward declarations
	namespace Graphics {
		class Texture;
		class Shader;
	}

	struct AssetDescriptor
	{
		std::string assetName;
		std::string absolutePath;

		AssetType assetType = AssetType::UNKNOWN;

		void SetAssetType(std::string type) {

			if (type == "texture")
			{
				assetType = AssetType::TEXTURE;
			}

			else if (type == "shader")
			{
				assetType = AssetType::SHADER;
			}

			else if (type == "unknown")
			{
				assetType = AssetType::UNKNOWN;
			}
		}
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

		std::vector<AssetDescriptor> GetAllShaders();

		static std::string AssetTypeToStr(AssetType type);
		static AssetType GetAssetTypeFromFileExtension(std::string extension);

	private:
		std::map<std::string, AssetDescriptor> m_RegisteredAssets;

		std::map<std::string, SharedPtr<Graphics::Texture>> m_LoadedTextures;
		std::map<std::string, SharedPtr<Graphics::Shader>> m_LoadedShaders;

	};
}


