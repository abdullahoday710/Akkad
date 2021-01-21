#pragma once
#include <Akkad/ECS/Scene.h>
#include <Akkad/Application/Layer.h>

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

		static std::string GetActiveScenePath() { return GetActiveProject().GetScenePath(s_ActiveScene->m_Name); }

	private:
		static SharedPtr<Scene> s_ActiveScene;
		static ProjectDescriptor s_ActiveProject;

		static float s_AspectRatio;
		bool m_IsPlaying = false;
		void DrawMainMenuBar();
		void ApplyImGuiStyles();

		static void LoadScene(std::string& filepath);
		void LoadProject();
		void SaveActiveScene();

		friend class AssetBrowserPanel;
		friend class NewScenePanel;
		friend class StartupPanel;

	};
}


