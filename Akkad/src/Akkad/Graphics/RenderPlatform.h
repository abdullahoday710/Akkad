#pragma once
namespace Akkad {
	namespace Graphics {

		enum class RenderAPI {
			OPENGL
		};

		class RenderPlatform {
		public:
			virtual RenderAPI GetRenderAPI() = 0;
			virtual void Init() = 0;
			static RenderPlatform* Create(RenderAPI api);
		};
	}
}
