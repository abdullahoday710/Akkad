#pragma once
#include <algorithm>
#include <vector>

namespace Akkad {
	namespace Graphics {

		enum class ShaderDataType {
			FLOAT, FLOAT2, FLOAT3, FLOAT4, UNISGNED_INT,
		};

		struct BufferElement {
			ShaderDataType type;
			unsigned int count;
			bool normalized;

			static unsigned int GetSizeOfType(ShaderDataType type);
		};
		
		class VertexBufferLayout {
		public:
			unsigned int GetStride() { return m_Stride; }

			void Push(ShaderDataType type, unsigned int count, bool normalized=false) {
				m_Elements.push_back({ type, count, normalized });
				m_Stride += count * BufferElement::GetSizeOfType(type);
			}

			std::vector<BufferElement> GetElements() { return m_Elements; }
		private:
			unsigned int m_Stride = 0;
			std::vector<BufferElement> m_Elements;

		};
		
		class VertexBuffer {
		public:
			virtual void Bind() = 0;
			virtual void UnBind() = 0;
			virtual void SetData(const void* data, unsigned int size) = 0;
			virtual void SetLayout(VertexBufferLayout layout) = 0;
			virtual VertexBufferLayout& GetLayout() = 0;
		};

		class IndexBuffer {
		public:
			virtual void Bind() = 0;
			virtual void Unbind() = 0;
			virtual void SetData(const void* data, unsigned int size) = 0;
		};
	}
}

