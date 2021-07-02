#include "RuntimeLayer.h"

#include <Akkad/Graphics/SortingLayer2D.h>
#include <fstream>
#include <sstream>
namespace Akkad {

	void RuntimeLayer::InitializeEngine()
	{
		LoadPackageInfo();
		std::string windowTitle = m_packageInfo["name"];
		ApplicationSettings settings;
		settings.window_settings.width = 800;
		settings.window_settings.height = 600;
		settings.window_settings.title = windowTitle.c_str();
		Application::Init(settings);
	}

	void RuntimeLayer::OnAttach()
	{
		RegisterAssets();
		Graphics::Renderer2D::InitShaders();
		Graphics::Renderer2D::SetGUIDebugDrawState(false);
		RegisterSortingLayers();
		LoadGameAssembly();
		LoadStartupScene();
	}

	void RuntimeLayer::OnDetach()
	{
	}

	void RuntimeLayer::OnUpdate()
	{
		auto sceneManager = Application::GetSceneManager();
		auto window = Application::GetInstance().GetWindow();

		sceneManager->GetActiveScene()->BeginRenderer2D((float)window->GetWidth() / (float)window->GetHeight());
		sceneManager->GetActiveScene()->Render2D();
		sceneManager->GetActiveScene()->RenderGUI();
		sceneManager->GetActiveScene()->RenderPickingBuffer2D();

		sceneManager->GetActiveScene()->Update();

	}

	void RuntimeLayer::RenderImGui()
	{
	}

	void RuntimeLayer::OnWindowResize(WindowResizeEvent e)
	{
		auto sceneManager = Application::GetSceneManager();
		auto window = Application::GetInstance().GetWindow();

		Graphics::Rect windowRect;
		windowRect.SetBounds(window->GetWindowRectMin(), window->GetWindowRectMax());
		sceneManager->GetActiveScene()->SetViewportRect(windowRect);
		sceneManager->GetActiveScene()->SetViewportSize({ e.m_Width, e.m_Height });
	}

	void RuntimeLayer::LoadPackageInfo()
	{
		std::ifstream pkginfo;
		const char* fname = "package_info.akpkg";
		pkginfo.open(fname, std::ios::binary);

		std::vector<unsigned char> package_info_data = std::vector<unsigned char>(std::istreambuf_iterator<char>(pkginfo), {});

		m_packageInfo = nlohmann::json::from_cbor(package_info_data);
	}
	void RuntimeLayer::RegisterAssets()
	{
		auto assetManager = Application::GetAssetManager();
		assetManager->SetAssetsRootPath("assets/");

		for (auto it : m_packageInfo["Assets"].items())
		{
			auto asset = it.value();
			AssetDescriptor desc;
			desc.assetID = it.key();
			desc.assetName = asset["name"];
			desc.absolutePath = asset["path"];
			desc.SetAssetType(asset["type"]);
			if (desc.assetType == AssetType::SHADER)
			{
				desc.absolutePath = asset["shaderdescPath"];
			}

			if (desc.assetType == AssetType::TEXTURE)
			{
				SharedPtr<TextureAssetInfo> textureInfo = CreateSharedPtr<TextureAssetInfo>();
				textureInfo->isTilemap = false;
				textureInfo->tileHeight = 0;
				textureInfo->tileWidth = 0;

				if (!asset["IsAtlas"].is_null())
				{
					bool ismap = asset["IsAtlas"];
					textureInfo->isTilemap = ismap;
				}
				desc.assetInfo = textureInfo;
			}

			assetManager->RegisterAsset(it.key(), desc);
		}
	}
	void RuntimeLayer::LoadGameAssembly()
	{
		bool loaded = Application::GetGameAssembly()->LoadAssembly("GameAssembly");
		AK_ASSERT(loaded, "Game startup failed, Because the game assembly could not be loaded !");
		Application::GetGameAssembly()->Initialize(Application::GetInstance().m_ApplicationComponents);
	}

	void RuntimeLayer::RegisterSortingLayers()
	{
		for (auto layer : m_packageInfo["SortingLayers2D"].items())
		{
			SortingLayer2DHandler::RegisterLayer(layer.value());
		}
	}
	void RuntimeLayer::LoadStartupScene()
	{
		std::string startupsceneName = m_packageInfo["startupScene"];
		std::string scenePath = "assets/scenes/" + startupsceneName;
		Application::GetSceneManager()->LoadScene(scenePath);
		Application::GetSceneManager()->GetActiveScene()->Start();
	}
}