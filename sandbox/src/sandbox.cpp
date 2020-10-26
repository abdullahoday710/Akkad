#include <iostream>
#include <Akkad/Akkad.h>

int main()
{
	Akkad::Application& app = Akkad::Application::GetInstance();

	app.Init();
	app.Run();
}
