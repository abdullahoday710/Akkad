#include "WebWindow.h"
#include "Akkad/Application/Application.h"

#include <emscripten/key_codes.h>
namespace Akkad {
	std::function<void(Event&)> WebWindow::m_EventCallbackFN;
	std::map<size_t, uint32_t> WebWindow::m_KeyCodes;
	bool WebWindow::m_KeyStates[512];

	int WebWindow::Init(WindowSettings settings)
	{
		int width = settings.width;
		int height = settings.height;
		emscripten_set_window_title(settings.title);
		emscripten_set_canvas_element_size("#canvas", width, height);
		MakeWebKeyCodes();
		emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, true, WebWindow::EmKeyDownCallback);
		emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, true, WebWindow::EmKeyUpCallback);

		return 0;
	}

	void WebWindow::OnUpdate()
	{
	}

	void WebWindow::SetEventCallback(std::function<void(Event&)> func)
	{
		m_EventCallbackFN = func;
	}

	unsigned int WebWindow::GetWidth()
	{
		int width = 0;
		int height = 0;
		emscripten_get_canvas_element_size("#canvas", &width, &height);
		return width;
	}
	unsigned int WebWindow::GetHeight()
	{
		int width = 0;
		int height = 0;
		emscripten_get_canvas_element_size("#canvas", &width, &height);
		return height;
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
		EmscriptenFullscreenChangeEvent ev;
		emscripten_get_fullscreen_status(&ev);

		return ev.isFullscreen;
	}

	EM_BOOL WebWindow::EmKeyDownCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData)
	{
		uint32_t code = emscripten_compute_dom_pk_code(keyEvent->code);
		uint32_t ak_code = m_KeyCodes[code];
		
		m_KeyStates[ak_code] = true;
		return true;
	}
	EM_BOOL WebWindow::EmKeyUpCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData)
	{
		uint32_t code = emscripten_compute_dom_pk_code(keyEvent->code);
		uint32_t ak_code = m_KeyCodes[code];

		m_KeyStates[ak_code] = false;
		return true;
	}
}