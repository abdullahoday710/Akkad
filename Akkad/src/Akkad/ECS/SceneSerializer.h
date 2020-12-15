#pragma once
#include "Scene.h"

namespace Akkad {
	class SceneSerializer
	{
	public:
		static void Serialize(Scene* scene, std::string& outputPath);
		static Scene* Deserialize(std::string filepath);

	};
}