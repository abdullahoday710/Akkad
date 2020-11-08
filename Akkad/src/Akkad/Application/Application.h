#include "Akkad/PlatformMacros.h"
#include "IWindow.h"
#include "Event.h"
#include "Akkad/Graphics/ImGuiHandler.h"
#include "Akkad/Graphics/RenderCommand.h"
#include "Layer.h"

namespace Akkad {
	using namespace Graphics;
	class Application {
	public:
		static Application& GetInstance() { return s_Instance; }

		static void AttachLayer(Layer* layer) { GetInstance().AttachLayerImpl(layer); }
		static void DetachLayer(Layer* layer) { GetInstance().DetachLayerImpl(layer); }
		static void Run() { GetInstance().RunImpl(); }
		static void Init() { GetInstance().InitImpl(); }
		static SharedPtr<RenderPlatform> GetRenderPlatform() { return GetInstance().m_platform; }
		Window* GetWindow() { return m_Window; }
		
	private:
		Application() {}
		~Application();
		static Application s_Instance;

		void InitImpl();
		void RunImpl();

		void OnEvent(Event& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void AttachLayerImpl(Layer* layer) {
			layer->OnAttach();
			m_Layers.push_back(layer);
		}

		void DetachLayerImpl(Layer* layer) {
			layer->OnDetach();
			m_Layers.erase(std::remove(m_Layers.begin(), m_Layers.end(), layer), m_Layers.end());
		}

		std::vector<Layer*> m_Layers;

		Window* m_Window = nullptr;

		SharedPtr<ImGuiHandler> m_ImguiHandler;
		SharedPtr<RenderPlatform> m_platform;
		//temporary code
		SharedPtr<RenderCommand> m_RenderCommand;
	};
}