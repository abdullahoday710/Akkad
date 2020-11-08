#include "Application.h"
#include <iostream>
#ifdef AK_PLATFORM_WINDOWS
	#include "Akkad/Platforms/Desktop/Windows/Win32Window.h"
#endif

#include "Akkad/Input/KeyEvent.h"
#include "Akkad/Input/KeyCodes.h"
#include "imgui.h"
#include "Akkad/Graphics/Buffer.h"
#include "Akkad/Graphics/Shader.h"
#include "glad/glad.h"
namespace Akkad {
	Application Application::s_Instance;

	void Application::Init()
	{
		#ifdef AK_PLATFORM_WINDOWS
			Win32Window* window = new Win32Window();
			EventFN event_cb = std::bind(&Application::OnEvent, this, std::placeholders::_1);

			window->SetEventCallback(event_cb);
			window->Init({"engine", 800, 600});
			m_Window = window;

			SharedPtr<RenderPlatform> platform = RenderPlatform::Create(RenderAPI::OPENGL);
			platform->Init();
			m_platform = platform;

			m_RenderCommand = RenderCommand::Create(RenderAPI::OPENGL);

			#ifdef AK_ENABLE_IMGUI
				SharedPtr<ImGuiHandler> imgui_handler = ImGuiHandler::create(RenderAPI::OPENGL);
				imgui_handler->Init();
				m_ImguiHandler = imgui_handler;
			#endif // AK_ENABLE_IMGUI
			
		#endif 

	}

	void Application::Run()
	{
		auto shader = m_platform->CreateShader("res/shaders/test.glsl");


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
		auto vb = m_platform->CreateVertexBuffer();
		auto ib = m_platform->CreateIndexBuffer();

		BufferLayout layout;
		layout.Push(ShaderDataType::FLOAT, 3);
		layout.Push(ShaderDataType::FLOAT, 2);

		vb->SetLayout(layout);
		vb->SetData(vertices, sizeof(vertices));

		ib->SetData(indices, sizeof(indices));

		vb->Bind();
		ib->Bind();

		auto texture = m_platform->CreateTexture("res/textures/container.jpg");

		shader->Bind();
		texture->Bind(0);

		while (!m_Window->IsCloseRequested())
		{
			m_RenderCommand->Clear();
			#ifdef AK_ENABLE_IMGUI
				m_ImguiHandler->NewFrame();

				m_ImguiHandler->Render();
			#endif
				// temp code
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			m_Window->SwapWindowBuffers();
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
	}

	Application::~Application()
	{
		delete m_Window;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		m_platform->OnWindowResize(e.m_Width, e.m_Height);
		return true;
	}
}