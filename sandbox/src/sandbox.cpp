#include <iostream>
#include <Akkad/Akkad.h>
#include <imgui.h>

using namespace Akkad;
using namespace Graphics;

class SandboxLayer : public Layer {
public:
	virtual void OnAttach() override {

	};

	virtual void OnDetach() override {};
	virtual void OnUpdate() override {
		Application::GetRenderPlatform()->GetRenderCommand()->Clear();
	};

	virtual void RenderImGui() override {

		ImGui::ShowDemoWindow();
	};
};
int main()
{
	SandboxLayer* layer = new SandboxLayer();
	Application::AttachLayer(layer);
	ApplicationSettings settings;
	settings.window_settings.width = 800;
	settings.window_settings.height = 600;
	settings.window_settings.title = "sandbox";
	Application::Init(settings);
	Application::Run();
}
