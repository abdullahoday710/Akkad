#include "Material.h"
#include "Renderer2D.h"
#include "RenderPlatform.h"

#include "Akkad/Logging.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

#include <spirv_cross.hpp>
#include <json.hpp>
#include <fstream>

namespace Akkad {

	namespace Graphics {
		std::string Material::DEFAULT_PROPERTY_BUFFER_NAME = "shader_props";

		void Material::SetShader(std::string assetID)
		{
			m_ShaderID = assetID;

			auto assetManager = Application::GetAssetManager();
			m_Shader = assetManager->GetShader(assetID);

			m_Shader->SetUniformBuffer(Renderer2D::GetSystemUniforms());

		}

		void Material::BindTextures()
		{
			for (auto it : m_Textures)
			{
				if (!it.second.assetID.empty())
				{
					auto texture = Application::GetAssetManager()->GetTexture(it.second.assetID);
					texture->Bind(it.second.textureBindingUnit);
				}
			}
		}

		void Material::BindShaders()
		{
			if (m_Shader != nullptr)
			{
				if (m_PropertyBuffer != nullptr)
				{
					m_Shader->SetUniformBuffer(m_PropertyBuffer);
				}
				m_Shader->Bind();
			}
		}

		void Material::ClearResources()
		{
			m_Textures.clear();
			m_PropertyBuffer.reset();
		}

		bool Material::isValid()
		{
			if (m_Shader == nullptr)
			{
				return false;
			}

			else
			{
				return true;
			}
		}

		Material Material::LoadFile(std::string filePath)
		{
				std::ifstream file;
				file.open(filePath);
				// Material deserialization should be ordered so it can match the underlying property buffer.
				nlohmann::ordered_json data;

				file >> data;

				Material material;

				std::string name = data["material"]["name"];
				material.m_Name = name;

				if (!data["material"]["shaderID"].is_null())
				{
					std::string shaderID = data["material"]["shaderID"];
					material.SetShader(shaderID);
					material.SerializeShader();
				}

				if (!data["material"]["textures"].is_null())
				{
					for (auto texture : data["material"]["textures"].items())
					{
						std::string assetID = texture.key();
						std::string samplerName = data["material"]["textures"][assetID]["samplerName"];
						unsigned int bindingUnit = data["material"]["textures"][assetID]["bindingUnit"];

						auto prop = material.m_Textures.find(samplerName);

						if (prop != material.m_Textures.end())
						{
							Graphics::TextureProps props;
							props.assetID = assetID;
							props.samplerName = samplerName;
							props.textureBindingUnit = bindingUnit;
							material.m_Textures[samplerName] = props;
						}

					}
				}

				if (!data["material"]["properties"].is_null())
				{
					// parsing the material properties is slow as fuck right now, but it works lol

					std::map<std::string, float> props_float;
					std::map<std::string, unsigned int> props_unsignedints;
					std::map<std::string, glm::vec2> props_float2;
					std::map<std::string, glm::vec3> props_float3;
					std::map<std::string, glm::vec4> props_float4;


					for (auto it : data["material"]["properties"].items())
					{
						std::string propertyName = it.key();
						std::string propertyType = data["material"]["properties"][propertyName]["type"];

						if (propertyType == "float")
						{
							float value = data["material"]["properties"][propertyName]["value"];
							props_float[propertyName] = value;
						}
						else if (propertyType == "float2")
						{
							float x = data["material"]["properties"][propertyName]["value"]["x"];
							float y = data["material"]["properties"][propertyName]["value"]["y"];
							
							glm::vec2 value({x, y});
							props_float2[propertyName] = value;

						}
						else if (propertyType == "float3")
						{
							float x = data["material"]["properties"][propertyName]["value"]["x"];
							float y = data["material"]["properties"][propertyName]["value"]["y"];
							float z = data["material"]["properties"][propertyName]["value"]["z"];

							glm::vec3 value({ x, y, z });
							props_float3[propertyName] = value;
						}
						else if (propertyType == "float4")
						{
							float x = data["material"]["properties"][propertyName]["value"]["x"];
							float y = data["material"]["properties"][propertyName]["value"]["y"];
							float z = data["material"]["properties"][propertyName]["value"]["z"];
							float w = data["material"]["properties"][propertyName]["value"]["w"];

							glm::vec4 value({ x, y, z, w });
							props_float4[propertyName] = value;
						}
						else if (propertyType == "uint")
						{
							unsigned int value = data["material"]["properties"][propertyName]["value"];
							props_unsignedints[propertyName] = value;
						}
					}

					if (material.m_PropertyBuffer != nullptr)
					{
						for (auto it : material.m_PropertyBuffer->GetLayout().GetElements())
						{
							ShaderDataType type = it.second.GetType();
							std::string elementName = it.first;

							switch (type)
							{
							case ShaderDataType::FLOAT:
							{
								float value = props_float[elementName];
								material.m_PropertyBuffer->SetData(elementName, value);
								break;
							}
							case ShaderDataType::FLOAT2:
							{
								glm::vec2 value = props_float2[elementName];
								material.m_PropertyBuffer->SetData(elementName, value);
								break;
							}
							case ShaderDataType::FLOAT3:
							{
								glm::vec3 value = props_float3[elementName];
								material.m_PropertyBuffer->SetData(elementName, value);
								break;
							}
							case ShaderDataType::FLOAT4:
							{
								glm::vec4 value = props_float4[elementName];
								material.m_PropertyBuffer->SetData(elementName, value);
								break;
							}
							case ShaderDataType::UNISGNED_INT:
							{
								unsigned int value = props_unsignedints[elementName];
								material.m_PropertyBuffer->SetData(elementName, value);
								break;
							}
							}

						}

						material.m_PropertyBuffer->SetName(DEFAULT_PROPERTY_BUFFER_NAME);
					}

				}
				
				return material;
		}

		Material Material::LoadFileFromID(std::string assetID)
		{
			auto desc = Application::GetAssetManager()->GetDescriptorByID(assetID);

			return LoadFile(desc.absolutePath);
		}

		void Material::SerializeShader()
		{
			ClearResources();

			auto assetManager = Application::GetAssetManager();
			auto assetDesc = assetManager->GetDescriptorByID(m_ShaderID);
			auto shaderDesc = Shader::LoadShader(assetDesc.absolutePath.c_str());

			for (ShaderProgramType shaderStage : shaderDesc.ProgramTypes)
			{
				ScopedPtr<spirv_cross::Compiler> shader;

				switch (shaderStage)
				{

				case ShaderProgramType::VERTEX:
				{
					shader = CreateScopedPtr<spirv_cross::Compiler>(shaderDesc.VertexData);
					break;
				}

				case ShaderProgramType::FRAGMENT:
				{
					shader = CreateScopedPtr<spirv_cross::Compiler>(shaderDesc.FragmentData);
					break;
				}

				case ShaderProgramType::GEOMETRY:
				{
					break;
				}

				}

				spirv_cross::ShaderResources resources = shader->get_shader_resources();

				/* Getting texture samplers from the shader */
				{
					unsigned int index = 0;
					for (auto resource : resources.sampled_images)
					{
						TextureProps props;
						props.textureBindingUnit = index;
						props.samplerName = resource.name;
						m_Textures[resource.name] = props;

						index += 1;
					}
				}
				/* ------------------------------------------ */

				/* Getting uniform buffers from the shader (for material properties) */
				{
					for (auto& resource : resources.uniform_buffers)
					{
						std::string BufferName = resource.name;
						
						if (BufferName == DEFAULT_PROPERTY_BUFFER_NAME && m_PropertyBuffer == nullptr)
						{
							auto& type = shader->get_type(resource.base_type_id);
							unsigned member_count = type.member_types.size();

							UniformBufferLayout uniformLayout;
							for (unsigned i = 0; i < member_count; i++)
							{
								const spirv_cross::SPIRType& member_type = shader->get_type(type.member_types[i]);
								spirv_cross::SPIRType::BaseType baseType = member_type.basetype;
								size_t member_size = shader->get_declared_struct_member_size(type, i);

								const std::string& name = shader->get_member_name(type.self, i);

								switch (baseType)
								{

								case spirv_cross::SPIRType::UInt:
								{
									uniformLayout.Push(name, ShaderDataType::UNISGNED_INT);
									break;
								}

								case spirv_cross::SPIRType::Float:
								{
									switch (member_type.vecsize)
									{

									case 1:
									{
										uniformLayout.Push(name, ShaderDataType::FLOAT);
										break;
									}

									case 2:
									{
										uniformLayout.Push(name, ShaderDataType::FLOAT2);
										break;
									}

									case 3:
									{
										uniformLayout.Push(name, ShaderDataType::FLOAT3);
										break;
									}

									case 4:
									{
										uniformLayout.Push(name, ShaderDataType::FLOAT4);
										break;
									}

									}
									break;
								}

								}
							}

							auto propertyBuffer = Application::GetRenderPlatform()->CreateUniformBuffer(uniformLayout);

							propertyBuffer->SetName(BufferName);
							m_PropertyBuffer = propertyBuffer;
						}

					}
				}

			}

		}

	}
}

