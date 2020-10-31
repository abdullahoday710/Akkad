#include "Application.h"
#include <iostream>
#ifdef AK_PLATFORM_WINDOWS
	#include "Akkad/Platforms/Desktop/Windows/Win32Window.h"
#endif

#include "Akkad/Input/KeyEvent.h"
#include "Akkad/Input/KeyCodes.h"
#include "Akkad/Graphics/RenderPlatform.h"
#include "imgui.h"

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

			#ifdef AK_ENABLE_IMGUI
				SharedPtr<ImGuiHandler> imgui_handler = ImGuiHandler::create(RenderAPI::OPENGL);
				imgui_handler->Init();
				m_ImguiHandler = imgui_handler;
			#endif // AK_ENABLE_IMGUI

			

			m_platform = platform;
			
		#endif 

	}

	void Application::Run()
	{
		while (!m_Window->IsCloseRequested())
		{

			m_platform->Clear();
			#ifdef AK_ENABLE_IMGUI
				m_ImguiHandler->NewFrame();

				m_ImguiHandler->Render();
			#endif
			
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
		return true;
	}
}