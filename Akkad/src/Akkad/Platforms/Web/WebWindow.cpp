#include "WebWindow.h"

#include "Akkad/Application/Application.h"

#include <emscripten.h>
namespace Akkad {

	int WebWindow::Init(WindowSettings settings)
	{
		emscripten_set_window_title(settings.title);

		return 0;
	}

	void WebWindow::OnUpdate()
	{
	}
	void WebWindow::SetEventCallback(std::function<void(Event&)> func)
	{
	}
	unsigned int WebWindow::GetWidth()
	{
		return 0;
	}
	unsigned int WebWindow::GetHeight()
	{
		return 0;
	}
	glm::vec2 WebWindow::GetWindowRectMin()
	{
		return glm::vec2();
	}
	glm::vec2 WebWindow::GetWindowRectMax()
	{
		return glm::vec2();
	}
	void* WebWindow::GetNativeWindow()
	{
		return nullptr;
	}
	void WebWindow::ToggleFullScreen()
	{
	}
	bool WebWindow::IsFullScreen()
	{
		return false;
	}
}