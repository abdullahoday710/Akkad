#include "SceneManager.h"

#include "Serializers/SceneSerializer.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

namespace Akkad {

	void SceneManager::LoadScene(std::string sceneName)
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->Stop();
		}

		m_ActiveScene.reset(new Scene());
		auto assetManager = Application::GetAssetManager();
		std::string filePath = assetManager->GetAssetsRootPath() + "/scenes/" + sceneName + ".AKSCENE";
		SceneSerializer::Deserialize(m_ActiveScene, filePath);

		auto window = Application::GetInstance().GetWindow();

		Graphics::Rect windowRect;
		windowRect.SetBounds(window->GetWindowRectMin(), window->GetWindowRectMax());
		m_ActiveScene->SetViewportRect(windowRect);
		m_ActiveScene->SetViewportSize({ window->GetWidth(), window->GetHeight() });

		m_ActiveScene->Start();
	}

	void SceneManager::LoadSceneEditor(std::string filepath)
	{
		m_ActiveScene.reset(new Scene());
		SceneSerializer::Deserialize(m_ActiveScene, filepath);
	}
}