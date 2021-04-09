#pragma once
#include "RenderPlatform.h"
#include "Material.h"
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
			static void DrawQuad(Material& material, glm::mat4& transform) { GetInstance().DrawQuadImpl(material, transform); }
			static void DrawQuad(glm::vec3 color, glm::mat4& transform) { GetInstance().DrawQuadImpl(color, transform); }
			static void Draw(SharedPtr<VertexBuffer> vb, SharedPtr<Shader> shader, unsigned int vertexCount) { GetInstance().DrawImpl(vb, shader, vertexCount); };
			static void InitShaders() { GetInstance().InitShadersImpl(); }
			static SharedPtr<UniformBuffer> GetSystemUniforms() {return GetInstance().m_SceneProps;};

		private:
			Renderer2D() {};
			~Renderer2D() {};

			static Renderer2D s_Instance;

			void InitImpl();
			void BeginSceneImpl(Camera& camera, glm::mat4& cameraTransform);
			void DrawQuadImpl(SharedPtr<Texture> texture, glm::mat4& transform);
			void DrawQuadImpl(Material& material, glm::mat4& transform);
			void DrawQuadImpl(glm::vec3 color, glm::mat4& transform);
			void DrawImpl(SharedPtr<VertexBuffer> vb, SharedPtr<Shader> shader, unsigned int vertexCount);
			void InitShadersImpl();

			SharedPtr<VertexBuffer> m_QuadVB;
			SharedPtr<IndexBuffer> m_QuadIB;
			SharedPtr<UniformBuffer> m_SceneProps;

			SharedPtr<Shader> m_ColorShader;
			SharedPtr<UniformBuffer> m_ColorShaderProps;
		};
	}
}


