#include "RuntimeLayer.h"

using namespace Akkad;
using namespace Graphics;

int main()
{
	RuntimeLayer* layer = new RuntimeLayer();
	Application::AttachLayer(layer);
	layer->InitializeEngine();
	Application::Run();
}
