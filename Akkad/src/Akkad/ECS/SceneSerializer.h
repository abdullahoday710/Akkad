#pragma once
#include "Scene.h"

namespace Akkad {
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* scene) { m_Scene = scene; };
		~SceneSerializer() {};

		void Serialize();
		void Deserialize();
	private:
		Scene* m_Scene;
	};
}