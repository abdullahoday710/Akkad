#include "Akkad/PlatformMacros.h"
#include "IWindow.h"
#include "Event.h"
#include "Akkad/Graphics/ImGuiHandler.h"

namespace Akkad {
	class Application {
	public:
		static Application& GetInstance() { return s_Instance; }

		void Init();
		void Run();
		void OnEvent(Event& e);
		Window* GetWindow() { return m_Window; }
		
	private:
		Application() {}
		~Application();
		static Application s_Instance;
		Window* m_Window = nullptr;
		bool OnWindowResize(WindowResizeEvent& e);
		Graphics::ImGuiHandler* m_ImguiHandler;
		Graphics::RenderPlatform* m_platform;
	};
}