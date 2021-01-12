#pragma once
#include "Scene.h"

namespace Akkad {
	class SceneSerializer
	{
	public:
		static void Serialize(SharedPtr<Scene> scene, std::string& outputPath);
		static void Deserialize(SharedPtr<Scene> scene, std::string filepath);

	};
}