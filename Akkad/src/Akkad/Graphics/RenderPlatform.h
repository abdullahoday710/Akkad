#pragma once
#include "Akkad/core.h"
#include "Buffer.h"

namespace Akkad {
	namespace Graphics {

		enum class RenderAPI {
			OPENGL
		};

		class RenderPlatform {
		public:
			virtual RenderAPI GetRenderAPI() = 0;
			virtual void Init() = 0;

			virtual SharedPtr<VertexBuffer> CreateVertexBuffer() = 0;
			virtual SharedPtr<IndexBuffer> CreateIndexBuffer() = 0;

			static SharedPtr<RenderPlatform> Create(RenderAPI api);
		};
	}
}
