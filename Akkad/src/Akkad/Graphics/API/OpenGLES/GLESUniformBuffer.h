#pragma once
#include "Akkad/Graphics/UniformBuffer.h"

namespace Akkad {
	namespace Graphics {

		class GLESUniformBuffer : public UniformBuffer
		{
		public:
			GLESUniformBuffer(UniformBufferLayout layout);
			~GLESUniformBuffer();
			virtual UniformBufferLayout& GetLayout() override;

			virtual std::string GetName() override { return m_Name; };
			virtual void SetName(std::string name) override { m_Name = name; };

			virtual void ResetData() override;

			unsigned int GetBindingPoint() { return m_BindingPoint; };

		private:
			void CookLayout(); // cooks the layout according to the std140 specs
			unsigned int GetBaseAlignmentSTD140(ShaderDataType type);
			unsigned int m_ResourceID;
			unsigned int m_BindingPoint;
			static unsigned int s_LastBindingPoint;

			std::string m_Name;
		};
	}
}


