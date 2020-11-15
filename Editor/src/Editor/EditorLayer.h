#pragma once
#include <Akkad/Akkad.h>
#include <Akkad/ECS/Scene.h>

namespace Akkad {

	class EditorLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate() override;

		virtual void RenderImGui() override;
	private:
		SharedPtr<Scene> m_Scene;
		SharedPtr<FrameBuffer> m_FrameBuffer;
		SharedPtr<Texture> m_Texture;
		SharedPtr<Shader> m_Shader;

	};
}


