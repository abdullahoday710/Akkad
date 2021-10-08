#include "WebWindow.h"

#include "Akkad/Input/KeyCodes.h"
#include <emscripten/key_codes.h>
void Akkad::WebWindow::MakeWebKeyCodes()
{
	m_KeyCodes[DOM_PK_UNKNOWN] = AK_KEY_UNKNOWN;
	m_KeyCodes[DOM_PK_A] = AK_KEY_A; m_KeyCodes[DOM_PK_B] = AK_KEY_B; m_KeyCodes[DOM_PK_C] = AK_KEY_C; m_KeyCodes[DOM_PK_D] = AK_KEY_D;
	m_KeyCodes[DOM_PK_E] = AK_KEY_E; m_KeyCodes[DOM_PK_F] = AK_KEY_F; m_KeyCodes[DOM_PK_G] = AK_KEY_G; m_KeyCodes[DOM_PK_H] = AK_KEY_H;
	m_KeyCodes[DOM_PK_I] = AK_KEY_I; m_KeyCodes[DOM_PK_J] = AK_KEY_J; m_KeyCodes[DOM_PK_K] = AK_KEY_K; m_KeyCodes[DOM_PK_L] = AK_KEY_L;
	m_KeyCodes[DOM_PK_M] = AK_KEY_M; m_KeyCodes[DOM_PK_N] = AK_KEY_N; m_KeyCodes[DOM_PK_O] = AK_KEY_O; m_KeyCodes[DOM_PK_P] = AK_KEY_P;
	m_KeyCodes[DOM_PK_Q] = AK_KEY_Q; m_KeyCodes[DOM_PK_R] = AK_KEY_R; m_KeyCodes[DOM_PK_S] = AK_KEY_S; m_KeyCodes[DOM_PK_T] = AK_KEY_T;
	m_KeyCodes[DOM_PK_U] = AK_KEY_U; m_KeyCodes[DOM_PK_V] = AK_KEY_V; m_KeyCodes[DOM_PK_W] = AK_KEY_W; m_KeyCodes[DOM_PK_X] = AK_KEY_X;
	m_KeyCodes[DOM_PK_Y] = AK_KEY_Y; m_KeyCodes[DOM_PK_Z] = AK_KEY_Z;
	m_KeyCodes[DOM_PK_0] = AK_KEY_0; m_KeyCodes[DOM_PK_1] = AK_KEY_1; m_KeyCodes[DOM_PK_2] = AK_KEY_2;
	m_KeyCodes[DOM_PK_3] = AK_KEY_3; m_KeyCodes[DOM_PK_4] = AK_KEY_4; m_KeyCodes[DOM_PK_5] = AK_KEY_5;
	m_KeyCodes[DOM_PK_6] = AK_KEY_6; m_KeyCodes[DOM_PK_7] = AK_KEY_7; m_KeyCodes[DOM_PK_8] = AK_KEY_8;
	m_KeyCodes[DOM_PK_9] = AK_KEY_9;
	m_KeyCodes[DOM_PK_F1] = AK_KEY_F1; m_KeyCodes[DOM_PK_F2] = AK_KEY_F2; m_KeyCodes[DOM_PK_F3] = AK_KEY_F3; m_KeyCodes[DOM_PK_F4] = AK_KEY_F4;
	m_KeyCodes[DOM_PK_F5] = AK_KEY_F5; m_KeyCodes[DOM_PK_F6] = AK_KEY_F6; m_KeyCodes[DOM_PK_F7] = AK_KEY_F7; m_KeyCodes[DOM_PK_F8] = AK_KEY_F8;
	m_KeyCodes[DOM_PK_F9] = AK_KEY_F9; m_KeyCodes[DOM_PK_F10] = AK_KEY_F10; m_KeyCodes[DOM_PK_F11] = AK_KEY_F11; m_KeyCodes[DOM_PK_F12] = AK_KEY_F12;
	m_KeyCodes[DOM_PK_ARROW_UP] = AK_KEY_UP; m_KeyCodes[DOM_PK_ARROW_DOWN] = AK_KEY_DOWN;
	m_KeyCodes[DOM_PK_ARROW_LEFT] = AK_KEY_LEFT; m_KeyCodes[DOM_PK_ARROW_RIGHT] = AK_KEY_RIGHT;
	m_KeyCodes[DOM_PK_SPACE] = AK_KEY_SPACE; m_KeyCodes[DOM_PK_TAB] = AK_KEY_TAB;
	m_KeyCodes[DOM_PK_SHIFT_LEFT] = AK_KEY_LEFT_SHIFT; m_KeyCodes[DOM_PK_SHIFT_RIGHT] = AK_KEY_RIGHT_SHIFT;
	m_KeyCodes[DOM_PK_CONTROL_LEFT] = AK_KEY_LEFT_CONTROL; m_KeyCodes[DOM_PK_CONTROL_RIGHT] = AK_KEY_RIGHT_CONTROL;
	m_KeyCodes[DOM_PK_INSERT] = AK_KEY_INSERT; m_KeyCodes[DOM_PK_DELETE] = AK_KEY_DELETE; m_KeyCodes[DOM_PK_HOME] = AK_KEY_HOME;
	m_KeyCodes[DOM_PK_END] = AK_KEY_END; m_KeyCodes[DOM_PK_PAGE_UP] = AK_KEY_PAGE_UP; m_KeyCodes[DOM_PK_PAGE_DOWN] = AK_KEY_PAGE_DOWN;
	m_KeyCodes[DOM_PK_BACKSPACE] = AK_KEY_BACKSPACE; m_KeyCodes[DOM_PK_ESCAPE] = AK_KEY_ESCAPE;
	m_KeyCodes[DOM_PK_ENTER] = AK_KEY_ENTER; m_KeyCodes[DOM_PK_NUMPAD_EQUAL] = AK_KEY_EQUAL;
	m_KeyCodes[DOM_PK_NUMPAD_ENTER] = AK_KEY_ENTER; m_KeyCodes[DOM_PK_PAUSE] = AK_KEY_PAUSE;
	m_KeyCodes[DOM_PK_SCROLL_LOCK] = AK_KEY_SCROLL_LOCK;
	m_KeyCodes[DOM_PK_NUMPAD_0] = AK_KEY_KP_0; m_KeyCodes[DOM_PK_NUMPAD_1] = AK_KEY_KP_1; m_KeyCodes[DOM_PK_NUMPAD_2] = AK_KEY_KP_2;
	m_KeyCodes[DOM_PK_NUMPAD_3] = AK_KEY_KP_3; m_KeyCodes[DOM_PK_NUMPAD_4] = AK_KEY_KP_4; m_KeyCodes[DOM_PK_NUMPAD_5] = AK_KEY_KP_5;
	m_KeyCodes[DOM_PK_NUMPAD_6] = AK_KEY_KP_6; m_KeyCodes[DOM_PK_NUMPAD_7] = AK_KEY_KP_7; m_KeyCodes[DOM_PK_NUMPAD_8] = AK_KEY_KP_8;
	m_KeyCodes[DOM_PK_NUMPAD_9] = AK_KEY_KP_9;
	m_KeyCodes[DOM_PK_NUMPAD_MULTIPLY] = AK_KEY_KP_MULTIPLY; m_KeyCodes[DOM_PK_NUMPAD_DIVIDE] = AK_KEY_KP_DIVIDE;
	m_KeyCodes[DOM_PK_NUMPAD_ADD] = AK_KEY_KP_ADD; m_KeyCodes[DOM_PK_NUMPAD_SUBTRACT] = AK_KEY_KP_SUBTRACT;
	m_KeyCodes[DOM_PK_NUMPAD_DECIMAL] = AK_KEY_KP_DECIMAL;
	m_KeyCodes[DOM_PK_PERIOD] = AK_KEY_PERIOD; m_KeyCodes[DOM_PK_EQUAL] = AK_KEY_EQUAL;
	m_KeyCodes[DOM_PK_COMMA] = AK_KEY_COMMA; m_KeyCodes[DOM_PK_MINUS] = AK_KEY_MINUS;
	m_KeyCodes[DOM_PK_CAPS_LOCK] = AK_KEY_CAPS_LOCK;
	m_KeyCodes[DOM_PK_SEMICOLON] = AK_KEY_SEMICOLON;	m_KeyCodes[DOM_PK_SLASH] = AK_KEY_SLASH; m_KeyCodes[DOM_PK_BACKQUOTE] = AK_KEY_GRAVE_ACCENT;
	m_KeyCodes[DOM_PK_BRACKET_LEFT] = AK_KEY_LEFT_BRACKET; m_KeyCodes[DOM_PK_BACKSLASH] = AK_KEY_BACKSLASH; m_KeyCodes[DOM_PK_BRACKET_RIGHT] = AK_KEY_RIGHT_BRACKET;
	m_KeyCodes[DOM_PK_QUOTE] = AK_KEY_APOSTROPHE; m_KeyCodes[DOM_PK_BACKSLASH] = AK_KEY_BACKSLASH;
}
