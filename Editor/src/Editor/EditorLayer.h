#pragma once
#include <Akkad/Akkad.h>

namespace Akkad {

	class EditorLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate() override;

		virtual void RenderImGui() override;
	private:
		SharedPtr<FrameBuffer> m_FrameBuffer;
		SharedPtr<Texture> m_Texture;
		SharedPtr<Shader> m_Shader;

	};
}


