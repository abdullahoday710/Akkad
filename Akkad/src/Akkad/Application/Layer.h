#pragma once
namespace Akkad {

	class Layer
	{
	public:
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual void OnInit() = 0;
		virtual void OnUpdate() = 0;
		
		virtual void RenderImGui() = 0;

	};

}

