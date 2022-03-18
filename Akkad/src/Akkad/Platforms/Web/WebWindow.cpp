#include "WebWindow.h"
#include "Akkad/Application/Application.h"

#include <emscripten/key_codes.h>
namespace Akkad {
	std::function<void(Event&)> WebWindow::m_EventCallbackFN;
	std::map<size_t, uint32_t> WebWindow::m_KeyCodes;
	bool WebWindow::m_KeyStates[512];
	int WebWindow::m_MouseStatesFrame[3];
	int WebWindow::m_KeyStatesFrame[512];

	bool WebWindow::MouseState_Left = false;
	bool WebWindow::MouseState_Right = false;
	bool WebWindow::MouseState_Middle = false;
	int WebWindow::MouseX = 0;
	int WebWindow::MouseY = 0;

	int WebWindow::Init(WindowSettings settings)
	{
		int width = settings.width;
		int height = settings.height;
		emscripten_set_window_title(settings.title);
		emscripten_set_canvas_element_size("#canvas", width, height);
		MakeWebKeyCodes();
		emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, true, WebWindow::EmKeyDownCallback);
		emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, true, WebWindow::EmKeyUpCallback);
		emscripten_set_mousedown_callback("#canvas", 0, true, WebWindow::EmMouseCallback);
		emscripten_set_mouseup_callback("#canvas", 0, true, WebWindow::EmMouseCallback);
		emscripten_set_mousemove_callback("#canvas", 0, true, WebWindow::EmMouseCallback);

		m_Width = width;
		m_Height = height;
		return 0;
	}

	void WebWindow::OnUpdate()
	{
		#ifndef AK_GAME_ASSEMBLY
			ResetKeyStates();
			int width;
			int height;
			emscripten_get_canvas_element_size("#canvas", &width, &height);
			m_Width = width;
			m_Height = height;
		#endif
	}

	void WebWindow::SetEventCallback(std::function<void(Event&)> func)
	{
		m_EventCallbackFN = func;
	}

	unsigned int WebWindow::GetWidth()
	{
		return m_Width;
	}
	unsigned int WebWindow::GetHeight()
	{
		return m_Height;
	}
	glm::vec2 WebWindow::GetWindowRectMin()
	{
		return {0,0};
	}
	glm::vec2 WebWindow::GetWindowRectMax()
	{
		return {GetWidth(), GetHeight()};
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

	void WebWindow::ResetKeyStates()
	{
		for (size_t i = 0; i < 3; i++)
		{
			m_MouseStatesFrame[i] = -1;
		}

		for (size_t i = 0; i < 512; i++)
		{
			m_KeyStatesFrame[i] = -1;
		}
	}

	EM_BOOL WebWindow::EmKeyDownCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData)
	{
		uint32_t code = emscripten_compute_dom_pk_code(keyEvent->code);
		uint32_t ak_code = m_KeyCodes[code];
		
		m_KeyStates[ak_code] = true;
		if (!keyEvent->repeat)
		{
			m_KeyStatesFrame[ak_code] = true;
		}
		return true;
	}

	EM_BOOL WebWindow::EmKeyUpCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData)
	{
		uint32_t code = emscripten_compute_dom_pk_code(keyEvent->code);
		uint32_t ak_code = m_KeyCodes[code];

		m_KeyStates[ak_code] = false;
		m_KeyStatesFrame[ak_code] = false;
		return true;
	}

	EM_BOOL WebWindow::EmMouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
	{
		// mouse position
		if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE)
		{
			MouseX = mouseEvent->targetX;
			MouseY = mouseEvent->targetY;
		}
		// left click
		if (mouseEvent->button == 0)
		{
			if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
			{
				m_MouseStatesFrame[0] = true;
				MouseState_Left = true;
			}
			else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
			{
				m_MouseStatesFrame[0] = false;
				MouseState_Left = false;
			}
		}

		// right click
		if (mouseEvent->button == 2)
		{
			if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
			{
				m_MouseStatesFrame[1] = true;
				MouseState_Right = true;
			}
			else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
			{
				m_MouseStatesFrame[1] = false;
				MouseState_Right = false;
			}
		}

		// middle click
		if (mouseEvent->button == 1)
		{
			if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
			{
				m_MouseStatesFrame[2] = true;
				MouseState_Middle = true;
			}
			else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
			{
				m_MouseStatesFrame[2] = false;
				MouseState_Middle = false;
			}
		}

		return EM_TRUE;
	}
}