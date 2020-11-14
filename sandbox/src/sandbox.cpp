#include <iostream>
#include <Akkad/Akkad.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace Akkad;
using namespace Graphics;

class SandboxLayer : public Layer {
public:
	virtual void OnAttach() override {
		auto platform = Application::GetRenderPlatform();
		auto shader = platform->CreateShader("res/shaders/test.glsl");;
		auto texture = platform->CreateTexture("res/textures/container.jpg");

		FrameBufferDescriptor desc;

		desc.width = Application::GetInstance().GetWindow()->GetWidth();
		desc.height = Application::GetInstance().GetWindow()->GetHeight();
		desc.hasColorAttachment = true;

		auto framebuffer = platform->CreateFrameBuffer(desc);
		m_FrameBuffer = framebuffer;

		m_Shader = shader;
		m_Texture = texture;
	};

	virtual void OnDetach() override {};
	virtual void OnUpdate() override {

		m_FrameBuffer->SetSize(Application::GetInstance().GetWindow()->GetWidth(), Application::GetInstance().GetWindow()->GetHeight());
		auto command = Application::GetRenderPlatform()->GetRenderCommand();

		command->Clear(); // clear the screen
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)Time::GetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		m_Shader->Bind();
		m_Shader->SetMat4("transform", trans);

		m_FrameBuffer->Bind();
		command->Clear(); // clear the framebuffer
		Renderer2D::DrawQuad(m_Shader, m_Texture);
		m_FrameBuffer->Unbind();
	};

	virtual void RenderImGui() override {
		ImGui::Image(reinterpret_cast<void*>(m_FrameBuffer->GetColorAttachmentTexture()), ImVec2{ 800, 600 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	};
private:
	SharedPtr<Shader> m_Shader;
	SharedPtr<Texture> m_Texture;
	SharedPtr<FrameBuffer> m_FrameBuffer;
};
int main()
{
	SandboxLayer* layer = new SandboxLayer();
	Application::AttachLayer(layer);

	Application::Init();
	Application::Run();
}
