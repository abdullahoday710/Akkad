#pragma once

namespace Akkad {
	namespace Graphics {

		enum class ShaderDataType {
			UNKNOWN, BOOL, FLOAT, FLOAT2, FLOAT3, FLOAT4, UNISGNED_INT, MAT3, MAT4
		};
		unsigned int GetSizeOfType(ShaderDataType type);
	}
}