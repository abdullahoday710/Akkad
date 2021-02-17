#pragma once
#include "ShaderDataType.h"
#include "Akkad/core.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <map>
#include <vector>

namespace Akkad {
	namespace Graphics {

		class UniformBufferElement
		{
		public:
			ShaderDataType GetType() { return m_Type; }
			void SetType(ShaderDataType type)
			{
				AK_ASSERT(m_Type == ShaderDataType::UNKNOWN, "re-setting the type is not allowed !");
				m_Type = type;
			}

			unsigned int offset = -1;
			unsigned int memoryAlignment = -1;

		private:
			ShaderDataType m_Type = ShaderDataType::UNKNOWN;
		};

		class UniformBufferLayout {
		public:
			void Push(std::string name, ShaderDataType type)
			{
				UniformBufferElement element;
				element.SetType(type);

				std::pair<std::string, UniformBufferElement> pair;
				pair.first = name;
				pair.second = element;

				m_DataMap.push_back(pair);
			}

			UniformBufferElement& operator[](std::string index)
			{

				auto it = std::find_if(m_DataMap.begin(), m_DataMap.end(),
					[&index](const std::pair<std::string, UniformBufferElement>& element) { return element.first == index; });
				
				AK_ASSERT(it != m_DataMap.end(), "ELEMENT DOES NOT EXIST !");

				auto result = *it;
				return result.second;
			}

		private:
			std::vector<std::pair<std::string, UniformBufferElement>> m_DataMap;
			unsigned int m_BufferSize;
			friend class GLUniformBuffer;
		};


		class UniformBuffer {
		public:
			virtual UniformBufferLayout& GetLayout() = 0;

			virtual std::string GetName() { return ""; };
			virtual void SetName(std::string name) {};

			virtual void ResetData() = 0;

			void PushDataTest(std::string index, float data)
			{
				auto elem = m_Layout[index];

				std::vector<char> dataBuffer;
				dataBuffer.resize(sizeof(float));
				memcpy(dataBuffer.data(), &data, sizeof(data));

				unsigned int position = elem.offset;
				for (auto it : dataBuffer)
				{
					m_BufferData[position] = it;
					position += 1;
				}
			}

			std::vector<char> m_BufferData;
		protected:
			UniformBufferLayout m_Layout;
		};
	}
}