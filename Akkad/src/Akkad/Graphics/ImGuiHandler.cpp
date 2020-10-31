#include "ImGuiHandler.h"
#include "API/OpenGL/GLImGuiHandler.h"
namespace Akkad {
    namespace Graphics {

        SharedPtr<ImGuiHandler> ImGuiHandler::create(RenderAPI api)
        {
            switch (api)
            {
            case RenderAPI::OPENGL:
                return CreateSharedPtr<GLImGuiHandler>();
            }
        }
    }
}

