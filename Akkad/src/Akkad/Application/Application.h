#include "Akkad/PlatformMacros.h"
#include "IWindow.h"
#include "Event.h"
#include "Akkad/Graphics/ImGuiHandler.h"
#include "Akkad/Graphics/RenderCommand.h"

namespace Akkad {
	using namespace Graphics;
	class Application {
	public:
		static Application& GetInstance() { return s_Instance; }
		void OnEvent(Event& e);
		Window* GetWindow() { return m_Window; }
		
	private:
		Application() { Init(); }
		~Application();
		static Application s_Instance;

		void Init();
		void Run();

		Window* m_Window = nullptr;
		bool OnWindowResize(WindowResizeEvent& e);
		SharedPtr<ImGuiHandler> m_ImguiHandler;
		SharedPtr<RenderPlatform> m_platform;

		//temporary code
		SharedPtr<RenderCommand> m_RenderCommand;
	};
}