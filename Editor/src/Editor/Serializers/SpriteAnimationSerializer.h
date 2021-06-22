#pragma once
#include <Akkad/core.h>

#include <string>
namespace Akkad {
	namespace Graphics {
		class SpriteAnimation;
	}
	class SpriteAnimationSerializer
	{
	public:
		static void Serialize(SharedPtr<Graphics::SpriteAnimation> animation, std::string animationName, std::string outputPath);
	};
}
