#pragma once

#include <glm/glm.hpp>
#include <vector>

struct LineRendererComponent {
	glm::vec3 color;
	std::vector<glm::vec4> lines;
	bool isActive = true;
};