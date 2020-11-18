#pragma once
#include "Akkad/Graphics/Shader.h"
namespace Akkad {
	namespace Graphics {

		class GLShader : public Shader
		{
		public:
			GLShader(const char* path);
			~GLShader();

			virtual void Bind() override;
			virtual void Unbind() override;
			virtual void SetMat4(const char* location, glm::mat4& value) override;
			virtual void SetVec3(const char* location, glm::vec3& value) override;
		private:
			unsigned int m_ResourceID;
			unsigned int m_VertexShader = -1;
			unsigned int m_FragmentShader = -1;
		};
	}
}


