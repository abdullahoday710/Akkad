#include <iostream>
#include <Akkad/Akkad.h>
#include <Akkad/Windows/Win32Window.h>
int main()
{
	Akkad::Application& app = Akkad::Application::GetInstance();

	app.Init();
	app.Run();
}
