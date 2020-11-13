#include <iostream>
#include <Akkad/Akkad.h>
#include <imgui.h>

using namespace Akkad;
using namespace Graphics;

class SandboxLayer : public Layer {
public:
	virtual void OnAttach() override {};

	virtual void OnInit() override {

		auto platform = Application::GetRenderPlatform();
		auto shader = platform->CreateShader("res/shaders/test.glsl");;
		auto texture = platform->CreateTexture("res/textures/container.jpg");

		m_Shader = shader;
		m_Texture = texture;


	};
	virtual void OnDetach() override {};
	virtual void OnUpdate() override {
		Renderer2D::DrawQuad(m_Shader, m_Texture);
	};

	virtual void RenderImGui() override {
		ImGui::ShowDemoWindow();
	};
private:
	SharedPtr<Shader> m_Shader;
	SharedPtr<Texture> m_Texture;
};
int main()
{
	SandboxLayer* layer = new SandboxLayer();
	Application::AttachLayer(layer);

	Application::Init();
	Application::Run();
}
