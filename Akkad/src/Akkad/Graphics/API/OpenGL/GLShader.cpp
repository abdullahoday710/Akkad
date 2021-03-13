#include "GLShader.h"
#include "GLUniformBuffer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <spirv_glsl.hpp>

namespace Akkad {
	namespace Graphics {

		GLShader::GLShader(const char* path)
		{
			
			ShaderDescriptor desc = LoadShader(path);
			m_ResourceID = glCreateProgram();
			
			for (ShaderProgramType type : desc.ProgramTypes)
			{
				switch (type)
				{
				case ShaderProgramType::VERTEX:
				{
					m_VertexShader = glCreateShader(GL_VERTEX_SHADER);

					std::string shaderString = CompileFromSPV(desc.VertexData);
					const char* src = shaderString.c_str();

					glShaderSource(m_VertexShader, 1, &src, NULL);

					glCompileShader(m_VertexShader);

					glAttachShader(m_ResourceID, m_VertexShader);
					break;
				}

				case ShaderProgramType::FRAGMENT:
				{
					m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

					std::string shaderString = CompileFromSPV(desc.FragmentData);
					const char* src = shaderString.c_str();

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
		void GLShader::SetMat4(const char* location, glm::mat4& value)
		{
			glUniformMatrix4fv(glGetUniformLocation(m_ResourceID, location), 1, GL_FALSE, glm::value_ptr(value));
		}
		void GLShader::SetVec3(const char* location, glm::vec3& value)
		{
			glUniform3f(glGetUniformLocation(m_ResourceID, location), value.x, value.y, value.z);
		}

		void GLShader::SetUniformBuffer(SharedPtr<UniformBuffer> buffer)
		{
			SharedPtr<GLUniformBuffer> glBuffer = DynamicCastPtr<GLUniformBuffer>(buffer);
			unsigned int bufferIndex = glGetUniformBlockIndex(m_ResourceID, glBuffer->GetName().c_str());
			glUniformBlockBinding(m_ResourceID, bufferIndex, glBuffer->GetBindingPoint());
		}

		std::string GLShader::CompileFromSPV(std::vector<unsigned int>& spv)
		{

			spirv_cross::CompilerGLSL glsl(std::move(spv));

			spirv_cross::CompilerGLSL::Options options;
			options.version = 400;
			glsl.set_common_options(options);

			spirv_cross::ShaderResources resources = glsl.get_shader_resources();

			// remove bindings from uniform buffers:
			for (auto& resource : resources.uniform_buffers)
			{
				unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
				unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);

				glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

				glsl.unset_decoration(resource.id, spv::DecorationBinding);
			}

			// set bindings for textures:
			unsigned int index = 0;
			for (auto& resource : resources.sampled_images)
			{
				unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
				unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);

				glsl.set_decoration(resource.id, spv::DecorationBinding, index);
				index++;
			}
			std::string source = glsl.compile();

			return source;
		}
	}
}