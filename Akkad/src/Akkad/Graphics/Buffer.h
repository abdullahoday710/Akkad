#pragma once
#include "ShaderDataType.h"

#include "Akkad/core.h"

#include <algorithm>
#include <vector>
#include <string>
#include <map>

namespace Akkad {
	namespace Graphics {
		class VertexBuffer;
		struct BufferElement {
			ShaderDataType type;
			unsigned int count;
			bool normalized;
			bool isInstanced = false;
			unsigned int offset = 0;
		};
		
		class VertexBufferLayout {
		public:
			bool isDynamic = false;
			// if set to true, vertex buffer will not create it's own vertex array.
			bool isStaticBuffer = false;
			unsigned int GetStride() { return m_Stride; }

			void Push(ShaderDataType type, unsigned int count, bool isInstanced = false, bool normalized=false) {
				m_Elements.push_back({ type, count, normalized, isInstanced });
				m_Stride += count * GetSizeOfType(type);
			}

			std::vector<BufferElement>& GetElements() { return m_Elements; }
		private:
			unsigned int m_Stride = 0;
			std::vector<BufferElement> m_Elements;
			friend class GLVertexBuffer;

		};
		
		class VertexBuffer {
		public:
			virtual void Bind() = 0;
			virtual void UnBind() = 0;
			virtual void SetData(const void* data, unsigned int size) = 0;
			virtual void SetSubData(unsigned int offset, const void* data, unsigned int size) = 0;
			virtual void SetLayout(VertexBufferLayout layout) = 0;
			virtual void ExtendLayout(SharedPtr<VertexBuffer> vb) = 0;
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

