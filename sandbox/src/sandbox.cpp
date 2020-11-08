#include <iostream>
#include <Akkad/Akkad.h>

using namespace Akkad;
using namespace Graphics;

class SandboxLayer : public Layer {
public:
	virtual void OnAttach() override {};

	virtual void OnInit() override {

		auto platform = Application::GetRenderPlatform();
		auto shader = platform->CreateShader("res/shaders/test.glsl");


		float vertices[] = {
			// positions             // texture coords
			 0.5f,  0.5f, 0.0f,      1.0f, 1.0f,   // top right
			 0.5f, -0.5f, 0.0f,      1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,      0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,      0.0f, 1.0f    // top left 
		};

		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
		auto vb = platform->CreateVertexBuffer();
		auto ib = platform->CreateIndexBuffer();

		BufferLayout layout;
		layout.Push(ShaderDataType::FLOAT, 3);
		layout.Push(ShaderDataType::FLOAT, 2);

		vb->SetLayout(layout);
		vb->SetData(vertices, sizeof(vertices));

		ib->SetData(indices, sizeof(indices));
		auto texture = platform->CreateTexture("res/textures/container.jpg");

		m_ib = ib;
		m_vb = vb;
		m_Shader = shader;
		m_Texture = texture;


	};
	virtual void OnDetach() override {};
	virtual void OnUpdate() override {

		m_vb->Bind();
		m_ib->Bind();


		m_Shader->Bind();
		m_Texture->Bind(0);
	};

	virtual void RenderImGui() override {};
private:
	SharedPtr<Shader> m_Shader;
	SharedPtr<Texture> m_Texture;
	SharedPtr<VertexBuffer> m_vb;
	SharedPtr<IndexBuffer> m_ib;


};
int main()
{
	SandboxLayer* layer = new SandboxLayer();
	Application::AttachLayer(layer);

	Application::Init();
	Application::Run();
}
