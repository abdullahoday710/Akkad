#pragma once
#include "Akkad/core.h"

#include <vector>
#include <map>
#include <string>

namespace Akkad {
	enum class AssetType {
		UNKNOWN, TEXTURE, SHADER, MATERIAL
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
		std::string assetID;

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

			else if (type == "material")
			{
				assetType = AssetType::MATERIAL;
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

		/*---- Asset registry handlers ----*/
		void RegisterAsset(std::string assetID, AssetDescriptor& asset);
		bool IsRegistered(std::string assetID);
		unsigned int GetSizeOfRegisteredAssets() { return m_RegisteredAssets.size(); }
		AssetDescriptor& GetDescriptorByID(std::string assetID);
		void Clear();

		void SetAssetsRootPath(std::string path) { m_AssetsRootPath = path; };
		std::string GetAssetsRootPath() { return m_AssetsRootPath; };
		/*--------------------------------*/


		/*---- Texture handlers ----*/
		SharedPtr<Graphics::Texture> GetTexture(std::string assetID);
		/*---------------------------*/

		/*----- Shader handlers -----*/
		SharedPtr<Graphics::Shader> GetShader(std::string assetID);
		AssetDescriptor GetShaderByName(std::string shaderName);
		void RemoveShader(std::string assetID);
		std::vector<AssetDescriptor> GetAllShaders();
		/*----------------------------*/

		/*---- Helper functions ----*/
		static std::string AssetTypeToStr(AssetType type);
		static AssetType GetAssetTypeFromFileExtension(std::string extension);
		/*--------------------------*/

	private:
		std::map<std::string, AssetDescriptor> m_RegisteredAssets;

		std::map<std::string, SharedPtr<Graphics::Texture>> m_LoadedTextures;
		std::map<std::string, SharedPtr<Graphics::Shader>> m_LoadedShaders;

		std::string m_AssetsRootPath;

	};
}


