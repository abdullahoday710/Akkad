#include <iostream>
#include <Akkad/Akkad.h>
#include <Akkad/Graphics/Shader.h>
using namespace Akkad;
using namespace Graphics;
int main()
{
	Akkad::Application& app = Akkad::Application::GetInstance();
	auto desc = Shader::LoadFile("res/shaders/test.txt");

	app.Init();
	app.Run();
}
