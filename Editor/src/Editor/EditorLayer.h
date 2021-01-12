#pragma once
#include <Akkad/Akkad.h>
#include <Akkad/ECS/Scene.h>

#include "EditorCamera.h"
#include "ProjectSerializer.h"

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
		static ProjectDescriptor& GetActiveProject() { return s_ActiveProject; }
		static SharedPtr<Scene> GetActiveScene() { return s_ActiveScene; }
		static void SetActiveProject(ProjectDescriptor& desc) { s_ActiveProject = desc; }
		static void SaveActiveProject() { ProjectSerializer::SaveProject(s_ActiveProject); }

	private:
		SharedPtr<FrameBuffer> m_FrameBuffer;
		SharedPtr<Texture> m_Texture;
		SharedPtr<Shader> m_Shader;
		EditorCamera m_EditorCamera;

		static SharedPtr<Scene> s_ActiveScene;
		static ProjectDescriptor s_ActiveProject;

		static float s_AspectRatio;
		bool m_IsPlaying = false;
		void DrawMainMenuBar();
		void DrawViewport();
		void ApplyImGuiStyles();

		void OnScenePlay();
		void OnSceneStop();
		// TODO : change loading scenes to be handled by the engine not the editor.
		static void LoadScene(std::string& filepath);
		void LoadProject(std::string& filepath);
		void SaveActiveScene();
		void NewScene(std::string& sceneName);

		friend class AssetBrowserPanel;

	};
}


