#include "ShaderHandler.h"

#include <Akkad/Logging.h>

#include <glslang/public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>
#include <StandAlone/ResourceLimits.cpp>
#include <spirv_glsl.hpp>

namespace Akkad {

	std::string GetStageName(EShLanguage stage)
	{
		switch (stage)
		{
		case EShLangVertex:
			return "vertex";
		case EShLangFragment:
			return "fragment";
		}
	}

	void ShaderHandler::CompileSPV(std::filesystem::path glslpath, std::filesystem::path outputPath)
	{
		static bool glslangInitialized = false;

		if (!glslangInitialized)
		{
			glslang::InitializeProcess();
			glslangInitialized = true;
		}

		std::filesystem::path shaderpath = glslpath;
		std::string shaderName = shaderpath.filename().replace_extension("").string();

		ShaderSourceDescriptor shaderDesc = LoadSourceFile(glslpath.string().c_str());
		
		EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
        TBuiltInResource Resources = glslang::DefaultTBuiltInResource;

		/* data relevant to the .shaderdesc file */
		std::vector<std::pair<Graphics::ShaderProgramType, std::string>> shaderDescInfo;

		/*
		we iterate through all shader stages in our source file
		and compile each one to .spv
		*/
		for (Graphics::ShaderProgramType shaderType : shaderDesc.ProgramTypes)
		{
			EShLanguage shaderStage;
			const char* shaderStageSource;
			switch (shaderType)
			{
			case Akkad::Graphics::ShaderProgramType::VERTEX:
			{

				shaderStage = EShLangVertex;
				shaderStageSource = shaderDesc.VertexSource.c_str();
				break;
			}

			case Akkad::Graphics::ShaderProgramType::FRAGMENT:
			{
				shaderStage = EShLangFragment;
				shaderStageSource = shaderDesc.FragmentSource.c_str();
				break;
			}
			}

			glslang::TShader Shader(shaderStage);

			/* setting up target clients and versions */
			int ClientInputSemanticsVersion = 400;
			glslang::EShTargetClientVersion OpenGLClientVersion = glslang::EShTargetOpenGL_450;
			glslang::EShTargetLanguageVersion TargetVersion = glslang::EShTargetSpv_1_5;

			/* set shader properties */
			Shader.setStrings(&shaderStageSource, 1);
			Shader.setEnvInput(glslang::EShSourceGlsl, shaderStage, glslang::EShClientOpenGL, ClientInputSemanticsVersion);
			Shader.setEnvClient(glslang::EShClientOpenGL, OpenGLClientVersion);
			Shader.setEnvTarget(glslang::EShTargetSpv, TargetVersion);
			Shader.setAutoMapLocations(true);
			Shader.setAutoMapBindings(true);

			/*  parse shader and get glsl compiler errors if any */
			if (!Shader.parse(&Resources, 400, true, messages))
			{
				AK_ERROR("{} stage parsing failed for shader {}", GetStageName(shaderStage), shaderName);
				AK_ERROR("{}", Shader.getInfoLog());
				AK_ERROR("{}", Shader.getInfoDebugLog());
				return;
			}

			glslang::TProgram Program;
			Program.addShader(&Shader);

			/* link the program and get linking errors if any */
			if (!Program.link(messages))
			{
				AK_ERROR("Shader linking failed for file : ", shaderName);
				AK_ERROR("{}", Shader.getInfoLog());
				AK_ERROR("{}", Shader.getInfoDebugLog());
				return;
			}

			std::vector<unsigned int> SpirV;
			spv::SpvBuildLogger logger;
			glslang::SpvOptions spvOptions;
			glslang::GlslangToSpv(*Program.getIntermediate(shaderStage), SpirV, &logger, &spvOptions);
			std::ofstream outFile;
			
			std::string outputfilePath = outputPath.string() + shaderName + "." + GetStageName(shaderStage) + ".spv";

			outFile.open(outputfilePath, std::ios::trunc);

			for (auto value : SpirV)
			{
				outFile << value << "\n";
			}
			outFile.flush();
			outFile.close();

			/* data relevant to the .shaderdesc file */
			std::pair<Graphics::ShaderProgramType, std::string> info;
			info.first = shaderType;
			info.second = shaderName + "." + GetStageName(shaderStage) + ".spv";

			shaderDescInfo.push_back(info);

		}

		// Writing out the .shaderdesc file for easy access later.
		std::ofstream outShaderDesc;
		std::string outputfilePath = outputPath.string() + shaderName + "."+ "shaderdesc";
		outShaderDesc.open(outputfilePath, std::ios::trunc);

		for (auto pair : shaderDescInfo)
		{
			Graphics::ShaderProgramType type = pair.first;

			switch (type)
			{
			case Akkad::Graphics::ShaderProgramType::VERTEX:
			{
				outShaderDesc << "#VERTEX_SHADER" << "\n";
				outShaderDesc << pair.second << "\n";
				break;
			}
			case Akkad::Graphics::ShaderProgramType::FRAGMENT:
			{
				outShaderDesc << "#FRAGMENT_SHADER" << "\n";
				outShaderDesc << pair.second << "\n";
				break;
			}
			}
		}

		outShaderDesc.flush();
		outShaderDesc.close();

	}

	ShaderSourceDescriptor ShaderHandler::LoadSourceFile(const char* path)
	{
		std::ifstream file(path);
		std::string line;
		std::stringstream vertexsource;
		std::stringstream fragmentsource;
		Graphics::ShaderProgramType current_type;
		ShaderSourceDescriptor desc;

		while (std::getline(file, line)) {
			if (line.find("#VERTEX_SHADER") != std::string::npos)
			{
				current_type = Graphics::ShaderProgramType::VERTEX;
				desc.ProgramTypes.push_back(Graphics::ShaderProgramType::VERTEX);
			}

			else if (line.find("#FRAGMENT_SHADER") != std::string::npos)
			{
				current_type = Graphics::ShaderProgramType::FRAGMENT;
				desc.ProgramTypes.push_back(Graphics::ShaderProgramType::FRAGMENT);
			}

			else {

				switch (current_type)
				{
				case Graphics::ShaderProgramType::VERTEX:
				{
					vertexsource << line << "\n";
					break;
				}
				case Graphics::ShaderProgramType::FRAGMENT:
				{
					fragmentsource << line << "\n";
					break;
				}

				}
			}
		}
		desc.VertexSource = vertexsource.str();
		desc.FragmentSource = fragmentsource.str();
		return desc;
	}

}