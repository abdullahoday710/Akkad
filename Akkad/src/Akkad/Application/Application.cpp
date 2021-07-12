#include "Application.h"
#include "Time.h"

#include "Akkad/PlatformMacros.h"

#ifdef AK_PLATFORM_WINDOWS
	#include "Akkad/Platforms/Desktop/Windows/Win32Window.h"
	#include "Akkad/Platforms/Desktop/Windows/Win32GameAssembly.h"
	#include "Akkad/Platforms/Desktop/Windows/Win32Time.h"
	#include "Akkad/Platforms/Desktop/Windows/Win32Input.h"
#endif

#include "Akkad/Logging.h"
#include "Akkad/Graphics/ImGuiHandler.h"
#include "Akkad/Graphics/RenderPlatform.h"
#include "Akkad/Graphics/Renderer2D.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/ECS/SceneManager.h"

namespace Akkad {
	using namespace Graphics;

	Application Application::s_Instance;

	void Application::InitImpl(ApplicationSettings& settings)
	{
		#ifdef AK_PLATFORM_WINDOWS
			Win32Window* window = new Win32Window();
			EventFN event_cb = std::bind(&Application::OnEvent, this, std::placeholders::_1);

			window->SetEventCallback(event_cb);
			window->Init(settings.window_settings);

			m_ApplicationComponents.m_Window = window;
			m_ApplicationComponents.m_InputManager = new Win32Input();
			m_ApplicationComponents.m_TimeManager = new Win32TimeManager();
			m_LoadedGameAssembly = new Win32GameAssembly();

		#endif //AK_PLATFORM_WINDOWS
			
		m_ApplicationComponents.m_platform = RenderPlatform::Create(RenderAPI::OPENGL);
		m_ApplicationComponents.m_platform->Init();

		m_ApplicationComponents.m_AssetManager = CreateSharedPtr<AssetManager>();
		m_ApplicationComponents.m_SceneManager = CreateSharedPtr<SceneManager>();

		Renderer2D::Init();
		m_ApplicationComponents.m_Renderer2D = &Renderer2D::GetInstance();
		m_Running = true;

		#ifdef AK_ENABLE_IMGUI
			m_ApplicationComponents.m_ImguiHandler = ImGuiHandler::create(RenderAPI::OPENGL);
			m_ApplicationComponents.m_ImguiHandler->Init();
		#endif // AK_ENABLE_IMGUI
	}

	void Application::RunImpl()
	{
		for (auto layer : m_Layers)
		{
			layer->OnAttach();
		}

		while (m_Running)
		{
			for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
			{
				auto layer = *it;
				layer->OnUpdate();

				#ifdef AK_ENABLE_IMGUI
					m_ApplicationComponents.m_ImguiHandler->NewFrame();
					layer->RenderImGui();
					m_ApplicationComponents.m_ImguiHandler->Render();
					m_ApplicationComponents.m_ImguiHandler->UpdateRenderPlatforms();
				#endif
			}

			m_ApplicationComponents.m_TimeManager->CalculateDeltaTime();
			m_ApplicationComponents.m_platform->GetRenderContext()->SwapWindowBuffers();
			m_ApplicationComponents.m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
	}

	Application::~Application()
	{
		for (auto layer : m_Layers)
		{
			layer->OnDetach();
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		for (auto layer : m_Layers)
		{
			layer->OnWindowResize(e);
		}

		m_ApplicationComponents.m_platform->OnWindowResize(e.m_Width, e.m_Height);
		return true;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}
}