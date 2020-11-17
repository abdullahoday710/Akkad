#pragma once
#include <Akkad/Akkad.h>
#include <Akkad/ECS/Scene.h>

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
	private:
		Scene* m_Scene;
		SharedPtr<FrameBuffer> m_FrameBuffer;
		SharedPtr<Texture> m_Texture;
		SharedPtr<Shader> m_Shader;

	};
}


