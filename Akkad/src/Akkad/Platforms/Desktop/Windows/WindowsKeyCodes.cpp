#include "WindowsKeyCodes.h"
int keyCodes[512];
int scanCodes[512];

void MakeWindowskeyCodes()
{
    int scancode;
        keyCodes[0x00B] = AK_KEY_0;
        keyCodes[0x002] = AK_KEY_1;
        keyCodes[0x003] = AK_KEY_2;
        keyCodes[0x004] = AK_KEY_3;
        keyCodes[0x005] = AK_KEY_4;
        keyCodes[0x006] = AK_KEY_5;
        keyCodes[0x007] = AK_KEY_6;
        keyCodes[0x008] = AK_KEY_7;
        keyCodes[0x009] = AK_KEY_8;
        keyCodes[0x00A] = AK_KEY_9;
        keyCodes[0x01E] = AK_KEY_A;
        keyCodes[0x030] = AK_KEY_B;
        keyCodes[0x02E] = AK_KEY_C;
        keyCodes[0x020] = AK_KEY_D;
        keyCodes[0x012] = AK_KEY_E;
        keyCodes[0x021] = AK_KEY_F;
        keyCodes[0x022] = AK_KEY_G;
        keyCodes[0x023] = AK_KEY_H;
        keyCodes[0x017] = AK_KEY_I;
        keyCodes[0x024] = AK_KEY_J;
        keyCodes[0x025] = AK_KEY_K;
        keyCodes[0x026] = AK_KEY_L;
        keyCodes[0x032] = AK_KEY_M;
        keyCodes[0x031] = AK_KEY_N;
        keyCodes[0x018] = AK_KEY_O;
        keyCodes[0x019] = AK_KEY_P;
        keyCodes[0x010] = AK_KEY_Q;
        keyCodes[0x013] = AK_KEY_R;
        keyCodes[0x01F] = AK_KEY_S;
        keyCodes[0x014] = AK_KEY_T;
        keyCodes[0x016] = AK_KEY_U;
        keyCodes[0x02F] = AK_KEY_V;
        keyCodes[0x011] = AK_KEY_W;
        keyCodes[0x02D] = AK_KEY_X;
        keyCodes[0x015] = AK_KEY_Y;
        keyCodes[0x02C] = AK_KEY_Z;

        keyCodes[0x028] = AK_KEY_APOSTROPHE;
        keyCodes[0x02B] = AK_KEY_BACKSLASH;
        keyCodes[0x033] = AK_KEY_COMMA;
        keyCodes[0x00D] = AK_KEY_EQUAL;
        keyCodes[0x029] = AK_KEY_GRAVE_ACCENT;
        keyCodes[0x01A] = AK_KEY_LEFT_BRACKET;
        keyCodes[0x00C] = AK_KEY_MINUS;
        keyCodes[0x034] = AK_KEY_PERIOD;
        keyCodes[0x01B] = AK_KEY_RIGHT_BRACKET;
        keyCodes[0x027] = AK_KEY_SEMICOLON;
        keyCodes[0x035] = AK_KEY_SLASH;
        keyCodes[0x056] = AK_KEY_WORLD_2;

        keyCodes[0x00E] = AK_KEY_BACKSPACE;
        keyCodes[0x153] = AK_KEY_DELETE;
        keyCodes[0x14F] = AK_KEY_END;
        keyCodes[0x01C] = AK_KEY_ENTER;
        keyCodes[0x001] = AK_KEY_ESCAPE;
        keyCodes[0x147] = AK_KEY_HOME;
        keyCodes[0x152] = AK_KEY_INSERT;
        keyCodes[0x15D] = AK_KEY_MENU;
        keyCodes[0x151] = AK_KEY_PAGE_DOWN;
        keyCodes[0x149] = AK_KEY_PAGE_UP;
        keyCodes[0x045] = AK_KEY_PAUSE;
        keyCodes[0x146] = AK_KEY_PAUSE;
        keyCodes[0x039] = AK_KEY_SPACE;
        keyCodes[0x00F] = AK_KEY_TAB;
        keyCodes[0x03A] = AK_KEY_CAPS_LOCK;
        keyCodes[0x145] = AK_KEY_NUM_LOCK;
        keyCodes[0x046] = AK_KEY_SCROLL_LOCK;
        keyCodes[0x03B] = AK_KEY_F1;
        keyCodes[0x03C] = AK_KEY_F2;
        keyCodes[0x03D] = AK_KEY_F3;
        keyCodes[0x03E] = AK_KEY_F4;
        keyCodes[0x03F] = AK_KEY_F5;
        keyCodes[0x040] = AK_KEY_F6;
        keyCodes[0x041] = AK_KEY_F7;
        keyCodes[0x042] = AK_KEY_F8;
        keyCodes[0x043] = AK_KEY_F9;
        keyCodes[0x044] = AK_KEY_F10;
        keyCodes[0x057] = AK_KEY_F11;
        keyCodes[0x058] = AK_KEY_F12;
        keyCodes[0x064] = AK_KEY_F13;
        keyCodes[0x065] = AK_KEY_F14;
        keyCodes[0x066] = AK_KEY_F15;
        keyCodes[0x067] = AK_KEY_F16;
        keyCodes[0x068] = AK_KEY_F17;
        keyCodes[0x069] = AK_KEY_F18;
        keyCodes[0x06A] = AK_KEY_F19;
        keyCodes[0x06B] = AK_KEY_F20;
        keyCodes[0x06C] = AK_KEY_F21;
        keyCodes[0x06D] = AK_KEY_F22;
        keyCodes[0x06E] = AK_KEY_F23;
        keyCodes[0x076] = AK_KEY_F24;
        keyCodes[0x038] = AK_KEY_LEFT_ALT;
        keyCodes[0x01D] = AK_KEY_LEFT_CONTROL;
        keyCodes[0x02A] = AK_KEY_LEFT_SHIFT;
        keyCodes[0x15B] = AK_KEY_LEFT_SUPER;
        keyCodes[0x137] = AK_KEY_PRINT_SCREEN;
        keyCodes[0x138] = AK_KEY_RIGHT_ALT;
        keyCodes[0x11D] = AK_KEY_RIGHT_CONTROL;
        keyCodes[0x036] = AK_KEY_RIGHT_SHIFT;
        keyCodes[0x15C] = AK_KEY_RIGHT_SUPER;
        keyCodes[0x150] = AK_KEY_DOWN;
        keyCodes[0x14B] = AK_KEY_LEFT;
        keyCodes[0x14D] = AK_KEY_RIGHT;
        keyCodes[0x148] = AK_KEY_UP;

        keyCodes[0x052] = AK_KEY_KP_0;
        keyCodes[0x04F] = AK_KEY_KP_1;
        keyCodes[0x050] = AK_KEY_KP_2;
        keyCodes[0x051] = AK_KEY_KP_3;
        keyCodes[0x04B] = AK_KEY_KP_4;
        keyCodes[0x04C] = AK_KEY_KP_5;
        keyCodes[0x04D] = AK_KEY_KP_6;
        keyCodes[0x047] = AK_KEY_KP_7;
        keyCodes[0x048] = AK_KEY_KP_8;
        keyCodes[0x049] = AK_KEY_KP_9;
        keyCodes[0x04E] = AK_KEY_KP_ADD;
        keyCodes[0x053] = AK_KEY_KP_DECIMAL;
        keyCodes[0x135] = AK_KEY_KP_DIVIDE;
        keyCodes[0x11C] = AK_KEY_KP_ENTER;
        keyCodes[0x059] = AK_KEY_KP_EQUAL;
        keyCodes[0x037] = AK_KEY_KP_MULTIPLY;
        keyCodes[0x04A] = AK_KEY_KP_SUBTRACT;

        for (scancode = 0; scancode < 512; scancode++)
        {
            if (keyCodes[scancode] > 0)
                scanCodes[keyCodes[scancode]] = scancode;
        }
}
