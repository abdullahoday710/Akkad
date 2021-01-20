#include "SceneManager.h"

#include "SceneSerializer.h"
namespace Akkad {

	void SceneManager::LoadScene(std::string filepath)
	{
		m_ActiveScene.reset(new Scene());
		SceneSerializer::Deserialize(m_ActiveScene, filepath);
	}
}