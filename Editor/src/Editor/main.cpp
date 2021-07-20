#include <Akkad/Akkad.h>

#include "EditorLayer.h"

using namespace Akkad;

int main() {
		ApplicationSettings settings;
		settings.window_settings.title = "Akkad Editor";
		settings.window_settings.width = 800;
		settings.window_settings.height = 600;
		settings.enable_ImGui = true;

		EditorLayer* editorlayer = new EditorLayer();

		Application::AttachLayer(editorlayer);
		Application::Init(settings);
		Application::Run();
}

