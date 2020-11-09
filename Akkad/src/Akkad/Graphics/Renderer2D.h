#pragma once
#include "RenderPlatform.h"

namespace Akkad {

	namespace Graphics {

		class Renderer2D
		{
		public:
			static Renderer2D& GetInstance() { return s_Instance; }

			static void Init() { GetInstance().InitImpl(); }

			static void DrawQuad(SharedPtr<Shader> shader, SharedPtr<Texture> texture) { GetInstance().DrawQuadImpl(shader, texture); }
		private:
			Renderer2D() {};
			~Renderer2D() {};

			static Renderer2D s_Instance;

			void InitImpl();
			void DrawQuadImpl(SharedPtr<Shader> shader, SharedPtr<Texture> texture);

			SharedPtr<VertexBuffer> m_QuadVB;
			SharedPtr<IndexBuffer> m_QuadIB;
		};
	}
}


