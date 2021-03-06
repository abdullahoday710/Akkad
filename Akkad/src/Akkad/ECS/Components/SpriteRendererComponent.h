#pragma once
#include "Akkad/Graphics/Material.h"

#include <glm/glm.hpp>
#include <string>

namespace Akkad {

	struct SpriteRendererComponent {
		Graphics::Material material;
		std::string materialID;
	};
}