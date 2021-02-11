#pragma once
#include "Akkad/Graphics/Buffer.h"

namespace Akkad {
	namespace Graphics {

		class GLIndexBuffer : public IndexBuffer
		{
		public:
			GLIndexBuffer();
			~GLIndexBuffer();
			virtual void Bind() override;
			virtual void Unbind() override;
			virtual void SetData(const void* data, unsigned int size) override;
		private:
			unsigned int m_ResourceID;
		};
	}
}


