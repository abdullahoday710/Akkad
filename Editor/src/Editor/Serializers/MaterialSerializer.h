#pragma once
#include <Akkad/core.h>

#include <string>
namespace Akkad {
	namespace Graphics {
		class Material;
	}
	class MaterialSerializer
	{
	public:
		static void Serialize(SharedPtr<Graphics::Material> material, std::string outputPath);

	};
}


