#include "SceneManager.h"

#include "Serializers/SceneSerializer.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

namespace Akkad {

	void SceneManager::LoadScene(std::string sceneName)
	{
		m_ActiveScene.reset(new Scene());
		auto assetManager = Application::GetAssetManager();
		std::string filePath = assetManager->GetAssetsRootPath() + "/scenes/" + sceneName + ".AKSCENE";
		SceneSerializer::Deserialize(m_ActiveScene, filePath);
		m_ActiveScene->Start();
	}

	void SceneManager::LoadSceneEditor(std::string filepath)
	{
		m_ActiveScene.reset(new Scene());
		SceneSerializer::Deserialize(m_ActiveScene, filepath);
	}
}