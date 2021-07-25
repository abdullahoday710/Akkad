#include "GLESUniformBuffer.h"

#include <GLES3/gl3.h>
#include <iostream>
#include <cmath>

namespace Akkad {
	namespace Graphics {

		unsigned int GLESUniformBuffer::s_LastBindingPoint = 0;

		GLESUniformBuffer::GLESUniformBuffer(UniformBufferLayout layout)
		{
			m_Layout = layout;
			glGenBuffers(1, &m_ResourceID);

			CookLayout();

			glBindBuffer(GL_UNIFORM_BUFFER, m_ResourceID);
			glBufferData(GL_UNIFORM_BUFFER, m_Layout.m_BufferSize, NULL, GL_STATIC_DRAW); //allocate memory for the buffer on the GPU side
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			for (int i = 0; i < m_Layout.m_BufferSize; i++)
			{
				m_BufferData.push_back('\0'); // pushing "empty" bytes into the buffer
			}

			glBindBufferRange(GL_UNIFORM_BUFFER, s_LastBindingPoint, m_ResourceID, 0 , m_Layout.m_BufferSize);
			m_BindingPoint = s_LastBindingPoint;
			s_LastBindingPoint += 1;
		}

		GLESUniformBuffer::~GLESUniformBuffer()
		{
			glDeleteBuffers(1, &m_ResourceID);
		}

		UniformBufferLayout& GLESUniformBuffer::GetLayout()
		{
			return m_Layout;
		}

		void GLESUniformBuffer::ResetData()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ResourceID);
			
			void* data = glMapBufferRange(GL_UNIFORM_BUFFER,0, m_Layout.m_BufferSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
			memcpy(data, m_BufferData.data(), m_Layout.m_BufferSize);
			glUnmapBuffer(GL_UNIFORM_BUFFER);

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		int roundUp(int numToRound, int multiple)
		{
			return ((numToRound + multiple - 1) / multiple) * multiple;
		}

		void GLESUniformBuffer::CookLayout()
		{
			unsigned int lastOffset = 0;
			unsigned int lastSize = 0;
			unsigned int index = 0;

			for (auto& it : m_Layout.m_DataMap)
			{
				auto& element = it.second;

				if (index == 0)
				{
					element.offset = 0;
					element.memoryAlignment = GetBaseAlignmentSTD140(element.GetType());
					lastSize = GetSizeOfType(element.GetType());
				}

				else
				{
					element.offset = roundUp(lastOffset + lastSize, GetBaseAlignmentSTD140(element.GetType()));
					element.memoryAlignment = GetBaseAlignmentSTD140(element.GetType());
					lastOffset = element.offset;
					lastSize = GetSizeOfType(element.GetType());
				}

				index += 1;
			}

			unsigned int bufferSize = lastOffset + lastSize;

			m_Layout.m_BufferSize = bufferSize;

		}

		unsigned int GLESUniformBuffer::GetBaseAlignmentSTD140(ShaderDataType type)
		{
			unsigned int N = 4;
			switch (type)
			{
			case ShaderDataType::FLOAT:
			case ShaderDataType::UNISGNED_INT:
			case ShaderDataType::BOOL:
			{
				return N;
				break;
			}

			case ShaderDataType::FLOAT2:
			{
				return 2 * N;
				break;
			}

			case ShaderDataType::FLOAT3:
			case ShaderDataType::FLOAT4:
			{
				return 4 * N;
				break;
			}

			case ShaderDataType::MAT3:
			{
				return 4 * N;
				break;
			}

			case ShaderDataType::MAT4:
			{
				return 4 * N;
				break;
			}

			}
		}

	}
}

