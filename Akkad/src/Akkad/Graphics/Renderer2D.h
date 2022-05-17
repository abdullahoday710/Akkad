#pragma once
#include "RenderPlatform.h"
#include "Material.h"
#include "Camera.h"
#include "Rect.h"
#include "Sprite.h"

namespace Akkad {

	class GameAssembly;

	namespace GUI {
		class GUIText;
	}

	namespace Graphics {

		class Renderer2D
		{
		public:
			struct QuadVertex {
				glm::vec3 position;
				glm::vec3 color;
				glm::mat4 transform;
			};

			struct QuadInstance
			{
				glm::vec3 color;
				glm::mat4 transform;
			};

			struct LineVertex {
				glm::vec2 position;
				glm::vec3 color;
			};
			static Renderer2D& GetInstance() { return s_Instance; }

			static void Init() { GetInstance().InitImpl(); }
			static void BeginScene(Camera& camera, glm::mat4& cameraTransform) { GetInstance().BeginSceneImpl(camera, cameraTransform); }
			static void EndScene() { GetInstance().EndSceneImpl(); }

			static void DrawQuad(SharedPtr<Texture> texture, glm::mat4& transform) { GetInstance().DrawQuadImpl(texture, transform); }
			static void DrawQuad(Material& material, glm::mat4& transform) { GetInstance().DrawQuadImpl(material, transform); }
			static void DrawQuad(glm::vec3 color, glm::mat4& transform) { GetInstance().DrawQuadImpl(color, transform); }
			static void DrawQuad(glm::vec3 color, glm::mat4& transform, glm::mat4 projection) { GetInstance().DrawQuadImpl(color, transform, projection); }
			static void DrawRect(glm::vec2 min, glm::vec2 max, glm::vec3 color, bool filled) { GetInstance().DrawRectImpl(min, max, color, filled); }
			static void DrawRect(Rect rect, glm::vec3 color, bool filled) { GetInstance().DrawRectImpl(rect, color, filled); }
			static void DrawRect(Rect rect, glm::vec3 color, bool filled, glm::mat4 projection) { GetInstance().DrawRectImpl(rect, color, filled, projection); }

			static void DrawColoredQuadInstanced(glm::vec3 color, glm::mat4& transform) { GetInstance().DrawColoredQuadInstancedImpl(color, transform); }
			static void DrawSprite(Sprite& sprite, glm::mat4& transform) { GetInstance().DrawSpriteImpl(sprite, transform); };
			static void DrawAnimatedSprite(AnimatedSprite& sprite, AnimationFrame& frame, glm::mat4& transform) { GetInstance().DrawAnimatedSpriteImpl(sprite, frame, transform); };

			static void DrawLine(glm::vec2 point1, glm::vec2 point2, glm::vec3 color) { GetInstance().DrawLineImpl(point1, point2, color); }
			static void DrawLine(glm::vec2 point1, glm::vec2 point2, glm::vec3 color, glm::mat4& projection) { GetInstance().DrawLineImpl(point1, point2, color, projection); }

			static void Draw(SharedPtr<VertexBuffer> vb, SharedPtr<Shader> shader, unsigned int vertexCount) { GetInstance().DrawImpl(vb, shader, vertexCount); };
			static void RenderText(GUI::GUIText& uitext, glm::mat4 projection) { GetInstance().RenderTextImpl(uitext, projection); }
			static void InitShaders() { GetInstance().InitShadersImpl(); }
			static Camera GetCamera() { return GetInstance().m_Camera; }
			static SharedPtr<UniformBuffer> GetSystemUniforms() {return GetInstance().m_SceneProps;};

			static bool GetGUIDebugDrawState() { return GetInstance().m_DrawDebugGUIRects; }
			static void SetGUIDebugDrawState(bool state) { GetInstance().m_DrawDebugGUIRects = state; }

			static bool GetPhysicsDebugDrawState() { return GetInstance().m_DrawDebugPhysics; }
			static void SetPhysicsDebugDrawState(bool state) { GetInstance().m_DrawDebugPhysics = state; }

		private:
			Renderer2D() {};
			~Renderer2D() {};

			static Renderer2D s_Instance;

			void InitImpl();
			void BeginSceneImpl(Camera& camera, glm::mat4& cameraTransform);
			void EndSceneImpl();

			void DrawQuadImpl(SharedPtr<Texture> texture, glm::mat4& transform);
			void DrawQuadImpl(Material& material, glm::mat4& transform);
			void DrawQuadImpl(glm::vec3 color, glm::mat4& transform);
			void DrawQuadImpl(glm::vec3 color, glm::mat4& transform, glm::mat4 projection);

			void DrawRectImpl(glm::vec2 min, glm::vec2 max, glm::vec3 color, bool filled);
			void DrawRectImpl(glm::vec2 min, glm::vec2 max, glm::vec3 color, bool filled, glm::mat4 projection);
			void DrawRectImpl(Rect rect, glm::vec3 color, bool filled);
			void DrawRectImpl(Rect rect, glm::vec3 color, bool filled, glm::mat4 projection);
			void DrawRectImpl(Rect rect, SharedPtr<Texture> texture, glm::mat4 projection);

			void DrawSpriteImpl(Sprite& sprite, glm::mat4& transform);
			void DrawAnimatedSpriteImpl(AnimatedSprite& sprite, AnimationFrame& frame, glm::mat4& transform);

			void DrawLineImpl(glm::vec2 point1, glm::vec2 point2, glm::vec3 color);
			void DrawLineImpl(glm::vec2 point1, glm::vec2 point2, glm::vec3 color, glm::mat4& projection);

			void StartBatch();
			void NewBatch();
			void FlushBatch();

			void StartLineBatch();
			void NewLineBatch();
			void FlushLineBatch();

			void DrawImpl(SharedPtr<VertexBuffer> vb, SharedPtr<Shader> shader, unsigned int vertexCount);

			void RenderTextImpl(GUI::GUIText& uitext, glm::mat4 projection);

			void InitShadersImpl();

			void StartColoredQuadInstancedImpl();
			void DrawColoredQuadInstancedImpl(glm::vec3 color, glm::mat4& transform);
			void FlushColoredQuadInstancedImpl();

			bool m_DrawDebugGUIRects = true;
			bool m_DrawDebugPhysics = true;

			Camera m_Camera;
			glm::mat4 m_SceneCameraViewProjection = glm::mat4(1.0f);

			SharedPtr<VertexBuffer> m_QuadVB;
			SharedPtr<IndexBuffer> m_QuadIB;

			enum {MAX_BATCH_QUADS = 1000, MAX_BATCH_VERTS = MAX_BATCH_QUADS * 4, MAX_BATCH_INDICES = MAX_BATCH_QUADS * 6};
			SharedPtr<VertexBuffer> m_BatchVB;
			SharedPtr<IndexBuffer> m_BatchIB;

			glm::mat4 m_InstanceMatrices[MAX_BATCH_QUADS];
			QuadInstance* m_QuadInstanceData = nullptr;
			unsigned int m_QuadInstanceAmount = 0;
			
			SharedPtr<VertexBuffer> m_InstanceVB;

			QuadVertex* m_QuadBatchData = nullptr;
			QuadVertex* m_LastQuadVertexPtr = nullptr;
			QuadInstance* m_LastQuadInstancePtr = nullptr;
			glm::vec4 m_QuadVertexPositions[4] = {};
			unsigned int m_QuadBatchIndexCount = 0;

			SharedPtr<VertexBuffer> m_LineVB;
			LineVertex* m_LineBatchData = nullptr;
			LineVertex* m_LastLineVertexPtr = nullptr;
			unsigned int m_LineBatchVertexCount = 0;

			SharedPtr<Shader> m_LineShader;
			SharedPtr<UniformBuffer> m_LineShaderProps;

			SharedPtr<UniformBuffer> m_SceneProps;

			SharedPtr<Shader> m_ColorShader;
			SharedPtr<UniformBuffer> m_ColorShaderProps;

			SharedPtr<VertexBuffer> m_RectVB;
			SharedPtr<Shader> m_RectShader;
			SharedPtr<UniformBuffer> m_RectShaderProps;
			SharedPtr<Shader> m_TexturedRectShader;
			SharedPtr<UniformBuffer> m_TexturedRectShaderProps;

			SharedPtr<VertexBuffer> m_GUITextVB;
			SharedPtr<IndexBuffer> m_GUITextIB;
			SharedPtr<Shader> m_GUITextShader;
			SharedPtr<UniformBuffer> m_GUITextShaderProps;

			friend class ::Akkad::GameAssembly;
		};
	}
}


