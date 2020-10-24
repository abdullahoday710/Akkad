#pragma once
#include "Akkad/Input/KeyCodes.h"

unsigned int Win32KeyCodeToAkkadKeyCode(WPARAM code) {
    switch (code)
    {
    case    0x00B: return AK_KEY_0;
    case    0x002: return AK_KEY_1;
    case    0x003: return AK_KEY_2;
    case    0x004: return AK_KEY_3;
    case    0x005: return AK_KEY_4;
    case    0x006: return AK_KEY_5;
    case    0x007: return AK_KEY_6;
    case    0x008: return AK_KEY_7;
    case    0x009: return AK_KEY_8;
    case    0x00A: return AK_KEY_9;
    case    0x01E: return AK_KEY_A;
    case    0x030: return AK_KEY_B;
    case    0x02E: return AK_KEY_C;
    case    0x020: return AK_KEY_D;
    case    0x012: return AK_KEY_E;
    case    0x021: return AK_KEY_F;
    case    0x022: return AK_KEY_G;
    case    0x023: return AK_KEY_H;
    case    0x017: return AK_KEY_I;
    case    0x024: return AK_KEY_J;
    case    0x025: return AK_KEY_K;
    case    0x026: return AK_KEY_L;
    case    0x032: return AK_KEY_M;
    case    0x031: return AK_KEY_N;
    case    0x018: return AK_KEY_O;
    case    0x019: return AK_KEY_P;
    case    0x010: return AK_KEY_Q;
    case    0x013: return AK_KEY_R;
    case    0x01F: return AK_KEY_S;
    case    0x014: return AK_KEY_T;
    case    0x016: return AK_KEY_U;
    case    0x02F: return AK_KEY_V;
    case    0x011: return AK_KEY_W;
    case    0x02D: return AK_KEY_X;
    case    0x015: return AK_KEY_Y;
    case    0x02C: return AK_KEY_Z;

    case    0x028: return AK_KEY_APOSTROPHE;
    case    0x02B: return AK_KEY_BACKSLASH;
    case    0x033: return AK_KEY_COMMA;
    case    0x00D: return AK_KEY_EQUAL;
    case    0x029: return AK_KEY_GRAVE_ACCENT;
    case    0x01A: return AK_KEY_LEFT_BRACKET;
    case    0x00C: return AK_KEY_MINUS;
    case    0x034: return AK_KEY_PERIOD;
    case    0x01B: return AK_KEY_RIGHT_BRACKET;
    case    0x027: return AK_KEY_SEMICOLON;
    case    0x035: return AK_KEY_SLASH;
    case    0x056: return AK_KEY_WORLD_2;

    case    0x00E: return AK_KEY_BACKSPACE;
    case    0x153: return AK_KEY_DELETE;
    case    0x14F: return AK_KEY_END;
    case    0x01C: return AK_KEY_ENTER;
    case    0x001: return AK_KEY_ESCAPE;
    case    0x147: return AK_KEY_HOME;
    case    0x152: return AK_KEY_INSERT;
    case    0x15D: return AK_KEY_MENU;
    case    0x151: return AK_KEY_PAGE_DOWN;
    case    0x149: return AK_KEY_PAGE_UP;
    case    0x045: return AK_KEY_PAUSE;
    case    0x146: return AK_KEY_PAUSE;
    case    0x039: return AK_KEY_SPACE;
    case    0x00F: return AK_KEY_TAB;
    case    0x03A: return AK_KEY_CAPS_LOCK;
    case    0x145: return AK_KEY_NUM_LOCK;
    case    0x046: return AK_KEY_SCROLL_LOCK;
    case    0x03B: return AK_KEY_F1;
    case    0x03C: return AK_KEY_F2;
    case    0x03D: return AK_KEY_F3;
    case    0x03E: return AK_KEY_F4;
    case    0x03F: return AK_KEY_F5;
    case    0x040: return AK_KEY_F6;
    case    0x041: return AK_KEY_F7;
    case    0x042: return AK_KEY_F8;
    case    0x043: return AK_KEY_F9;
    case    0x044: return AK_KEY_F10;
    case    0x057: return AK_KEY_F11;
    case    0x058: return AK_KEY_F12;
    case    0x064: return AK_KEY_F13;
    case    0x065: return AK_KEY_F14;
    case    0x066: return AK_KEY_F15;
    case    0x067: return AK_KEY_F16;
    case    0x068: return AK_KEY_F17;
    case    0x069: return AK_KEY_F18;
    case    0x06A: return AK_KEY_F19;
    case    0x06B: return AK_KEY_F20;
    case    0x06C:  return AK_KEY_F21;
    case    0x06D: return AK_KEY_F22;
    case    0x06E: return AK_KEY_F23;
    case    0x076: return AK_KEY_F24;
    case    0x038: return AK_KEY_LEFT_ALT;
    case    0x01D: return AK_KEY_LEFT_CONTROL;
    case    0x02A: return AK_KEY_LEFT_SHIFT;
    case    0x15B: return AK_KEY_LEFT_SUPER;
    case    0x137: return AK_KEY_PRINT_SCREEN;
    case    0x138: return AK_KEY_RIGHT_ALT;
    case    0x11D: return AK_KEY_RIGHT_CONTROL;
    case    0x036: return AK_KEY_RIGHT_SHIFT;
    case    0x15C: return AK_KEY_RIGHT_SUPER;
    case    0x150: return AK_KEY_DOWN;
    case    0x14B: return AK_KEY_LEFT;
    case    0x14D: return AK_KEY_RIGHT;
    case    0x148: return AK_KEY_UP;

    case    0x052: return AK_KEY_KP_0;
    case    0x04F: return AK_KEY_KP_1;
    case    0x050: return AK_KEY_KP_2;
    case    0x051: return AK_KEY_KP_3;
    case    0x04B: return AK_KEY_KP_4;
    case    0x04C: return AK_KEY_KP_5;
    case    0x04D: return AK_KEY_KP_6;
    case    0x047: return AK_KEY_KP_7;
    case    0x048: return AK_KEY_KP_8;
    case    0x049: return AK_KEY_KP_9;
    case    0x04E: return AK_KEY_KP_ADD;
    case    0x053: return AK_KEY_KP_DECIMAL;
    case    0x135: return AK_KEY_KP_DIVIDE;
    case    0x11C: return AK_KEY_KP_ENTER;
    case    0x059: return AK_KEY_KP_EQUAL;
    case    0x037: return AK_KEY_KP_MULTIPLY;
    case    0x04A: return AK_KEY_KP_SUBTRACT;
    default:
        return AK_KEY_UNKNOWN;
    }
    
}