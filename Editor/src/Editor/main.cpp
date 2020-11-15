#include <Akkad/Akkad.h>

#include "EditorLayer.h"

using namespace Akkad;

int main() {

		EditorLayer* editorlayer = new EditorLayer();

		Application::AttachLayer(editorlayer);
		Application::Init();
		Application::Run();
}

