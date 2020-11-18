#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Akkad {
	namespace Graphics {

		enum class ShaderProgramType {
			VERTEX, FRAGMENT, GEOMETRY
		};

		struct ShaderDescriptor {
			std::vector<ShaderProgramType> ProgramTypes;
			std::string VertexSource;
			std::string FragmentSource;
			
		};

		class Shader
		{
		public:
			virtual void Bind() = 0;
			virtual void Unbind() = 0;
			virtual void SetMat4(const char* location, glm::mat4& value) = 0;
			virtual void SetVec3(const char* location, glm::vec3& value) = 0;
			static ShaderDescriptor LoadFile(const char* path);
		};

	}
}

