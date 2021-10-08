#include <Akkad/Akkad.h>

#include <Akkad/Graphics/Camera.h>

#include <Akkad/Platforms/Web/WebGameAssembly.h>
#include <dlfcn.h>
#include <emscripten.h>
using namespace Akkad;
using namespace Graphics;

class SandboxLayer : public Layer {
	SharedPtr<Graphics::Shader> m_Shader;
	Camera m_cam;
	void* dll;
public:

	virtual void OnAttach() override {
		WebGameAssembly* gameassembly = (WebGameAssembly*)Application::GetGameAssembly();
		std::cout << "before lib" << std::endl;
		const char* filename = "GameAssembly";
		gameassembly->LoadAssembly(filename);
		std::cout << "after lib" << std::endl;

		auto scripts = gameassembly->GetScripts();

		for (auto script : scripts)
		{
			std::cout << script << std::endl;
		}
		auto platform = Application::GetRenderPlatform();
		auto assetManager = Application::GetAssetManager();
		assetManager->SetAssetsRootPath("assets/");

		AssetDescriptor r2d_colorshader;
		r2d_colorshader.absolutePath = "assets/compiledSPV/r2d_colorShader.shaderdesc";
		r2d_colorshader.assetID = "1";
		r2d_colorshader.assetName = "r2d_colorShader";
		r2d_colorshader.assetType = AssetType::SHADER;
		assetManager->RegisterAsset("1", r2d_colorshader);

		AssetDescriptor r2d_rectshader;
		r2d_rectshader.absolutePath = "assets/compiledSPV/r2d_rectShader.shaderdesc";
		r2d_rectshader.assetID = "2";
		r2d_rectshader.assetName = "r2d_rectShader";
		r2d_rectshader.assetType = AssetType::SHADER;
		assetManager->RegisterAsset("2", r2d_rectshader);

		AssetDescriptor r2d_lineshader;
		r2d_lineshader.absolutePath = "assets/compiledSPV/r2d_lineShader.shaderdesc";
		r2d_lineshader.assetID = "3";
		r2d_lineshader.assetName = "r2d_lineShader";
		r2d_lineshader.assetType = AssetType::SHADER;
		assetManager->RegisterAsset("3", r2d_lineshader);

		AssetDescriptor r2d_texturedrect;
		r2d_texturedrect.absolutePath = "assets/compiledSPV/r2d_texturedRect.shaderdesc";
		r2d_texturedrect.assetID = "4";
		r2d_texturedrect.assetName = "r2d_texturedRect";
		r2d_texturedrect.assetType = AssetType::SHADER;
		assetManager->RegisterAsset("4", r2d_texturedrect);
		Renderer2D::InitShaders();

		

	};
	virtual void OnDetach() override {};
	virtual void OnUpdate() override {
		glm::mat4 transform = glm::mat4(1.0f);
		glm::translate(transform, { 0,0,0 });
		Renderer2D::BeginScene(m_cam, transform);
		Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.0f }, transform);

	};

	virtual void RenderImGui() override {

		//ImGui::ShowDemoWindow();
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
