#include <iostream>
#include <Akkad/Akkad.h>

using namespace Akkad;
using namespace Graphics;
int main()
{
	Akkad::Application& app = Akkad::Application::GetInstance();

	app.Init();
	app.Run();
}
