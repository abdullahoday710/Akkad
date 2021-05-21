#pragma once
#include <Akkad/core.h>

#include "Akkad/ECS/Scene.h"

namespace Akkad {
	class SceneSerializer
	{
	public:
		static void Serialize(SharedPtr<Scene> scene, std::string& outputPath);
		static void Deserialize(SharedPtr<Scene> scene, std::string filepath);

	};
}