#pragma once
#include "Scene.h"

#include "Akkad/core.h"
namespace Akkad {

	class SceneManager
	{
	public:
		void LoadScene(std::string sceneName);

		SharedPtr<Scene> GetActiveScene() { return m_ActiveScene; };

	private:
		void LoadSceneEditor(std::string filepath);
		SharedPtr<Scene> m_ActiveScene;

		friend class EditorLayer;
		friend class ViewPortPanel;
	};

}

