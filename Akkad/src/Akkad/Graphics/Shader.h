#pragma once
#include "Akkad/core.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Akkad {
	namespace Graphics {

		enum class ShaderProgramType {
			VERTEX, FRAGMENT, GEOMETRY
		};

		class UniformBuffer;

		struct ShaderDescriptor {
			std::vector<Graphics::ShaderProgramType> ProgramTypes;
			std::vector<unsigned int> VertexData;
			std::vector<unsigned int> FragmentData;

		};

		class Shader
		{
		public:
			virtual void Bind() = 0;
			virtual void Unbind() = 0;
			virtual void SetMat4(const char* location, glm::mat4& value) = 0;
			virtual void SetVec3(const char* location, glm::vec3& value) = 0;

			virtual void SetUniformBuffer(SharedPtr<UniformBuffer> buffer) = 0;

			static std::vector<unsigned int> LoadSpirV(std::string spirvPath);

			static ShaderDescriptor LoadShader(const char* shaderdescPath);
		};

	}
}

