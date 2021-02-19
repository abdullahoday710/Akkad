#pragma once
#include "RenderPlatform.h"
#include "Camera.h"

namespace Akkad {

	namespace Graphics {

		class Renderer2D
		{
		public:
			static Renderer2D& GetInstance() { return s_Instance; }

			static void Init() { GetInstance().InitImpl(); }

			static void BeginScene(Camera& camera, glm::mat4& cameraTransform) { GetInstance().BeginSceneImpl(camera, cameraTransform); }
			static void DrawQuad(SharedPtr<Texture> texture, glm::mat4& transform) { GetInstance().DrawQuadImpl(texture, transform); }

		private:
			Renderer2D() {};
			~Renderer2D() {};

			static Renderer2D s_Instance;

			void InitImpl();
			void BeginSceneImpl(Camera& camera, glm::mat4& cameraTransform);
			void DrawQuadImpl(SharedPtr<Texture> texture, glm::mat4& transform);

			SharedPtr<VertexBuffer> m_QuadVB;
			SharedPtr<IndexBuffer> m_QuadIB;
			SharedPtr<Shader> m_TextureShader;
			SharedPtr<Shader> m_ColorShader;
			SharedPtr<UniformBuffer> m_SceneProps;
		};
	}
}


