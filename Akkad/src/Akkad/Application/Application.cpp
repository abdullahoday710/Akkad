#include "Application.h"
#ifdef AK_PLATFORM_WINDOWS
	#include "Akkad/Platforms/Desktop/Windows/Win32Window.h"
#endif

#include "Akkad/Graphics/Renderer2D.h"
#include "Time.h"

namespace Akkad {
	Application Application::s_Instance;

	void Application::InitImpl()
	{
		#ifdef AK_PLATFORM_WINDOWS
			Win32Window* window = new Win32Window();
			EventFN event_cb = std::bind(&Application::OnEvent, this, std::placeholders::_1);

			window->SetEventCallback(event_cb);
			window->Init({"engine", 800, 600});
			m_Window = window;
		#endif //AK_PLATFORM_WINDOWS

		SharedPtr<RenderContext> context = RenderContext::Create();
		SharedPtr<RenderPlatform> platform = RenderPlatform::Create(RenderAPI::OPENGL);
		context->Init(RenderAPI::OPENGL);
		platform->Init();
		m_platform = platform;
		m_context = context;

		#ifdef AK_ENABLE_IMGUI
			SharedPtr<ImGuiHandler> imgui_handler = ImGuiHandler::create(RenderAPI::OPENGL);
			imgui_handler->Init();
			m_ImguiHandler = imgui_handler;
		#endif // AK_ENABLE_IMGUI

		Renderer2D::Init();
		Time::Init();
		for (auto layer : m_Layers)
		{
			layer->OnInit();
		}

	}

	void Application::RunImpl()
	{
		m_context->SetVsync(0);
		while (!m_Window->IsCloseRequested())
		{
			m_platform->GetRenderCommand()->Clear();
			
			for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
			{
				auto layer = *it;
				layer->OnUpdate();

				#ifdef AK_ENABLE_IMGUI
					m_ImguiHandler->NewFrame();
					layer->RenderImGui();
					m_ImguiHandler->Render();
					m_ImguiHandler->UpdateRenderPlatforms();
				#endif
			}

			Time::CalculateDeltaTime();
			m_context->SwapWindowBuffers();
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