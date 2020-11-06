#pragma once
#include "Akkad/core.h"
#include "Buffer.h"
#include "Shader.h"

namespace Akkad {
	namespace Graphics {

		enum class RenderAPI {
			OPENGL
		};

		class RenderPlatform {
		public:
			virtual RenderAPI GetRenderAPI() = 0;
			virtual void Init() = 0;
			virtual void OnWindowResize(unsigned int width, unsigned int height) = 0;

			virtual SharedPtr<VertexBuffer> CreateVertexBuffer() = 0;
			virtual SharedPtr<IndexBuffer> CreateIndexBuffer() = 0;
			virtual SharedPtr<Shader> CreateShader(const char* path) = 0;

			static SharedPtr<RenderPlatform> Create(RenderAPI api);
		};
	}
}
