#pragma once
#include <Akkad/Akkad.h>
#include <Akkad/ECS/Scene.h>

#include "EditorCamera.h"

#include <vector>

namespace Akkad {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate() override;

		virtual void RenderImGui() override;

		static float GetViewportAspectRatio() { return s_AspectRatio; }
		static Scene* s_ActiveScene;
	private:
		SharedPtr<FrameBuffer> m_FrameBuffer;
		SharedPtr<Texture> m_Texture;
		SharedPtr<Shader> m_Shader;
		EditorCamera m_EditorCamera;

		static float s_AspectRatio;
		bool m_IsPlaying = false;
		void ApplyImGuiStyles();

		void OnScenePlay();
		void OnSceneStop();
		void LoadScene(std::string& filepath);
		void SaveActiveScene();
		void NewScene(std::string& sceneName);

	};
}


