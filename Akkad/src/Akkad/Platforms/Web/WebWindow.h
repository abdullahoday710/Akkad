#pragma once
#include "Akkad/Application/IWindow.h"

#include "Akkad/PlatformMacros.h"
#ifdef AK_PLATFORM_WEB
#include <emscripten.h>
#include <emscripten/html5.h>
#endif // AK_PLATFORM_WEB

#include <map>

namespace Akkad {

	class WebWindow : public Window
	{
	public:
		virtual int Init(WindowSettings settings);
		virtual void OnUpdate();
		virtual void SetEventCallback(std::function<void(Event&)> func);
		virtual unsigned int GetWidth();
		virtual unsigned int GetHeight();
		virtual glm::vec2 GetWindowRectMin();
		virtual glm::vec2 GetWindowRectMax();
		virtual void* GetNativeWindow();
		virtual void ToggleFullScreen();
		virtual bool IsFullScreen();
	private:

		void MakeWebKeyCodes();
		void ResetKeyStates();

		static std::function<void(Event&)> m_EventCallbackFN;

		static EM_BOOL EmKeyDownCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData);
		static EM_BOOL EmKeyPressCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData);
		static EM_BOOL EmKeyUpCallback(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* userData);
		static EM_BOOL EmWindowResizeCallback(int eventType, const EmscriptenUiEvent* uiEvent, void* userData);
		static std::map<size_t, uint32_t> m_KeyCodes;
		static bool m_KeyStates[512];
		static int m_KeyStatesFrame[512];

		static EM_BOOL EmMouseCallback(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData);
		static bool MouseState_Left;
		static bool MouseState_Right;
		static bool MouseState_Middle;

		static int m_MouseStatesFrame[3];
		static int m_LastPressedChar;

		static int MouseX;
		static int MouseY;

		static int m_Width;
		static int m_Height;

		friend class WebInput;
	};
}


