#pragma once
#include "Scene.h"

namespace Akkad {
	class SceneSerializer
	{
	public:
		static void Serialize(Scene* scene);
		static Scene* Deserialize(std::string filepath);

	};
}