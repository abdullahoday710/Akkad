#pragma once
#include <vector>
#include <string>

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

			static ShaderDescriptor LoadFile(const char* path);
		};

	}
}

