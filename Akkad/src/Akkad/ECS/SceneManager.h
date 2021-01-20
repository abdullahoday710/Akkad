#pragma once
#include "Scene.h"

#include "Akkad/core.h"
namespace Akkad {

	class SceneManager
	{
	public:
		void LoadScene(std::string filepath);

		SharedPtr<Scene> GetActiveScene() { return m_ActiveScene; };

	private:

		SharedPtr<Scene> m_ActiveScene;

		friend class EditorLayer;
		friend class ViewPortPanel;
	};

}

