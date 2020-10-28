#include "ImGuiHandler.h"
#include "API/OpenGL/GLImGuiHandler.h"
namespace Akkad {
    namespace Graphics {

        ImGuiHandler* ImGuiHandler::create(RenderAPI api)
        {
            switch (api)
            {
            case RenderAPI::OPENGL:
                return new GLImGuiHandler();
            }
        }
    }
}

