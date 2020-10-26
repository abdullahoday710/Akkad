#include "Akkad/PlatformMacros.h"
#include "IWindow.h"
#include "Event.h"
namespace Akkad {
	class Application {
	public:
		static Application& GetInstance() { return s_Instance; }

		void Init();
		void Run();
		void OnEvent(Event& e);

		Window* m_Window = nullptr;
	private:
		Application() {}
		~Application();
		static Application s_Instance;
	};
}