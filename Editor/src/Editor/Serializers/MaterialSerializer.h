#pragma once

#include <string>
namespace Akkad {
	namespace Graphics {
		class Material;
	}
	class MaterialSerializer
	{
	public:
		static void Serialize(Graphics::Material& material, std::string outputPath);

	};
}


