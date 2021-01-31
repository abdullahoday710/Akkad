#include "IWindow.h"
#include "Event.h"

#include "Layer.h"

namespace Akkad {

	/* ---- Forward declarations ---- */
	class AssetManager;
	class SceneManager;
	class LoadedGameAssembly;
	class TimeManager;

	namespace Graphics
	{
		class ImGuiHandler;
		class RenderPlatform;
	}
	/*----------------------------------*/

	struct ApplicationSettings
	{
		WindowSettings window_settings;
	};

	class Application {
	public:
		static Application& GetInstance() { return s_Instance; }

		static void AttachLayer(Layer* layer) { GetInstance().AttachLayerImpl(layer); }
		static void DetachLayer(Layer* layer) { GetInstance().DetachLayerImpl(layer); }

		/*----- Application Lifetime -----*/
		static void Init(ApplicationSettings& settings) { GetInstance().InitImpl(settings); }
		static void Run() { GetInstance().RunImpl(); }
		static void Shutdown() { GetInstance().m_Running = false; };
		/*------------------------------*/

		/*---- Getters -----*/
		static LoadedGameAssembly* GetGameAssembly() { return GetInstance().m_LoadedGameAssembly; }
		static TimeManager* GetTimeManager() { return GetInstance().m_TimeManager; }
		static SharedPtr<Graphics::RenderPlatform> GetRenderPlatform() { return GetInstance().m_platform; }
		static SharedPtr<AssetManager> GetAssetManager() { return GetInstance().m_AssetManager; }
		static SharedPtr<SceneManager> GetSceneManager() { return GetInstance().m_SceneManager; }
		/*------------------*/

		Window* GetWindow() { return m_Window; }
		
	private:
		friend class Window;
		Application() {}
		~Application();
		static Application s_Instance;

		void InitImpl(ApplicationSettings& settings);
		void RunImpl();
		bool m_Running = false;

		// -------- Event Handlers -----------------
		void OnEvent(Event& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);
		// -------------------------------------

		// -------- Layer Handlers ------------------
		void AttachLayerImpl(Layer* layer) {
			m_Layers.push_back(layer);
		}

		void DetachLayerImpl(Layer* layer) {
			m_Layers.erase(std::remove(m_Layers.begin(), m_Layers.end(), layer), m_Layers.end());
			delete layer;
		}
		// -----------------------------------------

		std::vector<Layer*> m_Layers;

		// -------- Platform-specific classes --------

		Window* m_Window = nullptr;
		LoadedGameAssembly* m_LoadedGameAssembly = nullptr;
		TimeManager* m_TimeManager = nullptr;

		SharedPtr<Graphics::ImGuiHandler> m_ImguiHandler;
		SharedPtr<Graphics::RenderPlatform> m_platform;

		// -------------------------------------------

		SharedPtr<AssetManager> m_AssetManager;
		SharedPtr<SceneManager> m_SceneManager;
		
	};
}