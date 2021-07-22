#pragma once
#include "Akkad/Graphics/Buffer.h"

namespace Akkad {
	namespace Graphics {

		class GLESIndexBuffer : public IndexBuffer
		{
		public:
			GLESIndexBuffer();
			~GLESIndexBuffer();
			virtual void Bind() override;
			virtual void Unbind() override;
			virtual void SetData(const void* data, unsigned int size) override;
		private:
			unsigned int m_ResourceID;
		};
	}
}


