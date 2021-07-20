#pragma once
#include "IWindow.h"
namespace Akkad {

	class Layer
	{
	public:
		virtual ~Layer() {};
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void RenderImGui() = 0;
		virtual void OnWindowResize(WindowResizeEvent e) {};

	};

}

