#pragma once
#include "Akkad/core.h"

namespace Akkad {
	namespace Graphics {

		enum class PrimitiveType {
			TRIANGLE, TRIANGLE_FAN, POINTS, LINE
		};

		enum class PolygonMode {
			POINT, LINE, FILL
		};

		class RenderCommand
		{
		public:
			virtual void Clear() = 0;
			virtual void SetClearColor(float r, float g, float b) = 0;
			virtual void DrawArrays(PrimitiveType type, unsigned int count) = 0;
			virtual void DrawIndexed(PrimitiveType type, unsigned int count) = 0;
			virtual void SetPolygonMode(PolygonMode mode) = 0;
		};
	}
}


