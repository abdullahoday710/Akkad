#include "Application.h"
#include <iostream>
#ifdef AK_PLATFORM_WINDOWS
	#include "Akkad/Platforms/Desktop/Windows/Win32Window.h"
#endif

#include "Akkad/Input/KeyEvent.h"
#include "Akkad/Input/KeyCodes.h"
#include "Akkad/Graphics/RenderPlatform.h"
namespace Akkad {
	Application Application::s_Instance;
	void Application::Init()
	{
		#ifdef AK_PLATFORM_WINDOWS
			Win32Window* window = new Win32Window();
			window->Init({"engine", 800, 600});

			std::function<void(Event&)> event_cb = std::bind(&Application::OnEvent, this, std::placeholders::_1);
			window->SetEventCallback(event_cb);

			m_Window = window;

			Graphics::RenderPlatform* platform = Graphics::RenderPlatform::Create(Graphics::RenderAPI::OPENGL);
			platform->Init();
			
		#endif 

	}

	void Application::Run()
	{
		while (!m_Window->IsCloseRequested())
		{
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
	}

	Application::~Application()
	{
		delete m_Window;
	}
}