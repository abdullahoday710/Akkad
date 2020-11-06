#include "GLShader.h"
#include "glad/glad.h"
#include <iostream>
namespace Akkad {
	namespace Graphics {

		GLShader::GLShader(const char* path)
		{
			ShaderDescriptor desc = LoadFile(path);
			m_ResourceID = glCreateProgram();
			
			for (ShaderProgramType type : desc.ProgramTypes)
			{
				switch (type)
				{
				case ShaderProgramType::VERTEX:
				{
					m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
					const char* src = desc.VertexSource.c_str();
					glShaderSource(m_VertexShader, 1, &src, NULL);
					//TODO : check shader compiler errors
					glCompileShader(m_VertexShader);

					glAttachShader(m_ResourceID, m_VertexShader);
					break;
				}

				case ShaderProgramType::FRAGMENT:
				{
					m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
					const char* src = desc.FragmentSource.c_str();
					glShaderSource(m_FragmentShader, 1, &src, NULL);
					glCompileShader(m_FragmentShader);

					glAttachShader(m_ResourceID, m_FragmentShader);
					break;
				}

				case ShaderProgramType::GEOMETRY:
					break;
				default:
					break;
				}
			}
			//TODO : check shader linking errors
			glLinkProgram(m_ResourceID);

			glDeleteShader(m_VertexShader);
			glDeleteShader(m_FragmentShader);
		}

		GLShader::~GLShader()
		{
			glDeleteProgram(m_ResourceID);
		}

		void GLShader::Bind()
		{
			glUseProgram(m_ResourceID);
		}

		void GLShader::Unbind()
		{
			glUseProgram(0);
		}
	}
}