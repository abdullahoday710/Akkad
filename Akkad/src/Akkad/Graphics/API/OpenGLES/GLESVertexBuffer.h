#pragma once
#include "Akkad/Graphics/Buffer.h"

namespace Akkad {
	namespace Graphics {
		class GLESVertexBuffer : public VertexBuffer
		{
		public:
			GLESVertexBuffer();
			~GLESVertexBuffer();
			virtual void Bind() override;
			virtual void UnBind() override;
			virtual void SetData(const void* data, unsigned int size) override;
			virtual void SetSubData(unsigned int offset, const void* data, unsigned int size) override;
			virtual void SetLayout(VertexBufferLayout layout) override;
			virtual void ExtendLayout(SharedPtr<VertexBuffer> vb) override;
			virtual VertexBufferLayout& GetLayout() override;
		private:
			VertexBufferLayout m_Layout;
			unsigned int m_VA;
			unsigned int m_ResourceID;
			unsigned int m_AvailableVertexAttribute = 0;
		};
	}
}

