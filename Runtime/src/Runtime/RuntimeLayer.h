#pragma once
#include <Akkad/Akkad.h>

#include <json.hpp>
namespace Akkad {

	class RuntimeLayer : public Layer
	{
	public:
		void InitializeEngine();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void RenderImGui() override;
		virtual void OnWindowResize(WindowResizeEvent e) override;
	private:
		/*Initialization routines*/
		void LoadPackageInfo();
		void RegisterAssets();
		void LoadGameAssembly();
		void RegisterSortingLayers();
		void LoadStartupScene();
		/*-------------------------*/
		nlohmann::json m_packageInfo;
	};
}


