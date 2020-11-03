#pragma once
#include "Akkad/Graphics/Buffer.h"

namespace Akkad {
	namespace Graphics {

		class GLVertexBuffer : public VertexBuffer
		{
		public:
			GLVertexBuffer();
			~GLVertexBuffer();
			virtual void Bind() override;
			virtual void UnBind() override;
			virtual void SetData(const void* data, unsigned int size) override;
			virtual void SetLayout(BufferLayout layout) override;
			virtual BufferLayout& GetLayout() override;
		private:
			BufferLayout m_Layout;
			unsigned int m_VA;
			unsigned int m_ResourceID;
		};
	}
}


