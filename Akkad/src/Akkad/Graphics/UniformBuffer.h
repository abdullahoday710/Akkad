#pragma once
#include "ShaderDataType.h"
#include "Akkad/core.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <map>
#include <vector>
#include <type_traits>

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

				return (*it).second;
			}

			std::vector<std::pair<std::string, UniformBufferElement>>& GetElements() { return m_DataMap; }
			bool UseBindingPointCounter = true;
			
		private:
			std::vector<std::pair<std::string, UniformBufferElement>> m_DataMap;
			unsigned int m_BufferSize = 0;
			friend class GLUniformBuffer;
			friend class GLESUniformBuffer;
		};

		/* -------------- Templates to check if the data type we are passing is supported ------------------- */

		template <typename T>
		struct UniformBufferDataTypeMap
		{
			static const bool isValid = false;
			static const ShaderDataType shaderType = ShaderDataType::UNKNOWN;
		};

		template <>
		struct UniformBufferDataTypeMap<float>
		{
			using BaseType = float;
			static const bool isValid = true;
			static const ShaderDataType shaderType = ShaderDataType::FLOAT;
		};

		template <>
		struct UniformBufferDataTypeMap<unsigned int>
		{
			using BaseType = unsigned int;
			static const bool isValid = true;
			static const ShaderDataType shaderType = ShaderDataType::UNISGNED_INT;
		};

		template <>
		struct UniformBufferDataTypeMap<glm::mat3>
		{
			using BaseType = glm::mat3;
			static const bool isValid = true;
			static const ShaderDataType shaderType = ShaderDataType::MAT3;
		};

		template <>
		struct UniformBufferDataTypeMap<glm::mat4>
		{
			using BaseType = glm::mat4;
			static const bool isValid = true;
			static const ShaderDataType shaderType = ShaderDataType::MAT4;
		};

		template <>
		struct UniformBufferDataTypeMap<glm::vec2>
		{
			using BaseType = glm::vec2;
			static const bool isValid = true;
			static const ShaderDataType shaderType = ShaderDataType::FLOAT2;
		};

		template <>
		struct UniformBufferDataTypeMap<glm::vec3>
		{
			using BaseType = glm::vec3;
			static const bool isValid = true;
			static const ShaderDataType shaderType = ShaderDataType::FLOAT3;
		};

		template <>
		struct UniformBufferDataTypeMap<glm::vec4>
		{
			using BaseType = glm::vec4;
			static const bool isValid = true;
			static const ShaderDataType shaderType = ShaderDataType::FLOAT4;
		};

		/*------------------------------------------------------------------------------*/

		class UniformBuffer {
		public:
			enum RESERVED_BINDING_POINTS
			{
				_POINTS_MIN, MATERIAL_POINT, _POINTS_MAX
			};

			virtual UniformBufferLayout& GetLayout() = 0;

			virtual std::string GetName() { return ""; };
			virtual void SetName(std::string name) {};

			virtual void ResetData() = 0;
			virtual void SetReservedBindingPoint(RESERVED_BINDING_POINTS point) = 0;

			template<typename T>
			void SetData(std::string index, T& data)
			{
				bool valid = UniformBufferDataTypeMap<T>::isValid;
				AK_ASSERT(valid, "Trying to push unsupported data type to the uniform buffer !");

				auto element = m_Layout[index];
				AK_ASSERT(element.GetType() == UniformBufferDataTypeMap<T>::shaderType, "Uniform buffer data type mismatch !");

				auto elementType = element.GetType();
				
				std::vector<char> dataBuffer;
				dataBuffer.resize(GetSizeOfType(element.GetType()));

				memcpy(dataBuffer.data(), &data, GetSizeOfType(element.GetType()));

				unsigned int position = element.offset;
				
				for (auto it : dataBuffer)
				{
					m_BufferData[position] = it;
					position += 1;
				}

				ResetData();
			}

			template<typename T>
			T GetData(std::string index) {
				AK_ASSERT(false, "trying to get an unkown data type");
			}

			template<>
			float GetData(std::string index)
			{
				auto element = m_Layout[index];
				AK_ASSERT(element.GetType() == ShaderDataType::FLOAT, "uniform buffer data type mismatch !");
				auto data = GetDataGeneric(index);
				float result;

				memcpy(&result, (void*)data.data(), GetSizeOfType(element.GetType()));

				return result;
			}

			template<>
			unsigned int GetData(std::string index)
			{
				auto element = m_Layout[index];
				AK_ASSERT(element.GetType() == ShaderDataType::UNISGNED_INT, "uniform buffer data type mismatch !");
				auto data = GetDataGeneric(index);
				unsigned int result;

				memcpy(&result, (void*)data.data(), GetSizeOfType(element.GetType()));

				return result;
			}

			template<>
			glm::vec2 GetData(std::string index)
			{
				auto element = m_Layout[index];
				AK_ASSERT(element.GetType() == ShaderDataType::FLOAT2, "uniform buffer data type mismatch !");
				auto data = GetDataGeneric(index);
				glm::vec2 result;

				memcpy(&result, (void*)data.data(), GetSizeOfType(element.GetType()));

				return result;
			}

			template<>
			glm::vec3 GetData(std::string index)
			{
				auto element = m_Layout[index];
				AK_ASSERT(element.GetType() == ShaderDataType::FLOAT3, "uniform buffer data type mismatch !");
				auto data = GetDataGeneric(index);
				glm::vec3 result;

				memcpy(&result, (void*)data.data(), GetSizeOfType(element.GetType()));

				return result;
			}

			template<>
			glm::vec4 GetData(std::string index)
			{
				auto element = m_Layout[index];
				AK_ASSERT(element.GetType() == ShaderDataType::FLOAT4, "uniform buffer data type mismatch !");
				auto data = GetDataGeneric(index);
				glm::vec4 result;

				memcpy(&result, (void*)data.data(), GetSizeOfType(element.GetType()));

				return result;
			}

		protected:

			std::vector<char> GetDataGeneric(std::string index) {
				auto element = m_Layout[index];
				auto type = element.GetType();
				unsigned int offset = element.offset;

				std::vector<char> dataBuffer;

				for (unsigned int i = 0; i < GetSizeOfType(type); i++)
				{
					unsigned int position = i + offset;
					char byte = m_BufferData[position];
					dataBuffer.push_back(byte);
				}
				return dataBuffer;
			}

			std::vector<char> m_BufferData;
			UniformBufferLayout m_Layout;
		};
	}
}