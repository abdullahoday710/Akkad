#pragma once
#include <Windows.h>
#include "wglext.h"

namespace Akkad {

	struct GLContextData {
		HGLRC rc;
		HDC dc;
	};

	bool WGLExtensionSupported(const char* extension_name);

	GLContextData CreateContextGL(HWND hwnd);
}