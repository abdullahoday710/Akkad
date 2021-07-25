#include "Application.h"
#include "TimeManager.h"

#include "Akkad/PlatformMacros.h"

#ifdef AK_PLATFORM_WINDOWS
	#include "Akkad/Platforms/Desktop/Windows/Win32Window.h"
	#include "Akkad/Platforms/Desktop/Windows/Win32GameAssembly.h"
	#include "Akkad/Platforms/Desktop/Windows/Win32Time.h"
	#include "Akkad/Platforms/Desktop/Windows/Win32Input.h"
#endif

#ifdef AK_PLATFORM_WEB
	#include "Akkad/Platforms/Web/WebPlatform.h"
	#include <emscripten.h>
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
		Window* window;
		Input* input;
		TimeManager* timeManager;
		LoadedGameAssembly* loadedGameAssembly;
		EventFN event_cb = std::bind(&Application::OnEvent, this, std::placeholders::_1);
		RenderAPI targetRenderAPI = RenderAPI::OPENGL;

		#ifdef AK_PLATFORM_WINDOWS
			window = new Win32Window();
			input = new Win32Input();
			timeManager = new Win32TimeManager();
			loadedGameAssembly = new Win32GameAssembly();
		#endif //AK_PLATFORM_WINDOWS

		#ifdef AK_PLATFORM_WEB
			window = new WebWindow();
			input = new WebInput();
			timeManager = new WebTime();
			targetRenderAPI = RenderAPI::OPENGLES;
		#endif // AK_PLATFORM_WEB


		window->SetEventCallback(event_cb);
		window->Init(settings.window_settings);

		m_ApplicationComponents.m_Window = window;
		m_ApplicationComponents.m_InputManager = input;
		m_ApplicationComponents.m_TimeManager = timeManager;
		m_LoadedGameAssembly = loadedGameAssembly;
			
		m_ApplicationComponents.m_platform = RenderPlatform::Create(targetRenderAPI);
		m_ApplicationComponents.m_platform->Init();

		m_ApplicationComponents.m_AssetManager = CreateSharedPtr<AssetManager>();
		m_ApplicationComponents.m_SceneManager = CreateSharedPtr<SceneManager>();

		Renderer2D::Init();
		m_ApplicationComponents.m_Renderer2D = &Renderer2D::GetInstance();
		m_Running = true;

		if (settings.enable_ImGui)
		{
			#ifndef AK_PLATFORM_WEB
				m_ApplicationComponents.m_ImguiHandler = ImGuiHandler::create(RenderAPI::OPENGL);
				m_ApplicationComponents.m_ImguiHandler->Init();
				m_ImGuiEnabled = true;
			#endif
			
		}
	}

	void Application::RunImpl()
	{
		for (auto layer : m_Layers)
		{
			layer->OnAttach();
		}

		#ifdef AK_PLATFORM_WEB
		emscripten_set_main_loop(&Application::Update, 0, 1);
		#endif

		#ifndef AK_PLAFORM_WEB
		while (m_Running)
		{
			Update();
		}
		#endif // !AK_PLAFORM_WEB


	}

	void Application::Update()
	{
		for (auto it = GetInstance().m_Layers.rbegin(); it != GetInstance().m_Layers.rend(); ++it)
		{
			auto layer = *it;
			layer->OnUpdate();
			if (GetInstance().m_ImGuiEnabled)
			{
				#ifndef AK_PLATFORM_WEB
				GetInstance().m_ApplicationComponents.m_ImguiHandler->NewFrame();
				layer->RenderImGui();
				GetInstance().m_ApplicationComponents.m_ImguiHandler->Render();
				GetInstance().m_ApplicationComponents.m_ImguiHandler->UpdateRenderPlatforms();
				#endif // !AK_PLATFORM_WEB
			}

		}

		GetInstance().m_ApplicationComponents.m_TimeManager->CalculateDeltaTime();
		GetInstance().m_ApplicationComponents.m_platform->GetRenderContext()->SwapWindowBuffers();
		GetInstance().m_ApplicationComponents.m_Window->OnUpdate();
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