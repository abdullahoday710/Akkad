#pragma once
#include "Akkad/Graphics/UniformBuffer.h"

namespace Akkad {
	namespace Graphics {

		class GLUniformBuffer : public UniformBuffer
		{
		public:
			GLUniformBuffer(UniformBufferLayout layout);
			~GLUniformBuffer();
			virtual UniformBufferLayout& GetLayout() override;

			virtual std::string GetName() override { return m_Name; };
			virtual void SetName(std::string name) override { m_Name = name; };

			virtual void ResetData() override;
			virtual void SetReservedBindingPoint(RESERVED_BINDING_POINTS point) override;

			unsigned int GetBindingPoint() { return m_BindingPoint; };

		private:
			void SetBindingPoint();
			void CookLayout(); // cooks the layout according to the std140 specs
			unsigned int GetBaseAlignmentSTD140(ShaderDataType type);
			unsigned int m_ResourceID;
			unsigned int m_BindingPoint;
			static unsigned int s_LastBindingPoint;

			std::string m_Name;
		};
	}
}


