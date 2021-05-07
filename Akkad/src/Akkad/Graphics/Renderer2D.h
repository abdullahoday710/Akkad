#pragma once
#include "RenderPlatform.h"
#include "Material.h"
#include "Camera.h"
#include "Rect.h"

namespace Akkad {

	namespace GUI {
		class GUIText;
	}

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

			static void DrawRect(glm::vec2 min, glm::vec2 max, glm::vec3 color) { GetInstance().DrawRectImpl(min, max, color); }
			static void DrawRect(Rect rect, glm::vec3 color) { GetInstance().DrawRectImpl(rect, color); }
			static void DrawRect(Rect rect, glm::vec3 color, glm::mat4 projection) { GetInstance().DrawRectImpl(rect, color, projection); }

			static void Draw(SharedPtr<VertexBuffer> vb, SharedPtr<Shader> shader, unsigned int vertexCount) { GetInstance().DrawImpl(vb, shader, vertexCount); };
			static void RenderText(GUI::GUIText& text, glm::vec2 position, float scale, glm::vec3 color, glm::mat4 projection) { GetInstance().RenderTextImpl(text, position, scale, color, projection); }
			static void InitShaders() { GetInstance().InitShadersImpl(); }
			static Camera GetCamera() { return GetInstance().m_Camera; }
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

			void DrawRectImpl(glm::vec2 min, glm::vec2 max, glm::vec3 color);
			void DrawRectImpl(glm::vec2 min, glm::vec2 max, glm::vec3 color, glm::mat4 projection);
			void DrawRectImpl(Rect rect, glm::vec3 color);
			void DrawRectImpl(Rect rect, glm::vec3 color, glm::mat4 projection);

			void DrawImpl(SharedPtr<VertexBuffer> vb, SharedPtr<Shader> shader, unsigned int vertexCount);

			void RenderTextImpl(GUI::GUIText& text, glm::vec2 position, float scale, glm::vec3 color, glm::mat4 projection);

			void InitShadersImpl();

			Camera m_Camera;
			glm::mat4 m_SceneCameraViewProjection = glm::mat4(1.0f);

			SharedPtr<VertexBuffer> m_QuadVB;
			SharedPtr<IndexBuffer> m_QuadIB;
			SharedPtr<UniformBuffer> m_SceneProps;

			SharedPtr<Shader> m_ColorShader;
			SharedPtr<UniformBuffer> m_ColorShaderProps;

			SharedPtr<VertexBuffer> m_RectVB;
			SharedPtr<Shader> m_RectShader;
			SharedPtr<UniformBuffer> m_RectShaderProps;

			SharedPtr<VertexBuffer> m_GUITextVB;
			SharedPtr<Shader> m_GUITextShader;
			SharedPtr<UniformBuffer> m_GUITextShaderProps;
		};
	}
}


